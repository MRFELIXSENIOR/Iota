#pragma once

#include "IotaGameInstance.hpp"

#include <concepts>
#include <vector>
#include <filesystem>
#include <optional>
#include <unordered_map>
#include <string>
#include <stdexcept>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace iota {
	namespace Mono {
		class Error : std::runtime_error {
			using runtime_error::runtime_error;
		};

		struct Context {
		public:
			MonoAssembly* assembly = nullptr;
			MonoImage* img = nullptr;
			std::filesystem::path assembly_path;

			explicit Context(const std::string& cs_file);
			explicit Context(MonoAssembly* assembly);
			explicit Context(MonoImage* img);

			bool IsValid();
		};

		struct Domain final {
		private:
			mutable std::unordered_map<std::string, Context> contexts;
			MonoDomain* dom;

		public:
			explicit Domain(const std::string& name);
			~Domain();

			std::optional<Context> GetContext(const std::string& path) const;
			MonoDomain* get_ptr();
		};

		struct Function;

		struct Object;
		struct Class final {
		private:
			MonoClass* self = nullptr;
			std::vector<Function> fn;
			Context* context;
			std::string name;
			std::string fullname;
			std::string _namespace;

			friend struct Type;
			friend struct Object;
		public:
			explicit Class(MonoClass* klass);
			explicit Class(MonoType* type);

			Class(const std::string& class_name, Context& ctx);
			Class(const std::string& name_space, const std::string& class_name, Context& ctx);


			Object& CreateInstance();

			~Class();

			MonoClass* get_ptr() const;

			bool IsValid() const;

			std::string GetNamespace() const;
			std::string GetName() const;

			Function* GetFunctionRef(const std::string& name, int arg_count);
		};

		struct Type {
		private:
			MonoType* self = nullptr;
			Class class_ref;

			std::string name;
			friend struct Class;
		public:
			Type(MonoType* mono_type);
			Type(MonoClass* mono_class);
			Type(const std::string& type_name, Context& ctx);
		};

		class Function {
		private:
			Class* cl_ref = nullptr;

			MonoMethod* self = nullptr;
			MonoMethodDesc* desc = nullptr;

			std::string name;
			int arg_count;

			friend struct Class;
		public:
			Function(Class& klass, const std::string& fn_name, int argc);
			Function(const std::string& fn_name, int argc, Context& ctx);
			~Function();

			bool IsValid() const;
			MonoMethod* get_ptr() const;
			std::string GetName() const;

			template <typename... Args>
			void Invoke(Args... args) {

			}

			template <typename... Args>
			void operator()(Args... args) {
				Invoke(args...);
			}
		};

		struct Object {
		private:
			MonoObject* self;
			Class class_ref;

		public:
			Object(MonoObject* mono_obj);
			Object(MonoClass* mono_class);
			Object(Class& klass);
			~Object();

			bool IsValid();

			MonoObject* get_ptr() const;
		};

		struct Script : public GameInstance::Instance {
		private:
			Context context;
			std::string attached_iname;
			bool attached;
			std::filesystem::path script_path;
			std::string class_name;

			Function* Load;
			friend class GameInstance::Instance;

		public:
			Script(const std::string& file_name);
			~Script();

			template <GameInstance::IsInstance T>
			void Attach(T& instance) {
				attached = true;
				attached_iname = instance.name.data();
				instance.attached_scripts.push_back(this);
			}

			void InvokeLoad();
		};

		void Initialize(std::vector<std::string>& file_names);
		void Clean();
	}
};