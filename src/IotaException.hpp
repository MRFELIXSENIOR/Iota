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
			CANNOT_LOG,
			CANNOT_FIND_CHILDREN,
			LOAD_PATH_FAILURE,
			CREATE_TEXTURE_FAILURE,
		};
	};
};
