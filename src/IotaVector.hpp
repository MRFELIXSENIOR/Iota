#pragma once

#include <iostream>

namespace iota {
	template<typename T>
	struct Vector2 final {
		T x;
		T y;

		constexpr Vector2() = default;
		constexpr Vector2(T x, T y) : x(x), y(y) {}

		//Vector
		constexpr Vector2<T>& operator+=(const Vector2<T>& v) noexcept { x += v.x;  y += v.y; return *this; }
		constexpr Vector2<T>& operator-=(const Vector2<T>& v) noexcept { x -= v.x;  y -= v.y; return *this; }
		constexpr Vector2<T>& operator*=(const Vector2<T>& v) noexcept { x *= v.x;  y *= v.y; return *this; }
		constexpr Vector2<T>& operator/=(const Vector2<T>& v) { x /= v.x;  y /= v.y; return *this; }

		//Scalar
		constexpr Vector2<T>& operator+=(const T& s) noexcept { x += s;  y += s; return *this; }
		constexpr Vector2<T>& operator-=(const T& s) noexcept { x -= s;  y -= s; return *this; }
		constexpr Vector2<T>& operator*=(const T& s) noexcept { x *= s;  y *= s; return *this; }
		constexpr Vector2<T>& operator/=(const T& s) { x /= s;  y /= s; return *this; }

		constexpr bool operator==(const Vector2<T>& other) { return x == other.x && y == other.y; }
		constexpr bool operator!=(const Vector2<T>& other) { return !(*this == other); }

		template <int = 0>
		requires std::is_signed_v<T>
		constexpr Vector2<T> operator-() const noexcept { return Vector2<T>(-x, -y); }

		constexpr Vector2<T> operator+(const Vector2<T>& other) noexcept { return Vector2<T>(*this) += other; }
		constexpr Vector2<T> operator-(const Vector2<T>& other) noexcept { return Vector2<T>(*this) -= other; }
		constexpr Vector2<T> operator*(const Vector2<T>& other) noexcept { return Vector2<T>(*this) *= other; }
		constexpr Vector2<T> operator/(const Vector2<T>& other) noexcept { return Vector2<T>(*this) /= other; }

		std::ostream& operator<<(std::ostream& os) {
			os << '[' << x << ", " << y << "]\n";
			return os;
		}

		T Magnitude() { return std::sqrt(x * x + y * y); }
	};
};
