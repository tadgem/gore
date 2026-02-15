#include "glu.h"

#include <iostream>
#include <sstream>
// #include "SDL3/SDL_opengl.h"

namespace gore
{
	HashMap<ShaderStage, String>
	split_composite_shader (const String &input) {
		static HashMap<String, ShaderStage> s_known_stages = {
			{"#frag", ShaderStage::kFragment},
			{"#vert", ShaderStage::kVertex},
			{"#geom", ShaderStage::kGeometry},
			{"#compute", ShaderStage::kCompute}};

		auto stages = HashMap<ShaderStage, String>();
		String version = "";

		std::stringstream input_stream(input.c_str());
		std::stringstream stage_stream{};
		String line = "";
		String stage = "";

		while (std::getline(input_stream, line)) {
			if (line.find("#version") != String::npos) {
				version = line;
				stage_stream << version << "\n";
				continue;
			}

			for (auto &[known, stage_enum] : s_known_stages) {
				if (line.find(known) != String::npos) {
					if (!stage.empty()) {
						stages.emplace(ShaderStage(s_known_stages[stage]),
									   String(stage_stream.str()));
						stage_stream.str(std::string());
						stage_stream.clear();
						stage_stream << version << "\n";
					}

					stage = known;
					break;
				}
			}

			if (line == stage) {
				continue;
			}

			stage_stream << line << "\n";
		}

		String last_stream (stage_stream.str());
		if (!stage.empty() && !last_stream.empty()) {
			stages.emplace(s_known_stages[stage], last_stream);
		}

		return stages;
	}



  GLShader::GLShader(const String &comp) {
    
    auto c = compile_shader(comp, GL_COMPUTE_SHADER);
    linked_program_id = link_shader(c);

    glDeleteShader(c);
  }

  GLShader::GLShader(const String &vert, const String &frag) {
    
    auto v = compile_shader(vert, GL_VERTEX_SHADER);
    auto f = compile_shader(frag, GL_FRAGMENT_SHADER);

    linked_program_id = link_shader(v, f);

    glDeleteShader(v);
    glDeleteShader(f);
  }

  GLShader::GLShader(const String &vert, const String &geom,
                       const String &frag) {
    
    auto v = compile_shader(vert, GL_VERTEX_SHADER);
    auto g = compile_shader(geom, GL_GEOMETRY_SHADER);
    auto f = compile_shader(frag, GL_FRAGMENT_SHADER);

    linked_program_id = link_shader(v, g, f);

    glDeleteShader(v);
    glDeleteShader(g);
    glDeleteShader(f);
  }

  void GLShader::use() {
    
    glUseProgram(linked_program_id);
  }

  void GLShader::release() {
    
    glDeleteProgram(linked_program_id);
  }

  void GLShader::set_bool(const String &name, bool value) const {
    
    glUniform1i(glGetUniformLocation(linked_program_id, name.c_str()), (int)value);
  }

  void GLShader::set_int(const String &name, int value) const {
    
    glUniform1i(glGetUniformLocation(linked_program_id, name.c_str()), value);
  }

  void GLShader::set_uint(const String &name, unsigned int value) const {
    
    glUniform1ui(glGetUniformLocation(linked_program_id, name.c_str()), value);
  }

  void GLShader::set_float(const String &name, float value) const {
    
    glUniform1f(glGetUniformLocation(linked_program_id, name.c_str()), value);
  }

  void GLShader::set_vec2f(const String &name, vec2 value) const {
    
    glUniform2f(glGetUniformLocation(linked_program_id, name.c_str()), value.x,
                value.y);
  }

  void GLShader::set_vec3f(const String &name, vec3 value) const {
    
    glUniform3f(glGetUniformLocation(linked_program_id, name.c_str()), value.x, value.y,
                value.z);
  }

  void GLShader::set_vec4f(const String &name, vec4 value) const {
    
    glUniform4f(glGetUniformLocation(linked_program_id, name.c_str()), value.x, value.y,
                value.z, value.w);
  }

  void GLShader::set_vec2i(const String &name, ivec2 value) const {
    
    glUniform2i(glGetUniformLocation(linked_program_id, name.c_str()), value.x,
                value.y);
  }

  void GLShader::set_vec3i(const String &name, ivec3 value) const {
    
    glUniform3i(glGetUniformLocation(linked_program_id, name.c_str()), value.x, value.y,
                value.z);
  }

