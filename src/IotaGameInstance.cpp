#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"

#include <format>
#include <iostream>

using namespace IotaEngine;
using namespace GameInstance;

Instance::Instance() {}
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

void Instance::Update() {}
void Instance::Render() {}
void Instance::Initialize() {}