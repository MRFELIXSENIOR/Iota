#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"
#include "IotaEvent.hpp"
#include "IotaBasic.hpp"
#include "IotaBehavior.hpp"

#include <map>

using namespace iota;
using namespace GameInstance;

uint64_t GameBehavior::GetID() { return id; }

Instance::Instance(): GameBehavior() {}

Instance::~Instance() {}

void Instance::Load() {}
void Instance::Render() {}
void Instance::Update() {}