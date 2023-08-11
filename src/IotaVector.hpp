#pragma once

#include <iostream>

namespace iota {
	namespace Vector {
		template<typename T>
		concept IsArithmetic = std::is_arithmetic_v<T>;

		template<IsArithmetic T>
		struct Vec2 final {
			T x;
			T y;

			constexpr Vec2() = default;
			constexpr Vec2(T x, T y) : x(x), y(y) {}

			//Vector
			constexpr Vec2<T>& operator+=(const Vec2<T>& v) noexcept { x += v.x;  y += v.y; return *this; }
			constexpr Vec2<T>& operator-=(const Vec2<T>& v) noexcept { x -= v.x;  y -= v.y; return *this; }
			constexpr Vec2<T>& operator*=(const Vec2<T>& v) noexcept { x *= v.x;  y *= v.y; return *this; }
			constexpr Vec2<T>& operator/=(const Vec2<T>& v) { x /= v.x;  y /= v.y; return *this; }

			//Scalar
			constexpr Vec2<T>& operator+=(const T& s) noexcept { x += s;  y += s; return *this; }
			constexpr Vec2<T>& operator-=(const T& s) noexcept { x -= s;  y -= s; return *this; }
			constexpr Vec2<T>& operator*=(const T& s) noexcept { x *= s;  y *= s; return *this; }
			constexpr Vec2<T>& operator/=(const T& s) { x /= s;  y /= s; return *this; }

			std::ostream& operator<<(std::ostream& c) {
				c << '[' << x << ", " << y << ']';
				return c;
			}

			T Magnitude() { return std::sqrt(x * x + y * y); }
		};
	};
};
