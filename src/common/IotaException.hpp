#pragma once

namespace IotaEngine {
	namespace Application {
		enum class RuntimeException {
			WINDOW_CREATION_FAILURE,
			RENDERER_CREATION_FAILURE,
			INITIALIZATION_FAILURE,
			NO_INIT_ERROR,
		};

		enum class Exception {
			LUA_SCRIPT_ERROR,
			FIND_CHILDREN_FAILURE,
			GET_PROPERTY_FAILURE,
			ADD_PROPERTY_FAILURE,
			FIND_PROPERTY_FAILURE,
			LOAD_PATH_FAILURE,
			CREATE_TEXTURE_FAILURE,
		};
	};
};
