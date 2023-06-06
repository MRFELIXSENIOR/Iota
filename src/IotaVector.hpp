#pragma once

namespace IotaEngine {
namespace IotaVector {

template<typename T>
struct Vector2 final {
public:
    T x;
    T y;

    constexpr Vector2() = default;
    constexpr Vector2(T x, T y): x(x), y(y) {}

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
};

};
};