  void GLShader::set_vec4i(const String &name, ivec4 value) const {
    
    glUniform4i(glGetUniformLocation(linked_program_id, name.c_str()), value.x, value.y,
                value.z, value.w);
  }

  void GLShader::set_mat3f(const String &name, mat3 value) const {
    
    glUniformMatrix3fv(glGetUniformLocation(linked_program_id, name.c_str()), 1,
                       GL_FALSE, &value[0][0]);
  }

  void GLShader::set_mat4f(const String &name, mat4 value) const {
    
    glUniformMatrix4fv(glGetUniformLocation(linked_program_id, name.c_str()), 1,
                       GL_FALSE, &value[0][0]);
  }

  gl_handle GLShader::compile_shader(const String &source,
                                      GLenum shader_stage) {
    
    const char *src = source.c_str();
    gl_handle s = glCreateShader(shader_stage);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);

    int success = 0;

    glGetShaderiv(s, GL_COMPILE_STATUS, &success);

    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(s, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    return s;
  }

  int GLShader::link_shader(gl_handle comp) {
    
    auto prog_id = glCreateProgram();
    glAttachShader(prog_id, comp);
    glLinkProgram(prog_id);

    int success = 0;

    glGetProgramiv(prog_id, GL_LINK_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetProgramInfoLog(prog_id, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
    }

    return prog_id;
  }

  int GLShader::link_shader(gl_handle vert, gl_handle frag) {
    
    auto prog_id = glCreateProgram();
    glAttachShader(prog_id, vert);
    glAttachShader(prog_id, frag);
    glLinkProgram(prog_id);

    int success = 0;

    glGetProgramiv(prog_id, GL_LINK_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetProgramInfoLog(prog_id, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
    }

    return prog_id;
  }

  int GLShader::link_shader(gl_handle vert, gl_handle geom, gl_handle frag) {
    
    auto prog_id = glCreateProgram();
    glAttachShader(prog_id, vert);
    glAttachShader(prog_id, geom);
    glAttachShader(prog_id, frag);
    glLinkProgram(prog_id);

    int success = 0;

    glGetProgramiv(prog_id, GL_LINK_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetProgramInfoLog(prog_id, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
    }

    return prog_id;
  }

  GLShader GLShader::create_from_composite(const String &composite_shader) {
    HashMap<ShaderStage, String> stages =
        split_composite_shader(composite_shader);

    if (stages.find(ShaderStage::kCompute) != stages.end()) {
      return GLShader(stages[ShaderStage::kCompute]);
    }

    if (stages.find(ShaderStage::kVertex) != stages.end() &&
        stages.find(ShaderStage::kFragment) != stages.end() &&
        stages.find(ShaderStage::kGeometry) == stages.end()) {
      return GLShader(stages[ShaderStage::kVertex],
                       stages[ShaderStage::kFragment]);
    }

    if (stages.find(ShaderStage::kVertex) != stages.end() &&
        stages.find(ShaderStage::kFragment) != stages.end() &&
        stages.find(ShaderStage::kGeometry) != stages.end()) {
      return GLShader(stages[ShaderStage::kVertex],
                       stages[ShaderStage::kGeometry],
                       stages[ShaderStage::kFragment]);
    }
    return {};
  }

  UniformType GLShader::get_uniform_type_from_gl(GLenum type) {
    
    switch (type) {
    case GL_SAMPLER_2D:
      return UniformType::kSampler2D;
    case GL_SAMPLER_3D:
      return UniformType::kSampler3D;
    case GL_IMAGE_2D:
      return UniformType::kImage2D;
    case GL_IMAGE_3D:
      return UniformType::kImage3D;
    case GL_INT:
      return UniformType::kInt;
    case GL_FLOAT:
      return UniformType::kFloat;
    case GL_FLOAT_VEC2:
      return UniformType::kVec2;
    case GL_FLOAT_VEC3:
      return UniformType::kVec3;
    case GL_FLOAT_VEC4:
      return UniformType::kVec4;
    case GL_FLOAT_MAT3:
      return UniformType::kMat3;
    case GL_FLOAT_MAT4:
      return UniformType::kMat4;
    default:
      return UniformType::kUnknown;
    }

    return UniformType::kUnknown;
  }
  
  
GLFramebuffer::GLFramebuffer() {
  handle = INVALID_GL_HANDLE;
}

void GLFramebuffer::unbind() {
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::cleanup() {
  
  glDeleteFramebuffers(1, &handle);
}

void GLFramebuffer::add_colour_attachment(GLenum attachment_index,
                                           uint32_t width, uint32_t height,
                                           GLenum internal_format,
                                           GLenum format, GLenum filter,
                                           GLenum pixel_format) {
  
  gl_handle textureColorbuffer;
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, internal_format,
               pixel_format, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_index, GL_TEXTURE_2D,
                         textureColorbuffer, 0);
  colour_attachments.push_back(textureColorbuffer);
  framebuffer_width = width;
  framebuffer_height = height;
}

void GLFramebuffer::add_depth_attachment(uint32_t width, uint32_t height,
                                          GLenum format) {
  
  gl_handle rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  depth_attachment = rbo;
  framebuffer_width = width;
  framebuffer_height = height;
}

void GLFramebuffer::add_depth_attachment_sampled(uint32_t width,
                                                           uint32_t height,
                                                           GLenum format) {
  
  gl_handle depthMap;
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depthMap, 0);
  glDrawBuffers(0, GL_NONE);
  glReadBuffer(GL_NONE);

  depth_attachment = depthMap;
  framebuffer_width = width;
  framebuffer_height = height;
}

void GLFramebuffer::check() {
  
  auto zero = GL_COLOR_ATTACHMENT0;
  Vector<unsigned int> attachments;

  for (auto &a : colour_attachments) {
    attachments.push_back(zero);
    zero++;
  }

  if (attachments.empty()) {
    glDrawBuffers(GL_NONE, nullptr);
    glReadBuffer(GL_NONE);
  } else {
    glDrawBuffers(attachments.size(), attachments.data());
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
              << std::endl;
  }
}

GLFramebuffer GLFramebuffer::create(vec2 resolution,
                                      Vector<GLAttachmentInfo> attachments,
                                      bool add_depth) {
  
  GLenum attachment = GL_COLOR_ATTACHMENT0;
  GLFramebuffer fb{};
  fb.bind();
  for (auto &info : attachments) {
    fb.add_colour_attachment(attachment, resolution.x, resolution.y,
                             info.internal_format, info.format,
                             info.filter, info.pixel_format);
    attachment++;
  }
  if (add_depth) {
    fb.add_depth_attachment_sampled(resolution.x, resolution.y,
                                             GL_DEPTH24_STENCIL8);
  }
  fb.check();
  fb.unbind();
  return fb;
}

GLVAO::GLVAO(gl_handle vao, gl_handle ibo, uint32_t index_count, Vector<gl_handle> vbos)
	: vao(vao), ibo(ibo), index_count(index_count), vbos(std::move(vbos))
{
}

void GLVAO::use()
{
	  glBindVertexArray(vao);
}

void GLVAO::draw()
{
	  use();
	  if (ibo != INVALID_GL_HANDLE) {
	    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count),
                     GL_UNSIGNED_INT, 0);
	  } else {
	    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(index_count));
	  }
}

