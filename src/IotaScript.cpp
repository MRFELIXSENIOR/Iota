#include "IotaScript.hpp"

#include <vector>

using namespace iota;

static std::vector<std::filesystem::path> script_vec;

Script::Script(const std::filesystem::path& sp) : script_path(sp), attached_inst(nullptr) {
	script_vec.push_back(sp);
}

const std::filesystem::path& Script::GetPath() const { return script_path; }

const std::vector<std::filesystem::path>& iota::GetScripts() { return script_vec; }