#include "JIT.hpp"
#include "InternalCall.hpp"
#include "TypeClass.hpp"
#include "Object.hpp"
#include "Context.hpp"
#include "FunctionInvoker.hpp"
#include "Function.hpp"

#include <unordered_map>
#include <cstdlib>
#include <vector>
#include <format>
#include <iostream>

using namespace iota;
using namespace Mono;
using namespace JIT;

static MonoDomain* jit_domain;

static void IotaLogCallback(const char* log_domain, const char* log_level, const char* message, mono_bool fatal, void* userdata) {
	std::string dom = (log_domain != nullptr) ? log_domain : "HIDDEN";
	std::cout << std::format("[{}] [{}] {}", dom, log_level, message) << '\n';
}

static std::vector<std::string> references {
	"Iota.Application.dll",
	"Iota.Exceptions.dll"
};

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

int compile(std::vector<std::filesystem::path> files, bool debug) {
	std::string cmd = "csc";
	for (auto& path : files) {
		cmd += " ";
		cmd += quote_wrap(path.string());
	}

	cmd += " -t:library -lib:lib/mono -reference:";
	for (auto& ref : references) {
		cmd += ref;
		cmd += ",";
	}
	cmd.pop_back();

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

Result Mono::RunScript(const std::vector<std::filesystem::path>& scripts_path, const ExceptionHandler& exc_callback) {
	compile(scripts_path, false);
	Context context("Iota-ScriptAssembly.dll");

	std::vector<RunResult> script_result;
	for (auto& path : scripts_path) {
		TypeClass klass("", path.filename().replace_extension("").string().c_str(), context);
		Object obj = klass.CreateInstance();

		Function method = klass.GetFunction("Load", 0);
		Invoker<void()> load_invoker(method);
		load_invoker(obj, exc_callback);

		if (!load_invoker.GetResult().success) {
			script_result.push_back({ path, false });
			continue;
		}

		script_result.push_back({ path, true });
	}

	return script_result;
}

 Result Mono::RunScript(const std::vector<std::filesystem::path>& scripts_path) {
	return RunScript(scripts_path, DefaultExceptionHandler);
}