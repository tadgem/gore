//
// Created by Liam on 15/02/2026.
//

#ifndef GORE_MODEL_H
#define GORE_MODEL_H

#include "glu.h"

namespace gore
{
	class Model {
	public:

		static Model create_from_gltf(const char * filename);

	};
}

#endif //GORE_MODEL_H