#include "IotaModel.hpp"
#include "IotaVector.hpp"
#include "IotaScriptEnvironment.hpp"
#include "IotaBasic.hpp"
#include "IotaApplication.hpp"

#include <SDL.h>

using namespace iota;

Object::Object(): shape(ObjectShape::RECTANGLE), GameInstance::Instance() {
	rs.width() = 100;
	rs.height() = 100;

	rs.x() = Application::GetWindow().GetCenterX() - (rs.width() / 2);
	rs.y() = Application::GetWindow().GetCenterY() - (rs.height() / 2);
}

Object::Object(ObjectShape sh): shape(sh), GameInstance::Instance() {
	rs.width() = 100;
	rs.height() = 100;

	rs.x() = Application::GetWindow().GetCenterX() - (rs.width() / 2);
	rs.y() = Application::GetWindow().GetCenterY() - (rs.height() / 2);
}
Object::Object(Vector::Vec2<int> pos, Vector::Vec2<unsigned int> size, ObjectShape sh): shape(sh), GameInstance::Instance() {
	rs.width() = size.x;
	rs.height() = size.y;
	
	rs.x() = pos.x;
	rs.y() = pos.y;
}

Object::Object(int x, int y, unsigned int w, unsigned int h, ObjectShape sh): shape(sh), GameInstance::Instance() {
	rs.width() = w;
	rs.height() = h;

	rs.x() = x;
	rs.y() = y;
}
Object::~Object() {}

void Object::SetColor(Color c) { rs.color = c; }
void Object::SetColorRGB(uint8_t red, uint8_t green, uint8_t blue) {
	rs.color.red = red;
	rs.color.green = green;
	rs.color.blue = blue;
}

Color Object::GetColor() { return rs.color; }

void Object::SetX(int x) { rs.x() = x; }
int Object::GetX() { return rs.x(); }

void Object::SetY(int y) { rs.y() = y; }
int Object::GetY() { return rs.y(); }

void Object::SetWidth(int width) { rs.width() = width; }
int Object::GetWidth() { return rs.width(); }

void Object::SetHeight(int height) { rs.height() = height; }
int Object::GetHeight() { return rs.height(); }

void Object::Load() {}

void Object::Render() {
	actor_renderer->SetDrawColor(rs.color);
	actor_renderer->DrawCircle(Basic::DrawMode::FILL, rs);
}

void Object::Update() {}

void Model::LoadLuaSTD() {
	sol::state& lua = Lua::GetState();
	sol::table& Iota = Lua::GetIota();
	sol::table& Enum = Lua::GetEnum();

	BindVectorType<int>();
	BindVectorType<unsigned int>();
	GameInstance::BindPropertyType<int>();
	GameInstance::BindPropertyType<unsigned int>();

	sol::usertype<Object> object = lua.new_usertype<Object>("Object", sol::constructors<Object(), Object(ObjectShape), Object(Vector::Vec2<int>, Vector::Vec2<unsigned int>, ObjectShape), Object(int, int, unsigned int, unsigned int, ObjectShape)>());
	object["Color"] = sol::property(&Object::GetColor, &Object::SetColor);
	object["SetColorRGB"] = &Object::SetColorRGB;
	object["x"] = sol::property(&Object::GetX, &Object::SetY);
	object["y"] = sol::property(&Object::GetY, &Object::SetY);
	object["width"] = sol::property(&Object::GetWidth, &Object::SetWidth);
	object["height"] = sol::property(&Object::GetHeight, &Object::SetHeight);
}
