#pragma once

#include "Object.hpp"

namespace iota {
	namespace Mono {
		struct String : public Object {
		public:
			explicit String(const Object& object);
			explicit String(const std::string& str);

			const std::string& ToUTF8() const;
			const std::u16string& ToUTF16() const;
			const std::u32string& ToUTF32() const;
		};
	}
}