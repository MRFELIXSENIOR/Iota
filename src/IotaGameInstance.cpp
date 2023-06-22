#include "IotaGameInstance.hpp"

using namespace IotaEngine;
using namespace GameInstance;

InstanceCore::InstanceCore()
	: changed(new Event::EventSignal<>),
	child_added(new Event::EventSignal<Instance*>),
	parent_changed(new Event::EventSignal<Instance*>),
	destroying(new Event::EventSignal<>) {}
InstanceCore::InstanceCore(std::string_view name)
	: changed(new Event::EventSignal<>),
	child_added(new Event::EventSignal<Instance*>),
	parent_changed(new Event::EventSignal<Instance*>),
	destroying(new Event::EventSignal<>), name(name) {}
InstanceCore::~InstanceCore() {}


Instance::Instance() {}
Instance::~Instance() {}

void InstanceCore::FireVoidEvent(Event::EventSignal<>* event) {
	event->Fire();
}

void InstanceCore::FireInstanceEvent(Event::EventSignal<Instance*>* event, Instance* inst) {
	event->Fire(inst);
}

void Instance::Destroy() {
	core.FireVoidEvent(core.destroying);
	for (Instance* inst : children) {
		inst->Destroy();
	}

	SetParent(nullptr);
	core.name.empty();
}

std::vector<Instance*>& Instance::GetChildren() { return children; }

void Instance::ClearAllChildren() {
	for (Instance* inst : children) {
		inst->Destroy();
	}
	children.clear();
}

Instance* Instance::FindFirstChild(std::string_view name) {
	for (Instance* inst : children) {
		if (inst->core.name == name)
			return inst;
	}
	return nullptr;
}

void Instance::AddChildren(Instance* instance) {
	core.FireInstanceEvent(core.child_added, this);
	children.push_back(instance);
}

void Instance::SetParent(Instance* instance) {
	this->parent = instance;
	core.FireInstanceEvent(core.parent_changed, this);
	instance->AddChildren(this);
}

void Instance::Update() {}
void Instance::Render() {}
void Instance::Initialize() {}

using namespace GameComponent;

Component::Component() {}
Component::~Component() {}

void Component::Update() {}
void Component::Render() {}
void Component::Initialize() {}