void GLVAO::release()
{
	  if (ibo > 0) {
	    glDeleteBuffers(1, &ibo);
	  }
	  if (!vbos.empty()) {
	    glDeleteBuffers(vbos.size(), vbos.data());
	  }
	  glDeleteVertexArrays(1, &vao);
}

void GLVAOBuilder::add_index_buffer(uint32_t *data, uint32_t data_count)
{
	  gl_handle ibo;
	  glGenBuffers(1, &ibo);

	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * data_count, data,
                 GL_STATIC_DRAW);
	  ibo_handle = ibo;
	  index_count = data_count;
}

void GLVAOBuilder::add_index_buffer(Vector<uint32_t> data)
{
	  add_index_buffer(data.data(), data.size());
}

void GLVAOBuilder::add_vertex_attribute(uint32_t binding, uint32_t total_vertex_size, uint32_t num_elements,
  uint32_t element_size, GLenum primitive_type)
{
	  glVertexAttribPointer(binding, num_elements, primitive_type, GL_FALSE,
                        total_vertex_size, (void *)offset_counter_);
	  glEnableVertexAttribArray(binding);
	  // glBindBuffer(GL_ARRAY_BUFFER, m_vbos.back());
	  offset_counter_ += num_elements * element_size;
}

GLVAO GLVAOBuilder::build_vao()
{
	return  GLVAO(vao_handle, ibo_handle, index_count, vbos);
}

void GLFramebuffer::bind() {
  
  if (handle == INVALID_GL_HANDLE) {
    glGenFramebuffers(1, &handle);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, handle);
}
}