#pragma once

#include <string>

#include <mono/metadata/loader.h>

namespace iota {
	namespace Mono {
		template <typename Fn>
		void AddInternalCall(const std::string& name, Fn&& fn) {
			mono_add_internal_call(name.c_str(), reinterpret_cast<const void*>(fn));
		}
	}
}