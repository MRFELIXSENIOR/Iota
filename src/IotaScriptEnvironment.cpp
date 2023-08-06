#include "IotaScriptEnvironment.hpp"
#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"
#include "IotaEvent.hpp"
#include "IotaKeyboard.hpp"
#include "IotaBasic.hpp"
#include "IotaTexture.hpp"
#include "IotaModel.hpp"

#include <SDL.h>

#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

using namespace iota;
using namespace Lua;

static sol::state lua;

static sol::table Iota;
static sol::table Enum;
static sol::table global_instance;

static std::mutex script_mutex;
static std::map<std::string, Script*> script_container;

void ErrorHandle(sol::optional<std::string> maybe_msg) {
	if (maybe_msg) {
		std::cerr << "[IOTA] [Script Error!]\n" << "[Message]: " << maybe_msg.value() << '\n';
	}
}

int EngineExceptionHandle(lua_State* L, sol::optional<const std::exception&> maybe_err, sol::string_view desc) {
	std::cerr << "[IOTA] [Script Engine Error!]\n";
	if (maybe_err) {
		std::cerr << "[Message]: ";
		std::cerr << maybe_err.value().what() << '\n';
	}
	else {
		std::cerr << "[Description]: " << desc.data() << '\n';
	}

	return sol::stack::push(L, desc);
}

sol::protected_function_result ScriptErrorHandle(lua_State* L, sol::protected_function_result prtf) {
	if (!prtf.valid()) {
		sol::error err = prtf;
		ErrorHandle(sol::make_optional(err.what()));
	}

	return prtf;
}

Script::Script(std::string file) : file_name(file) {
	auto&& script = std::make_pair(file_name.data(), this);
	script_container.insert(script);
}

Script::~Script() {
	script_container.erase(file_name.data());
}

void Script::Run() {
	execution_result = lua.script_file(file_name.data(), ScriptErrorHandle);
	if (attached) {
		lua[attached_instance_name] = sol::lua_nil;
	}
}

#pragma region BindingTypes

template <typename... Args>
sol::usertype<Event::EventSignal<Args...>> BindScriptSignalType() {
	sol::usertype<Event::EventSignal<Args...>> script_signal = lua.new_usertype<Event::EventSignal<Args...>>("ScriptSignal", sol::no_constructor);
	script_signal["Connect"] = &Event::EventSignal<Args...>::Connect;
	script_signal["Disconnect"] = &Event::EventSignal<Args...>::Disconnect;

	return script_signal;
}

template <typename T>
sol::usertype<GameInstance::Property<T>> BindPropertyType() {
	sol::usertype<Property<T>> property = lua.new_usertype<Property<T>>("Property", sol::no_constructor);
	property["Value"] = sol::property(&Property<T>::data, &Property<T>::set);
	property.set("Name", sol::readonly(&Property<T>::property_name));

	BindScriptSignalType<T>();

	return property;
}

#pragma endregion

#pragma region Utilities

sol::table GetCenter(Window& self) {
	int x = self.GetCenterX();
	int y = self.GetCenterY();

	sol::table center = Lua::GetState().create_table();
	center["x"] = x;
	center["y"] = y;

	return center;
}

template <GameInstance::IsInstance T, typename... Args>
std::optional<T> Instance_new(const std::string& class_name, Args... args) {
	static std::unordered_map<std::string, std::function<T()>> m = {
		"Object", [&]() -> Object {
			return Object(args...);
		},
		"GUIObject", [&]() -> GUIObject {
			return GUIObject(args...);
		}
	};

	auto it = m.find(class_name);
	if (it != m.end()) {
		return it->second();
	}

	return std::nullopt;
}

#pragma endregion

