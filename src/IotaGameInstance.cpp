#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"
#include "IotaEvent.hpp"
#include "IotaBasic.hpp"

#include <map>

using namespace iota;

ActorInterface::ActorInterface() {
	ActorInterface* ac = static_cast<ActorInterface*>(this);
	GetCont().push_back(ac);
}

ActorInterface::ActorContType& ActorInterface::GetCont() {
	static ActorContType ct;
	return ct;
}

void ActorInterface::Init() {
	window = std::make_shared<Window>(Window::GetFocusedWindow().GetDataPointer());
}
	
Instance::Instance() : parent(nullptr), ParentChanged(parent_changed), ChildAdded(child_added) {}
Instance::~Instance() {}

void Instance::Load() {}
void Instance::Render() {}
void Instance::Update() {}