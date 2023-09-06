#pragma once

#include "Context.hpp"
#include "Conversion.hpp"
#include "Field.hpp"
#include "Function.hpp"
#include "FunctionInvoker.hpp"
#include "InternalCall.hpp"
#include "JIT.hpp"
#include "Object.hpp"
#include "Property.hpp"
#include "String.hpp"
#include "TypeClass.hpp"
#include "TypeTraits.hpp"

namespace iota {
	namespace Mono {
		void Initialize();
		void Clean();
	}
}
