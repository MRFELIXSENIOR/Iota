#pragma once

#include <mono/metadata/object.h>

#include <map>
#include <cstddef>
#include <functional>
#include <utility>
#include <tuple>
#include <string>

namespace iota {
	namespace Mono {
		template <typename T>
		concept IsInvokable = requires(T t) {
			{ t() };
		};

		template <IsInvokable Fn>
		struct FunctionTraits : public FunctionTraits<decltype(&Fn::operator())> {};

		template <typename RT, typename... Args>
		struct FunctionTraits<RT(Args...)> {
			using ReturnType = RT;
			using FullSignature = RT(Args...);
			static constexpr size_t ArgCount = sizeof...(Args);
			using ArgTypes = std::tuple<Args...>;
			using ArgTypesDecayed = std::tuple<std::decay_t<Args>...>;
		};

		template <typename RT, typename... Args>
		struct FunctionTraits<RT(*)(Args...)> : public FunctionTraits<RT(Args...)> {};

		template <typename OC, typename RT, typename... Args>
		struct FunctionTraits<RT(OC::*)(Args...)> : public FunctionTraits<RT(Args...)> {
			using OwnerType = OC&;
		};

		template <typename OC, typename RT, typename... Args>
		struct FunctionTraits<RT(OC::*)(Args...) const> : public FunctionTraits<RT(Args...)> {
			using OwnerType = const OC&;
		};

		template <typename OC, typename RT, typename... Args>
		struct FunctionTraits<RT(OC::*)(Args...) volatile> : public FunctionTraits<RT(Args...)> {
			using OwnerType = volatile OC&;
		};

		template <typename OC, typename RT, typename... Args>
		struct FunctionTraits<RT(OC::*)(Args...) const volatile> : public FunctionTraits<RT(Args...)> {
			using OwnerType = const volatile OC&;
		};

		template <typename Fn>
		struct FunctionTraits<std::function<Fn>> : public FunctionTraits<Fn> {};

		template <typename T>
		struct FunctionTraits<T&> : public FunctionTraits<T> {};

		template <typename T>
		struct FunctionTraits<const T&> : public FunctionTraits<T> {};

		template <typename T>
		struct FunctionTraits<volatile T&> : public FunctionTraits<T> {};

		template <typename T>
		struct FunctionTraits<const volatile T&> : public FunctionTraits<T> {};

		template <typename T>
		struct FunctionTraits<T&&> : public FunctionTraits<T> {};

		template <typename T>
		struct FunctionTraits<const T&&> : public FunctionTraits<T> {};

		template <typename T>
		struct FunctionTraits<volatile T&&> : public FunctionTraits<T> {};

		template <typename T>
		struct FunctionTraits<const volatile T&&> : public FunctionTraits<T> {};

		/////////////////////////////////////

#define UNKNOWN_NAME "unknown"
#define UNKNOWN_FULLNAME "UnknownType"

		struct TypeName {
			std::string name;
			std::string fullname;
		};

		using index_t = std::size_t;
		using TypeMap = std::map<index_t, TypeName>;

		inline index_t& counter() {
			static index_t v = 0;
			return v;
		}

		template <typename T>
		inline index_t IndexOf() {
			static index_t value = counter()++;
			return value;
		}

		const TypeMap& GetTypeMap();
		
		template <typename T>
		const std::string& GetTypeName() {
			const TypeMap& type_map = GetTypeMap();
			auto it = type_map.find(IndexOf<std::decay_t<T>>());
			if (it == type_map.end())
				return UNKNOWN_NAME;

			return it->second.name;
		}

		template <typename T>
		const std::string& GetTypeFullname() {
			const TypeMap& type_map = GetTypeMap();
			auto it = type_map.find(IndexOf<std::decay_t<T>>());
			if (it == type_map.end())
				return UNKNOWN_FULLNAME;

			return it->second.fullname;
		}

		template <typename T>
		bool IsTypeNameCompatible(const std::string& name) {
			const std::string& n = GetTypeName<std::decay_t<T>>();
			if (n != UNKNOWN_NAME)
				return n == name;

			return true;
		}

		template <typename T>
		bool IsTypeFullnameCompatible(const std::string& fullname) {
			const std::string& n = GetTypeFullname<std::decay_t<T>>();
			if (n != UNKNOWN_FULLNAME)
				return n == fullname;

			return true;
		}
	}
}