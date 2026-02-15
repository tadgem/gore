//
// Created by Liam on 15/02/2026.
//

#ifndef GORE_JSON_H
#define GORE_JSON_H

#include "json.hpp"

namespace gore
{
	template <class T> struct STLMimallocAllocator {
		typedef T value_type;

		STLMimallocAllocator() {} // default ctor not required by C++ Standard Library

		// A converting copy constructor:
		template <class U> STLMimallocAllocator(const STLMimallocAllocator<U> &o) {}
		template <class U> bool operator==(const STLMimallocAllocator<U> &) const {
			return true;
		}
		template <class U> bool operator!=(const STLMimallocAllocator<U> &) const {
			return false;
		}
		T *allocate(const size_t n) const {
			return static_cast<T *>(mi_malloc(sizeof(T)));
		}
		void deallocate(T *const p, size_t) const { mi_free((void *)p); }
	};


	using json =
	nlohmann::basic_json<std::map, std::vector, String, bool, int64, uint64,
						 f64, STLMimallocAllocator, nlohmann::adl_serializer,
						 std::vector<uint8>, void>;
}
#endif //GORE_JSON_H