void Lua::LoadSTD() {
	if (Application::IsInitialized()) return;
	lua.open_libraries(sol::lib::base, sol::lib::package);

	lua.set_exception_handler(&EngineExceptionHandle);
	lua.set_panic(sol::c_call<decltype(&ErrorHandle), &ErrorHandle>);

	std::cout << "[INFO] Lua STD Loaded!\n";

	Iota = lua.create_named_table("Iota");
	Enum = lua.create_named_table("Enum");

	global_instance = lua.create_named_table("Instance");

	Iota["GetWindow"] = Application::GetWindow;
	Iota["GetRenderer"] = Application::GetRenderer;

	Iota["Util"] = lua.create_table();

	sol::usertype<Color> color = lua.new_usertype<Color>(
		"Color",
		sol::constructors<Color(), Color(uint8_t, uint8_t, uint8_t, uint8_t)>()
	);
	color["Red"] = &Color::red;
	color["Green"] = &Color::green;
	color["Blue"] = &Color::blue;
	color["Alpha"] = &Color::alpha;

	sol::usertype<Window> window = lua.new_usertype<Window>("Window", sol::constructors<Window(), Window(std::string, unsigned int, unsigned int)>());
	window["Create"] = &Window::Create;
	window["Destroy"] = &Window::Destroy;
	window["GetCenter"] = &GetCenter;

	sol::usertype<Renderer> renderer = lua.new_usertype<Renderer>("Renderer", sol::constructors<Renderer(Window&), Renderer(Window&, bool)>());

	sol::usertype<RenderSurface> surface = lua.new_usertype<RenderSurface>(
		"RenderSurface",
		sol::constructors<RenderSurface(), RenderSurface(int, int, unsigned int, unsigned int), RenderSurface(Vector::Vec2<int>, Vector::Vec2<unsigned int>)>()
	);

	BindVectorType<int>();
	BindVectorType<unsigned int>();

	surface.set_function("SetPosition", sol::overload(
		static_cast<void (RenderSurface::*)(Vector::Vec2<int>)>(&RenderSurface::SetPosition),
		static_cast<void (RenderSurface::*)(int, int)>(&RenderSurface::SetPosition)
	));

	surface.set_function("Resize", sol::overload(
		static_cast<void (RenderSurface::*)(Vector::Vec2<unsigned int>)>(&RenderSurface::Resize),
		static_cast<void (RenderSurface::*)(unsigned int, unsigned int)>(&RenderSurface::Resize)
	));

	Enum["Shape"] = lua.create_table();

	Enum["Shape"]["Rectangle"] = ObjectShape::RECTANGLE;
	Enum["Shape"]["Circle"] = ObjectShape::CIRCLE;
	Enum["Shape"]["Triangle"] = ObjectShape::TRIANGLE;

	BindScriptSignalType<Keyboard::KeyCode>();

	Iota["Input"] = lua.create_table();
	Iota["Input"]["OnKeyDown"] = Keyboard::GetKeydownEvent();
	Iota["Input"]["OnKeyRelease"] = Keyboard::GetKeyupEvent();

	Iota["Input"]["IsKeyDown"] = Keyboard::IsKeyDown;
	Iota["Input"]["IsKeyRelease"] = Keyboard::IsKeyReleased;

	Enum["KeyCode"] = lua.create_table();
	for (auto& k : Keyboard::GetKeyEntries()) {
		Enum["KeyCode"][Keyboard::GetKeystringEntries().at(k.second)] = (int)k.first;
	}

	Iota["Util"]["ConvertKeyCode"] = [&](Keyboard::KeyCode key) {
		return Keyboard::GetKeystringEntries().at(key);
	};

	BindPropertyType<std::string>();

	sol::usertype<GameInstance::Instance> instance = lua.new_usertype<GameInstance::Instance>(
		"GameInstance",
		sol::no_constructor
	);

	instance["Name"] = &GameInstance::Instance::name;

	BindVectorType<PosType>();
	BindVectorType<SizeType>();
	BindPropertyType<PosType>();
	BindPropertyType<SizeType>();

	sol::usertype<Object> object = lua.new_usertype<Object>(
		"Object",
		sol::constructors<
		Object(),
		Object(ObjectShape),
		Object(Vector::Vec2<PosType>, Vector::Vec2<SizeType>, ObjectShape),
		Object(Vector::Vec2<PosType>, Vector::Vec2<SizeType>),
		Object(PosType, PosType, SizeType, SizeType, ObjectShape),
		Object(PosType, PosType, SizeType, SizeType)
		>()
	);

	object["Color"] = sol::property(&Object::GetColor, &Object::SetColor);
	object["SetColorRGB"] = &Object::SetColorRGB;
	object["x"] = sol::property(&Object::x, &Object::SetY);
	object["y"] = sol::property(&Object::y, &Object::SetY);
	object["width"] = sol::property(&Object::width, &Object::SetWidth);
	object["height"] = sol::property(&Object::height, &Object::SetHeight);

	global_instance["new"] = sol::overload(
		Instance_new<Object>,
		Instance_new<Object, ObjectShape>,
		Instance_new<Object, Vector::Vec2<PosType>, Vector::Vec2<SizeType>>,
		Instance_new<Object, Vector::Vec2<PosType>, Vector::Vec2<SizeType>, ObjectShape>,
		Instance_new<Object, PosType, PosType, SizeType, SizeType>,
		Instance_new<Object, PosType, PosType, SizeType, SizeType, ObjectShape>,

		Instance_new<GUIObject>,
		Instance_new<GUIObject, PosType, PosType, SizeType, SizeType>,
		Instance_new<GUIObject, Vector::Vec2<PosType>, Vector::Vec2<SizeType>>
	);

	sol::usertype<Texture> texture = lua.new_usertype<Texture>(
		"Texture",
		sol::constructors<Texture()>());

	texture.set_function("LoadTexture", sol::overload(
		static_cast<Texture & (Texture::*)(const std::string&)>(&Texture::LoadTexture),
		static_cast<Texture & (Texture::*)(const std::string&, Renderer&)>(&Texture::LoadTexture)
	));

}

void Lua::Clean() {
	std::vector<std::string> usertypes = {
		//default UserTypes
		"Vector2",
		"ScriptSignal",
		"Property",
		"Texture",
		"Object",
		"Color",
		"RenderSurface",
		"Window",
		"Renderer"
	};


	for (const std::string& name : usertypes) {
		lua.set(name, sol::nil);
	}

	lua.collect_garbage();
}

void run_all_script() {
	std::lock_guard<std::mutex> lock(script_mutex);

	for (auto& s : script_container) {
		s.second->Run();
	}
}

void Lua::RunAllScript() {
	std::thread thread(run_all_script);

	if (thread.joinable()) {
		thread.join();
	}
}
