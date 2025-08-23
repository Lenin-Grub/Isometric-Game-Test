#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>

namespace smpl
{
	class Shader
	{
	public:
		Shader();
		~Shader() = default;

		bool loadFromFile(const std::string& filename, GLenum shader_type);
		bool loadFromMemory(const std::string& name, GLenum shader_type);
		bool isLoaded() const;
		void deleteShader();

		GLuint getShaderID() const;
		GLenum getShaderType() const;

	private:
		bool getFileContents(const std::string& filename, std::vector<char>& buffer);

		GLuint shader_id;
		GLenum shader_type;
		bool   is_loaded;
	};
}
