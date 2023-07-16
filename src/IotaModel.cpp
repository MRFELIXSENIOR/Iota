#include "IotaModel.hpp"
#include "IotaVector.hpp"
#include "IotaScriptEnvironment.hpp"
#include "IotaApplication.hpp"

#include <SDL.h>

using namespace iota;

Object::Object(): pos_x(Application::GetWindow().GetCenterX() - 20), pos_y(Application::GetWindow().GetCenterY() - 20), width(40), height(40), GameInstance::Instance() {}
Object::Object(Vector::Vec2<int> pos, Vector::Vec2<unsigned int> size): pos_x(pos.x), pos_y(pos.y), width(size.x), height(size.y), GameInstance::Instance() {}
Object::Object(int x, int y, unsigned int w, unsigned int h) : pos_x(x), pos_y(y), width(w), height(h), GameInstance::Instance() {}
Object::~Object() {}

void Object::SetColor(Color c) { color = c; }
void Object::SetColorRGB(uint8_t red, uint8_t green, uint8_t blue) {
	color.red = red;
	color.green = green;
	color.blue = blue;
}

Color Object::GetColor() { return color; }

void Object::Load() {
	rs = RenderSurface(pos_x.data(), pos_y.data(), width.data(), height.data());

	pos_x.GetValueChangedSignal().Connect([&](int oldvalue) { rs.SetPosition(pos_x.data(), pos_y.data()); });
	pos_y.GetValueChangedSignal().Connect([&](int oldvalue) { rs.SetPosition(pos_x.data(), pos_y.data()); });
	width.GetValueChangedSignal().Connect([&](unsigned int oldvalue) { rs.Resize(width.data(), height.data()); });
	height.GetValueChangedSignal().Connect([&](unsigned int oldvalue) { rs.Resize(width.data(), height.data()); });
}

void Object::Render() {
	renderer->SetDrawColor(color);
	renderer->DrawRectangle(Basic::DrawMode::FILL, rs);
}

void Object::Update() {}

void Model::LoadLuaSTD() {
	sol::state& lua = Lua::GetState();
	sol::table& Iota = Lua::GetIota();

	BindVectorType<int>();
	BindVectorType<unsigned int>();
	GameInstance::BindPropertyType<int>();
	GameInstance::BindPropertyType<unsigned int>();

	sol::usertype<Object> object = lua.new_usertype<Object>("Object", sol::constructors<Object(), Object(Vector::Vec2<int>, Vector::Vec2<unsigned int>), Object(int, int, unsigned int, unsigned int)>());
	object["Color"] = sol::property(&Object::GetColor, &Object::SetColor);
	object["SetColorRGB"] = &Object::SetColorRGB;
	object["x"] = &Object::pos_x;
	object["y"] = &Object::pos_y;
	object["width"] = &Object::width;
	object["height"] = &Object::height;
}
