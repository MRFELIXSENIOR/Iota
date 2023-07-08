#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"

using namespace iota;
using namespace GameInstance;

Instance::Instance(): parent(nullptr) {}
Instance::~Instance() {
	Destroy();
}

void Instance::Destroy() {
	destroying.Fire();
	parent = nullptr;
	ClearAllChildren();
}

std::vector<Instance*> Instance::GetChildren() { return children; }

void Instance::ClearAllChildren() {
	for (Instance* inst : children) {
		inst->Destroy();
	}
	children.clear();
}