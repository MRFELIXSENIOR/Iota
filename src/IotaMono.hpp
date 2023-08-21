#pragma once

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
		struct Context final {
		public:
			MonoAssembly* assembly = nullptr;
			MonoImage* img = nullptr;
			std::filesystem::path assembly_path;

			//explicit Context(const std::string& cs_file);
			explicit Context(std::filesystem::path path_to_assembly);
			explicit Context(MonoAssembly* assembly);
			explicit Context(MonoImage* image);
			~Context();

			bool valid();
		};

		struct Function;
		struct Object;

		struct Class final {
		private:
			MonoClass* self = nullptr;
			MonoType* mono_type = nullptr;

			std::vector<Function> fn;
			Context context;

			std::string name;
			std::string fullname;
			std::string _namespace;
			bool value_type;

			void metadata_init();

			friend struct Type;
			friend struct Object;
		public:
			explicit Class(MonoClass* mono_class);
			explicit Class(MonoType* mono_type);

			explicit Class(const std::string& name_space, const std::string& class_name, Context& ctx);
			explicit Class(const std::string& class_name, Context& ctx);
			~Class();

			Object& CreateInstance();

			MonoClass* get_mono_ptr() const;
			MonoType* get_mono_type_ptr() const;
			bool valid() const;

			const std::string& get_namespace() const;
			const std::string& get_fullname() const;
			const std::string& get_name() const;

			Function* GetFunctionRef(const std::string& name, int arg_count);
		};

		struct Function {
		private:
			std::optional<Class> class_ref;

			MonoMethod* self = nullptr;
			MonoMethodSignature* sig = nullptr;
			MonoMethodDesc* desc = nullptr;

			std::string name;
			int arg_count;

			friend struct Class;
		public:
			Function(MonoMethod* mono_method);
			Function(Class& klass, const std::string& fn_name, int argc);
			Function(const std::string& fn_name, int argc, Context& ctx); //global function
			~Function();

			bool valid() const;
			MonoMethod* get_mono_ptr() const;
			MonoMethodDesc* get_mono_desc_ptr() const;
			MonoMethodSignature* get_mono_sig_ptr() const;
			const std::string& get_name() const;

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

			bool valid();
			MonoObject* get_mono_ptr() const;
		};
	}
};