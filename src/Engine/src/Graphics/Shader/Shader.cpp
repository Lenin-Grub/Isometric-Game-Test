#include <Graphics/Shader/Shader.hpp>

#include <Log/Log.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>

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
        : m_is_loaded{ false }
        , m_is_compiled{ false }
        , m_shader_id{ 0 }
        , m_shader_type{ 0 }
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

        m_shader_type = getType(type);
        m_current_type = type;

        m_shader_id = glCreateShader(m_shader_type);

        const char* src = shader.data();
        glShaderSource(m_shader_id, 1, &src, nullptr);

        glCompileShader(m_shader_id);

        GLint compileStatus;
        glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus != GL_TRUE)
        {
            GLint logLength;
            glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> log(logLength);
            glGetShaderInfoLog(m_shader_id, logLength, nullptr, log.data());

            LOG_ERROR("Shader compilation failed for '{}':\n{}", filename, std::string(log.data()));

            glDeleteShader(m_shader_id);
            return false;
        }

        m_is_compiled = true;
        m_is_loaded = true;
        LOG_DEBUG("Shader loaded: \"{}\" and compile.", filename);
        return true;
    }

    bool Shader::isLoaded() const
    {
        return m_is_loaded;
    }

    bool Shader::isCompiled() const
    {
        return m_is_compiled;
    }

    void Shader::release()
    {
        glDeleteShader(m_shader_id);
    }

    smpl::Shader::Type Shader::getCurrentType() const
    {
        return m_current_type;
    }

    GLuint Shader::getID() const
    {
        return m_shader_id;
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
        : m_id{ 0 }
        , m_is_linked{ false }
    {
        // Do nothing
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_id);
    }

    ShaderProgram::ShaderProgram(smpl::ShaderProgram&& other) noexcept
        : m_id(other.m_id)
        , m_is_linked(other.m_is_linked)
    {
        other.m_id = 0;
        other.m_is_linked = false;
    }

    ShaderProgram& smpl::ShaderProgram::operator=(smpl::ShaderProgram&& other) noexcept
    {
        glDeleteProgram(m_id);
        m_id = other.m_id;
        m_is_linked = other.m_is_linked;

        other.m_id = 0;
        other.m_is_linked = false;

        return *this;
    }

    bool ShaderProgram::create(smpl::Shader& vertex_shader, smpl::Shader& fragment_shader)
    {
        bool is_create = false;
        createProgram();

        // Vertex
        if (vertex_shader.getCurrentType() == smpl::Shader::Vertex)
        {
            if (!bind(vertex_shader))
            {
                vertex_shader.release();
                fragment_shader.release();
                return is_create;
            }
        }
        else
        {
            LOG_ERROR("Incorrect type of shader! Should be \"Vertex\".");
            vertex_shader.release();
            fragment_shader.release();
            LOG_ERROR("Vertex shader bind failed.");
            return is_create;
        }

        // Fragment
        if (fragment_shader.getCurrentType() == smpl::Shader::Fragment)
        {
            if (!bind(fragment_shader))
            {
                vertex_shader.release();
                fragment_shader.release();
                LOG_ERROR("Fragment shader bind failed.");
                return is_create;
            }
        }
        else
        {
            LOG_ERROR("Incorrect type of shader! Should be \"Fragment\".");
            vertex_shader.release();
            fragment_shader.release();
            return is_create;
        }

        if (!link())
        {
            vertex_shader.release();
            fragment_shader.release();
            return is_create;
        }

        vertex_shader.release();
        fragment_shader.release();

        is_create = true;
        return is_create;
    }

    void ShaderProgram::createProgram()
    {
        m_id = glCreateProgram();
    }

    bool ShaderProgram::bind(const Shader& shader) const
    {
        if (!shader.isCompiled())
            return false;

        glAttachShader(m_id, shader.getID());
        return true;
    }

    bool ShaderProgram::link()
    {
        if (m_is_linked)
            return true;

        glLinkProgram(m_id);
        GLint linkStatus;
        glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);
        m_is_linked = linkStatus == GL_TRUE;

        if (!m_is_linked)
        {
            LOG_ERROR("Shader program wasn't linked!");

            GLint logLength;
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logLength);

            if (logLength > 0)
            {
                GLchar* logMessage = new GLchar[logLength];
                glGetProgramInfoLog(m_id, logLength, nullptr, logMessage);
                LOG_ERROR("The linker returned: {}", logMessage);
                delete[] logMessage;
            }

            return false;
        }

        return m_is_linked;
    }

    void ShaderProgram::use() const
    {
        if (m_id == 0)
        {
            LOG_ERROR("Attempt to use uninitialized shader program!");
            return;
        }
        if (!m_is_linked)
        {
            LOG_WARN("Shader program is not linked!");
        }
        glUseProgram(m_id);
    }

    void ShaderProgram::setUniform1i(const std::string& name, int x) const
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniform1i(location, x);
    }

    void ShaderProgram::setUniform1f(const std::string& name, float x) const
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniform1f(location, x);
    }

    void ShaderProgram::setUniform3f(const std::string& name, float x, float y, float z) const
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniform3f (location, x,y,z);
    }

    void ShaderProgram::setUniform3f(const std::string& name, const glm::vec3& xyz) const
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniform3f(location, xyz.x, xyz.y, xyz.z);
    }

    void ShaderProgram::setUniform4f(const std::string& name, const glm::vec4& xyzw) const
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniform4f(location, xyzw.x, xyzw.y, xyzw.z, xyzw.w);
    }

    void ShaderProgram::setUniformMatrix(const std::string& name, const glm::mat4& matrix) const
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void ShaderProgram::setUniformMatrix(const std::string& name, const glm::mat3& matrix) const
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());

        if (location == -1)
        {
            LOG_WARN("Uniform \"{}\" not found.", name);
            return;
        }

        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    GLuint ShaderProgram::getID() const
    {
        return m_id;
    }

    const bool ShaderProgram::isLinked() const
    {
        return m_id;
    }
}