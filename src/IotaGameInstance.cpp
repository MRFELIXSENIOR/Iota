#include "IotaGameInstance.hpp"

using namespace IotaEngine;
using namespace GameInstance;

Instance::Instance()
	: changed(new Event::EventSignal<>),
	child_added(new Event::EventSignal<Instance*>),
	parent_changed(new Event::EventSignal<Instance*>),
	destroying(new Event::EventSignal<>) {}
Instance::Instance(std::string_view name)
	: changed(new Event::EventSignal<>),
	child_added(new Event::EventSignal<Instance*>),
	parent_changed(new Event::EventSignal<Instance*>),
	destroying(new Event::EventSignal<>), name(name) {}
Instance::~Instance() {}

void Instance::Destroy() {
	destroying->Fire();
	for (Instance* inst : children) {
		inst->Destroy();
	}

	SetParent(nullptr);
	name = "";
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
		if (inst->name == name)
			return inst;
	}
	return nullptr;
}

void Instance::AddChildren(Instance* instance) {
	child_added->Fire(this);
	children.push_back(instance);
}

void Instance::SetParent(Instance* instance) {
	this->parent = instance;
	parent_changed->Fire(instance);
	instance->AddChildren(this);
}
