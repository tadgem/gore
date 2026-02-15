//
// Created by Liam on 15/02/2026.
//

#ifndef GORE_GLU_H
#define GORE_GLU_H
#include "glad/glad.h"
#include "stl.h"
#include "maths.h"

namespace gore
{
	using gl_handle = unsigned int;

	static const gl_handle INVALID_GL_HANDLE = UINT32_MAX;

	enum class ShaderStage { kUnknown, kVertex, kFragment, kGeometry, kCompute };
	enum class UniformType {
		kUnknown,
		kInt,
		kFloat,
		kVec2,
		kVec3,
		kVec4,
		kMat3,
		kMat4,
		kSampler2D,
		kSampler3D,
		kImage2D,
		kImage3D
	  };

	static HashMap<ShaderStage, String>	split_composite_shader(const String &input);

	class GLShader {
	public:
		unsigned int linked_program_id;

		GLShader() = default;
		GLShader(const String &comp);
		GLShader(const String &vert, const String &frag);
		GLShader(const String &vert, const String &geom,
				  const String &frag);

		void use();
		void release();

		void set_bool(const String &name, bool value) const;
		void set_int(const String &name, int value) const;
		void set_uint(const String &name, unsigned int value) const;
		void set_float(const String &name, float value) const;
		void set_vec2f(const String &name, vec2 value) const;
		void set_vec3f(const String &name, vec3 value) const;
		void set_vec4f(const String &name, vec4 value) const;
		void set_vec2i(const String &name, ivec2 value) const;
		void set_vec3i(const String &name, ivec3 value) const;
		void set_vec4i(const String &name, ivec4 value) const;
		void set_mat3f(const String &name, mat3 value) const;
		void set_mat4f(const String &name, mat4 value) const;

		static gl_handle compile_shader(const String &source,
										GLenum shader_stage);
		static int link_shader(gl_handle comp);
		static int link_shader(gl_handle vert, gl_handle frag);
		static int link_shader(gl_handle vert, gl_handle geom, gl_handle frag);


		static GLShader create_from_composite(const String &composite_shader);

		static UniformType get_uniform_type_from_gl(GLenum type);

	};
	
	class GLFramebuffer {
	public:
		GLFramebuffer();

		void bind();
		void unbind();
		void cleanup();

		void add_colour_attachment(GLenum attachment_index, uint32_t width,
								   uint32_t height, GLenum internal_format,
								   GLenum format, GLenum filter = GL_LINEAR,
								   GLenum pixel_format = GL_UNSIGNED_BYTE);

		void add_depth_attachment(uint32_t width, uint32_t height,
								  GLenum format = GL_DEPTH24_STENCIL8);

		void
		add_depth_attachment_sampled(uint32_t width, uint32_t height,
											  GLenum format = GL_DEPTH24_STENCIL8);
		void check();

		template <typename _Ty, size_t w, size_t h>
		Array<_Ty, w * h> read_pixels(u32 x, u32 y, u32 attachment_index,
										   GLenum pixel_format, GLenum pixel_type) {
			Array<_Ty, w * h> out_data{};
			bind();
			glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
			glReadPixels(x, y, w, h, pixel_format, pixel_type, &out_data[0]);
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			unbind();
			return out_data;
		}

		struct GLAttachmentInfo {
			GLenum internal_format;
			GLenum format;
			GLenum filter;
			GLenum pixel_format;
		};

		static GLFramebuffer create(vec2 resolution,
									 Vector<GLAttachmentInfo> colour_attachments,
									 bool add_depth = true);

		gl_handle handle;
		Vector<gl_handle> colour_attachments;
		gl_handle depth_attachment;
		uint32_t framebuffer_width, framebuffer_height;
	};

	class GLVAO {
	public:
		gl_handle			vao;
		gl_handle			ibo;
		uint32_t			index_count;
		Vector<gl_handle>	vbos;

		GLVAO(gl_handle vao, gl_handle ibo, uint32_t index_count, Vector<gl_handle> vbos);

		void use();
		void draw();
		void release();
	};

	class GLMesh {
	public:

		uint32_t	index_count;
		AABB3		model_space_aabb;
		AABB3		world_space_aabb;
		uint32_t	material_index;
	};

	class GLVAOBuilder {
	public:
		GLVAOBuilder() = default;

		void Begin();

		template <typename _Ty>
		void add_vertex_buffer(_Ty *data, uint32_t count,
							   GLenum usage_flags = GL_STATIC_DRAW) {
			gl_handle vbo;
			glGenBuffers(1, &vbo);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			auto data_size = sizeof(_Ty) * count;
			glBufferData(GL_ARRAY_BUFFER, data_size, data, usage_flags);
			vbos.push_back(vbo);

		}

		template <typename _Ty>
		void add_vertex_buffer(Vector<_Ty> data,
							   GLenum usage_flags = GL_STATIC_DRAW) {
			add_vertex_buffer<_Ty>(&data[0], data.size(), usage_flags);
		}

		void add_index_buffer(uint32_t *data, uint32_t data_count);
		void add_index_buffer(Vector<uint32_t> data);

		void add_vertex_attribute(uint32_t binding, uint32_t total_vertex_size,
								  uint32_t num_elements, uint32_t element_size = 4,
								  GLenum primitive_type = GL_FLOAT);

		GLVAO build_vao();

		gl_handle vao_handle = INVALID_GL_HANDLE;
		gl_handle ibo_handle = INVALID_GL_HANDLE;
		uint32_t index_count = 0;
		Vector<gl_handle> vbos;
	private:
		uint32_t offset_counter_;
	};
}


#endif //GORE_GLU_H