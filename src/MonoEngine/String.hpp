#pragma once

#include "Object.hpp"

namespace iota {
	namespace Mono {
		struct String : public Object {
		public:
			explicit String(const Object& object);
			explicit String(const std::string& str);

			const std::string& utf8() const;
			const std::u16string& utf16() const;
			const std::u32string& utf32() const;
		};
	}
}