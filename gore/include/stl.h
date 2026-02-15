//
// Created by Liam on 15/02/2026.
//

#ifndef GORE_STL_H
#define GORE_STL_H

#undef MI_DEBUG
#include "mimalloc.h"
#include <algorithm>
#include <filesystem>
#include <future>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

#define GORE_OVERRIDE_GLOBAL_NEW(ENABLE_PRINT)                              \
void *__cdecl operator new[](size_t size, const char *name, int flags,       \
unsigned debugFlags, const char *file,          \
int line) {                                     \
ENABLE_PRINT ? printf("Unassigned Allocation!\n") : 0;                     \
HNY_ASSERT_NOT_REACHED();                                                  \
return mi_malloc(size);                                                    \
}                                                                            \
\
void *__cdecl operator new[](size_t size, unsigned __int64,                  \
unsigned __int64, char const *, int,            \
unsigned int, char const *, int) {              \
ENABLE_PRINT ? printf("Unassigned Allocation!\n") : 0;                     \
HNY_ASSERT_NOT_REACHED();                                                  \
return mi_malloc(size);                                                    \
}

namespace gore {

	template <typename T> class mimalloc_default_delete {
	public:
		void operator()(T *p) const { mi_free((void *)p); }
	};


	using String =
		std::basic_string<char, std::char_traits<char>, mi_stl_allocator<char>>;

	template <typename _Ty, typename Allocator = mi_stl_allocator<_Ty>>
	using Vector = std::vector<_Ty, Allocator>;

	template <typename _Ty, size_t _Size>
	using Array = std::array<_Ty, _Size>;

	template <typename _Ty1, typename _Ty2> using Pair = std::pair<_Ty1, _Ty2>;

	template <typename _Ty>
	using Unique = std::unique_ptr<_Ty, mimalloc_default_delete<_Ty>>;

	template <typename _Key, typename _Value>
	using HashMap =
		std::unordered_map<_Key, _Value, std::hash<_Key>, std::equal_to<_Key>,
						   mi_stl_allocator<std::pair<const _Key, _Value>>>;

	template <typename _Ty, typename... Args>
	Unique<_Ty> make_unique_ptr(Args &&...args) {
		void *memory_loc = mi_malloc(sizeof(_Ty));
		Unique<_Ty> ptr =
			Unique<_Ty>(new (memory_loc) _Ty(std::forward<Args>(args)...));
		return ptr;
	}

	template <typename _Ty> using Future = std::future<_Ty>;

	// template utils
	template <typename _Ty> static bool is_future_ready(Future<_Ty> const &o) {
		return o.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}

	namespace Filesystem = std::filesystem;
	namespace stl = std;

} // namespace harmony
#endif //GORE_STL_H