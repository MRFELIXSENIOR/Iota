#include "IotaModel.hpp"
#include "IotaVector.hpp"
#include "IotaBasic.hpp"
#include "IotaApplication.hpp"

#include <SDL.h>

using namespace iota;

#define DEFAULT_SHAPE ObjectShape::RECTANGLE

Object::Object(): 
	shape(DEFAULT_SHAPE), 
	RenderSurface(
		Application::GetWindow().GetRelativeCenterX(100),
		Application::GetWindow().GetRelativeCenterY(100),
		100,
		100
	),
	GameInstance::Instance()
{}

Object::Object(ObjectShape sh): 
	shape(sh), 
	RenderSurface(
		Application::GetWindow().GetRelativeCenterX(100),
		Application::GetWindow().GetRelativeCenterY(100),
		100,
		100
	),
	GameInstance::Instance()
{}

Object::Object(Vector::Vec2<PosType> pos, Vector::Vec2<SizeType> size): 
	shape(DEFAULT_SHAPE), 
	RenderSurface(pos.x, pos.y, size.x, size.y),
	GameInstance::Instance()
{}

Object::Object(Vector::Vec2<PosType> pos, Vector::Vec2<SizeType> size, ObjectShape sh) :
	shape(sh),
	RenderSurface(pos.x, pos.y, size.x, size.y),
	GameInstance::Instance()
{}

Object::Object(PosType x, PosType y, SizeType w, SizeType h): 
	shape(DEFAULT_SHAPE), 
	RenderSurface(x, y, w, h),
	GameInstance::Instance() 
{}

Object::Object(PosType x, PosType y, SizeType w, SizeType h, ObjectShape sh) :
	shape(sh),
	RenderSurface(x, y, w, h),
	GameInstance::Instance()
{}

Object::~Object() {}

void Object::SetColor(Color c) { color = c; }
void Object::SetColorRGB(uint8_t red, uint8_t green, uint8_t blue) {
	color.red = red;
	color.green = green;
	color.blue = blue;
}

Color Object::GetColor() { return color; }

void Object::SetX(PosType pos_x) { x() = pos_x; }
void Object::SetY(PosType pos_y) { y() = pos_y; }
void Object::SetWidth(PosType w) { width() = w; }
void Object::SetHeight(PosType h) { height() = h; }

void Object::Load() {}

void Object::Render() {
	actor_renderer->SetDrawColor(color);
	actor_renderer->Draw(
		shape,
		Basic::DrawMode::FILL,
		*this
		);
}

void Object::Update() {}

GUIObject::GUIObject():
	RenderSurface(
		Application::GetWindow().GetRelativeCenterX(100),
		Application::GetWindow().GetRelativeCenterY(100),
		100,
		100
	),
	GameInstance::Instance()
{}

GUIObject::GUIObject(PosType x, PosType y, SizeType w, SizeType h) :
	RenderSurface(x, y, w, h),
	GameInstance::Instance()
{}

GUIObject::GUIObject(Vector::Vec2<PosType> pos, Vector::Vec2<SizeType> size):
	RenderSurface(pos, size),
	GameInstance::Instance()
{}

GUIObject::~GUIObject() {}
