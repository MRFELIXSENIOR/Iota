#pragma once

#include "IotaEvent.hpp"

namespace iota {
	template <typename T>
	struct ValuePointedProperty;

	template <typename T>
	struct ValuePointedProperty<Vector2<T>>;

	template <typename T, typename PropertyClass>
	struct PropertyValueRef {
	protected:
		T& ref;
		PropertyClass& prop_ref;

	public:
		PropertyValueRef(T& value_ref, PropertyClass& property_class) : ref(value_ref), prop_ref(property_class) {}

		PropertyValueRef<T, PropertyClass>& operator=(const T& other) { 
			prop_ref.signal.Fire(ref);
			ref = other;
			return *this;
		}

		template <int = 0>
		requires std::is_arithmetic_v<T>
		PropertyValueRef<T, PropertyClass>& operator+(const T& other) { return ref + other; return *this; }

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, PropertyClass>& operator-(const T& other) { return ref - other; return *this; }

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, PropertyClass>& operator*(const T& other) { return ref * other; return *this; }

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, PropertyClass>& operator/(const T& other) { return ref / other; return *this; }

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, PropertyClass>& operator+=(const T& other) { 
			prop_ref.signal.Fire(ref);
			ref += other;
			return *this;
		}

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, PropertyClass>& operator-=(const T& other) { 
			prop_ref.signal.Fire(ref);
			ref -= other;
			return *this;
		}

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, PropertyClass>& operator*=(const T& other) { 
			prop_ref.signal.Fire(ref);
			ref *= other;
			return *this;
		}

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, PropertyClass>& operator/=(const T& other) { 
			prop_ref.signal.Fire(ref);
			ref /= other;
			return *this;
		}

		bool operator==(const T& other) { return (ref == other); }
		bool operator!=(const T& other) { return !(*this == other); }

		operator T() const {
			return ref;
		}

		std::ostream& operator<<(std::ostream& os) {
			os << ref;
			return os;
		}
	};

	template <typename T>
	struct PropertyValueRef<T, ValuePointedProperty<Vector2<T>>> {
	private:
		T& ref;
		ValuePointedProperty<Vector2<T>>& prop_ref;

	public:
		PropertyValueRef(T& value_ref, ValuePointedProperty<Vector2<T>>& property_class) : ref(value_ref), prop_ref(property_class) {}

		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>& operator=(const T& other) {
			prop_ref.signal.Fire(Vector2<T>(*prop_ref.xptr, *prop_ref.yptr));
			ref = other;
			return *this;
		}

		template <int = 0>
		requires std::is_arithmetic_v<T>
		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>& operator+(const T& other) { return ref + other; return *this; }

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>& operator-(const T& other) { return ref - other; return *this; }

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>& operator*(const T& other) { return ref * other; return *this; }

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>& operator/(const T& other) { return ref / other; return *this; }

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>& operator+=(const T& other) {
			prop_ref.signal.Fire(Vector2<T>(*prop_ref.xptr, *prop_ref.yptr));
			ref += other;
			return *this;
		}

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>& operator-=(const T& other) {
			prop_ref.signal.Fire(Vector2<T>(*prop_ref.xptr, *prop_ref.yptr));
			ref -= other;
			return *this;
		}

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>& operator*=(const T& other) {
			prop_ref.signal.Fire(Vector2<T>(*prop_ref.xptr, *prop_ref.yptr));
			ref *= other;
			return *this;
		}

		template <int = 0>
			requires std::is_arithmetic_v<T>
		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>& operator/=(const T& other) {
			prop_ref.signal.Fire(Vector2<T>(*prop_ref.xptr, *prop_ref.yptr));
			ref /= other;
			return *this;
		}

		bool operator==(const T& other) { return (*ref == other); }
		bool operator!=(const T& other) { return !(*this == other); }

		operator T() const {
			return ref;
		}

		std::ostream& operator<<(std::ostream& os) {
			os << ref;
			return os;
		}
	};

	template <typename T>
	requires std::is_copy_constructible_v<T>
	struct Property {
	protected:
		T value;
		Event<T> signal;

		friend struct PropertyValueRef<T, Property<T>>;

	public:
		Property() : Value(value, *this) {}
		explicit Property(T val) : value(val), Value(value, *this) {}

		PropertyValueRef<T, Property<T>> Value;

		Property<T>& operator=(const T& rhs) { Value.operator=(rhs); }
		ScriptSignal<T> GetValueChangedEvent() { return ScriptSignal<T>(signal); }

		bool operator==(const T& rhs) { return (Value == rhs); }
		bool operator!=(const T& rhs) { return !(*this == rhs); }
	};

	template <typename T>
	struct ValuePointedProperty {
	private:
		T* value = nullptr;
		Event<T> signal;

		friend struct PropertyValueRef<T, ValuePointedProperty<T>>;

	public:
		ValuePointedProperty(T& value_ref) : value(&value_ref), Value(*value, *this) {}

		PropertyValueRef<T, ValuePointedProperty<T>> Value;

		ValuePointedProperty<T>& operator=(const T& rhs) { Value.operator=(rhs); }
		ScriptSignal<T> GetValueChangedEvent() { return ScriptSignal<T>(signal); }

		bool operator==(const T& rhs) { return (Value == rhs); }
		bool operator!=(const T& rhs) { return !(*this == rhs); }
	};

	template <typename T>
	struct ValuePointedProperty<Vector2<T>> {
	private:
		T* xptr = nullptr;
		T* yptr = nullptr;
		Event<Vector2<T>> signal;

		friend struct PropertyValueRef<T, ValuePointedProperty<Vector2<T>>>;

	public:
		ValuePointedProperty(T& argx, T& argy) : xptr(&argx), yptr(&argy), x(*xptr, *this), y(*yptr, *this) {}

		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>> x;
		PropertyValueRef<T, ValuePointedProperty<Vector2<T>>> y;

		ValuePointedProperty<T>& operator=(const Vector2<T>& rhs) { 
			x.operator=(rhs.x);
			y.operator=(rhs.y);
		}
		ScriptSignal<Vector2<T>> GetValueChangedEvent() { return ScriptSignal<Vector2<T>>(signal); }

		bool operator==(const Vector2<T>& other) { return (x == other.x) && (y == other.y); }
		bool operator!=(const Vector2<T>& other) { return !(*this == other); }
	};
}