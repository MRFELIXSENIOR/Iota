#pragma once

namespace IotaEngine {
	namespace Application {

		namespace RuntimeException {

			enum RuntimeException {
				WINDOW_CREATION_FAILURE = 1,
				RENDERER_CREATION_FAILURE = 2,
				INITIALIZATION_FAILURE = 3,
				NO_INIT_ERROR = 4,
			};

		};

		namespace Exception {

			enum Exception {
				LOAD_PATH_FAILURE = 1,
				CREATE_TEXTURE_FAILURE = 2,
			};

		};

	};
};
