#include "model.h"
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"
#include "mimalloc.h"

// typedef struct cgltf_memory_options
// {
// 	void* (*alloc_func)(void* user, cgltf_size size);
// 	void (*free_func) (void* user, void* ptr);
// 	void* user_data;
// } cgltf_memory_options;

void* model_mimalloc_alloc(void* user, cgltf_size size)
{
	return mi_malloc(size);
}

void model_mimalloc_free(void* user, void* ptr)
{
	mi_free(ptr);
}
gore::Model gore::Model::create_from_gltf(const char *filename)
{
	cgltf_memory_options mem = {};
	mem.alloc_func = &model_mimalloc_alloc;
	mem.free_func = &model_mimalloc_free;
	mem.user_data = nullptr;
	cgltf_options options = {};
	options.memory = mem;
	cgltf_data* data = nullptr;
	cgltf_result result = cgltf_parse_file(&options, "scene.gltf", &data);
	if (result == cgltf_result_success)
	{
		/* TODO make awesome stuff */
		cgltf_free(data);
	}

	return {};
}
