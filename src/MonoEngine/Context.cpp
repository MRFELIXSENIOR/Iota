#include "Context.hpp"
#include "../IotaException.hpp"

using namespace iota;
using namespace Mono;

#define GET_PATH_CSTR(path) (path.string().c_str())

Context::Context(std::filesystem::path path_to_assembly) : assembly_path(path_to_assembly) {
	assembly = mono_domain_assembly_open(mono_get_root_domain(), GET_PATH_CSTR(assembly_path));
	if (!assembly) {
		throw RuntimeError("Failed To Open Assembly! File: " + assembly_path.string());
	}

	img = mono_assembly_get_image(assembly);
	if (!img) {
		throw RuntimeError("Failed To Get Assembly Image! File: " + assembly_path.string());
	}
}

Context::Context(MonoAssembly* assem): assembly(assem) {
	if (!assembly) {
		throw RuntimeError("Cannot Create Mono Context with NULL MonoAssembly*");
	}

	img = mono_assembly_get_image(assembly);
	if (!img) {
		throw RuntimeError("Failed To Get Assembly Image! File: " + std::string(mono_image_get_filename(img)));
	}

	assembly_path = std::filesystem::path(mono_image_get_filename(img));
}

Context::Context(MonoImage* image): img(image) {
	if (!img) {
		throw RuntimeError("Cannot Create Mono Context with NULL Assembly Image");
	}

	assembly = mono_image_get_assembly(image);
	if (!assembly) {
		throw RuntimeError("Failed To Get Assembly, file: " + std::string(mono_image_get_filename(img)));
	}

	assembly_path = std::filesystem::path(mono_image_get_filename(img));
}

Context::~Context() {
	mono_image_close(img);
	mono_assembly_close(assembly);
}

bool Context::valid() {
	return img && assembly;
}