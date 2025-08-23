#include <Shader/Shader.hpp>
#include <Log/Log.hpp>
#include <fstream>

smpl::Shader::Shader()
    : is_loaded{ false }
    , shader_id{ 0 }
    , shader_type{ 0 }
{
    //Do nothing
}

bool smpl::Shader::loadFromFile(const std::string& filename, GLenum shaderType)
{
    std::vector<char> shader;
    if (!getFileContents(filename, shader))
    {
        LOG_ERROR("Failed to open shader file \"{}\".", filename );
        return false;
    }

    shader_id = glCreateShader(shaderType);

    const char* src = shader.data();
    glShaderSource(shader_id, 1, &src, nullptr);

    glCompileShader(shader_id);

    GLint compileStatus;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE)
    {
        GLint logLength;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<char> log(logLength);
        glGetShaderInfoLog(shader_id, logLength, nullptr, log.data());

        LOG_ERROR("Shader compilation failed for '{}':\n{}", filename, std::string(log.data()));

        glDeleteShader(shader_id);
        return false;
    }

    is_loaded = true;
    return true;
}

bool smpl::Shader::loadFromMemory(const std::string& name, GLenum shader_type)
{
	return false;
}

bool smpl::Shader::isLoaded() const
{
    return is_loaded;
}

void smpl::Shader::deleteShader()
{
}

GLuint smpl::Shader::getShaderID() const
{
    return shader_id;
}

GLenum smpl::Shader::getShaderType() const
{
    return GLenum();
}


bool smpl::Shader::getFileContents(const std::string& filename, std::vector<char>& buffer)
{
    std::ifstream file(filename.c_str(), std::ios_base::binary);
    if (file)
    {
        file.seekg(0, std::ios_base::end);
        std::ifstream::pos_type size = file.tellg();
        if (size > 0)
        {
            file.seekg(0, std::ios_base::beg);
            buffer.resize(static_cast<std::size_t>(size));
            file.read(&buffer[0], static_cast<std::streamsize>(size));
        }
        buffer.push_back('\0');
        return true;
    }
    else
    {
        return false;
    }
}
