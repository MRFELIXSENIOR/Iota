#include "IotaMonoJIT.hpp"
#include "IotaMono.hpp"

#include <unordered_map>
#include <cstdlib>
#include <format>
#include <iostream>

using namespace iota;
using namespace Mono;
using namespace JIT;

static MonoDomain* jit_domain;

static void IotaLogCallback(const char* log_domain, const char* log_level, const char* message, mono_bool fatal, void* userdata) {
	std::cout << std::format("[{}] [{}] {}", log_domain, log_level, message) << '\n';
}

void JIT::Initialize(bool enable_debug) {
	mono_set_dirs(nullptr, nullptr); //system installation
	mono_set_assemblies_path("lib/mono");

	if (enable_debug) {
		mono_trace_set_level_string("debug");
		mono_debug_init(MONO_DEBUG_FORMAT_MONO);
	}
	else {
		mono_trace_set_level_string("warning");
	}

	mono_trace_set_log_handler(IotaLogCallback, nullptr);

	jit_domain = mono_jit_init("IotaJITDomain");
	mono_thread_set_main(mono_thread_current());
}

void JIT::Clean() {
	mono_jit_cleanup(jit_domain);
}

inline std::string quote_wrap(const std::string& str) {
	return "\"" + str + "\"";
}

int compile(CompileOptions option, bool debug) {
	std::string cmd = "csc";
	for (auto& path : option.files) {
		cmd += " ";
		cmd += quote_wrap(path.string());
	}

	cmd += " -t:library";
	if (!option.references.empty()) {
		cmd += " -r:";
		for (auto& ref : option.references) {
			cmd += ref;
			cmd += ",";
		}
		cmd.pop_back();
	}

	if (debug) {
		cmd += " -debug";
	} else cmd += " -o";

	cmd += " -out:Iota-ScriptAssembly.dll";

#ifdef _WIN32
	cmd = quote_wrap(cmd);
#endif

	std::cout << cmd << '\n';
	return std::system(cmd.c_str());
}

void DefaultExceptionHandler(const Exception& exc) {
	std::cerr << "[Iota] [Mono Error] [" << exc.get_name() << "] " << exc.what() << '\n';
}

CompilationResult Mono::RunScript(const std::vector<std::filesystem::path>& scripts_path, const ExceptionHandler& exc_callback) {
	CompileOptions opt;
	opt.files = scripts_path;

	if (compile(opt, true) != 0)
		return CompilationResult::FAILED;

	MonoDomain* root = mono_get_root_domain();
	MonoAssembly* assembly = mono_domain_assembly_open(root, "Iota-ScriptAssembly.dll");
	MonoImage* image = mono_assembly_get_image(assembly);

	std::vector<MonoException*> exc;
	for (auto& path : scripts_path) {
		MonoClass* cl = mono_class_from_name(image, "", path.filename().replace_extension("").string().c_str());
		MonoObject* obj = mono_object_new(root, cl);
		mono_runtime_object_init(obj);

		MonoMethod* method = mono_class_get_method_from_name(cl, "Load", 0);
		if (!method)
			continue;

		MonoObject* exception;
		mono_runtime_invoke(method, obj, nullptr, &exception);
		if (exception) {
			exc_callback(Exception(exception));
			return CompilationResult::FAILED;
		}
	}
}

CompilationResult Mono::RunScript(const std::vector<std::filesystem::path>& scripts_path) {
	return RunScript(scripts_path, DefaultExceptionHandler);
}

void Mono::Initialize() {
	JIT::Initialize(true);
}

void Mono::Clean() {
	JIT::Clean();
}