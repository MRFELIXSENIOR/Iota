#include "Engine.hpp"

using namespace iota;
using namespace Mono;

void Mono::Initialize() {
	JIT::Initialize(false);
}

void Mono::Clean() {
	JIT::Clean();
}