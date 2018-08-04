#include "program.h"
#include "log.h"

auto program::use(void) -> void
{
	glUseProgram(program_id);
}
auto program::create_shader(GLenum type, std::string const & dir) -> void
{
	shaders.emplace_back(type);
#ifdef _WIN32
	shaders.back().compile(std::string("res/") + dir);
#else
	shaders.back().compile("../res/" + dir);
#endif
}
auto program::status(void) -> bool
{
	int32_t status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		//std::cerr << "ERROR : failed to create program" << std::endl;
		logger::error_log("failed to create program");

		int32_t info_log_length = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
		char * buffer = (char *)alloca(info_log_length * sizeof(char));
		int32_t buffer_size;
		glGetProgramInfoLog(program_id, info_log_length * sizeof(char), &buffer_size, buffer);

		logger::new_log("program link error info");
		std::cout << buffer << std::endl;
		return false;
	}
	return true;
}
auto program::attach_shaders(void) -> void
{
	for (auto & shader : shaders) glAttachShader(program_id, shader.id());
}
auto program::delete_shaders(void) -> void
{
	for (auto & shader : shaders) shader.delete_shader();
}

auto program::uniform_1f(float p, uint32_t index) -> void
{
	glUniform1f(uniform_locations[index], p);
}
auto program::uniform_2f(float* p, uint32_t index) -> void
{
	glUniform2fv(uniform_locations[index], 1, p);
}
auto program::uniform_3f(float* p, uint32_t index) -> void
{
	glUniform3fv(uniform_locations[index], 1, p);
}
auto program::uniform_mat4(float* p, uint32_t index) -> void
{
	glUniformMatrix4fv(uniform_locations[index], 1, GL_FALSE, p);
}
auto program::uniform_1i(int32_t i, uint32_t index) -> void
{
	glUniform1i(uniform_locations[index], i);
}
auto program::uniform_4f(float * p, uint32_t index) -> void
{
	glUniform4fv(uniform_locations[index], 1, p);
}