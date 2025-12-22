#include <Graphics/Shader/Shader.hpp>

#include <Log/Log.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace
{
    constexpr GLenum getType(smpl::Shader::Type type)
    {
        switch (type)
        {
        case smpl::Shader::Type::Vertex:
            return  GL_VERTEX_SHADER;
        case smpl::Shader::Type::Fragment:
            return GL_FRAGMENT_SHADER;
        default:
            LOG_ERROR("Unknown shader type.");
            return -1;
        }
    }
}

namespace smpl
{

    Shader::Shader()
        : is_loaded{ false }
        , is_compiled{ false }
        , shader_id{ 0 }
        , shader_type{ 0 }
    {
        //Do nothing
    }

    bool Shader::loadFromFile(const std::string& filename, smpl::Shader::Type type)
    {
        std::vector<char> shader;
        if (!getFileContents(filename, shader))
        {
            LOG_ERROR("Failed to open shader file \"{}\".", filename);
            return false;
        }

        shader_type = getType(type);

        shader_id = glCreateShader(shader_type);

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

        is_compiled = true;
        is_loaded = true;
        LOG_DEBUG("Shader loaded: \"{}\" and compile.", filename);
        return true;
    }

    bool Shader::isLoaded() const
    {
        return is_loaded;
    }

    bool Shader::isCompiled() const
    {
        return is_compiled;
    }

    void Shader::release()
    {
        glDeleteShader(shader_id);
    }

    void Shader::bind()
    {
        LOG_WARN("Do this implementation! It`s do nothing!");
    }

    GLuint Shader::getID() const
    {
        return shader_id;
    }

    bool Shader::getFileContents(const std::string& filename, std::vector<char>& buffer)
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

    ShaderProgram::ShaderProgram()
        : program_id{ 0 }
        , is_linked{ false }
    {
        // Do nothing
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(program_id);
    }

    ShaderProgram::ShaderProgram(smpl::ShaderProgram&& other) noexcept
        : program_id(other.program_id)
        , is_linked(other.is_linked)
    {
        other.program_id = 0;
        other.is_linked = false;
    }

    ShaderProgram& smpl::ShaderProgram::operator=(smpl::ShaderProgram&& other) noexcept
    {
        glDeleteProgram(program_id);
        program_id = other.program_id;
        is_linked = other.is_linked;

        other.program_id = 0;
        other.is_linked = false;

        return *this;
    }

    void ShaderProgram::create()
    {
        program_id = glCreateProgram();
    }

    bool ShaderProgram::bind(const Shader& shader) const
    {
        if (!shader.isCompiled())
            return false;

        glAttachShader(program_id, shader.getID());
        return true;
    }

    bool ShaderProgram::link()
    {
        if (is_linked)
            return true;

        glLinkProgram(program_id);
        GLint linkStatus;
        glGetProgramiv(program_id, GL_LINK_STATUS, &linkStatus);
        is_linked = linkStatus == GL_TRUE;

        if (!is_linked)
        {
            LOG_ERROR("Shader program wasn't linked!");

            GLint logLength;
            glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &logLength);

            if (logLength > 0)
            {
                GLchar* logMessage = new GLchar[logLength];
                glGetProgramInfoLog(program_id, logLength, nullptr, logMessage);
                LOG_ERROR("The linker returned: {}", logMessage);
                delete[] logMessage;
            }

            return false;
        }

        return is_linked;
    }

    void ShaderProgram::use() const
    {
        if (program_id == 0)
        {
            LOG_ERROR("Attempt to use uninitialized shader program!");
            return;
        }
        if (!is_linked)
        {
            LOG_WARN("Shader program is not linked!");
        }
        glUseProgram(program_id);
    }

    void ShaderProgram::setUniform1i(const std::string& name, int x) const
    {
        GLint location = glGetUniformLocation(program_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniform1i(location, x);
    }

    void ShaderProgram::setUniform1f(const std::string& name, float x) const
    {
        GLint location = glGetUniformLocation(program_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniform1f(location, x);
    }

    void ShaderProgram::setUniform3f(const std::string& name, float x, float y, float z) const
    {
        GLint location = glGetUniformLocation(program_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniform3f (location, x,y,z);
    }

    void ShaderProgram::setUniformMatrix(const std::string& name, const glm::mat4& matrix) const
    {
        GLint location = glGetUniformLocation(program_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void ShaderProgram::setUniformMatrix(const std::string& name, const glm::mat3& matrix) const
    {
        GLint location = glGetUniformLocation(program_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    GLuint ShaderProgram::getID() const
    {
        return program_id;
    }

    const bool ShaderProgram::isLinked() const
    {
        return program_id;
    }
}