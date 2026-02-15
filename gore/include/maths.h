//
// Created by Liam on 15/02/2026.
//

#ifndef GORE_MATHS_H
#define GORE_MATHS_H
#include "glm/glm.hpp"
namespace gore
{
	using namespace glm;

	struct AABB3 {
		vec3 min;
		vec3 max;
	};

	namespace maths = glm;
}
#endif //GORE_MATHS_H