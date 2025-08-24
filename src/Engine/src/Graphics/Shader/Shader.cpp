#include <Graphics/Shader/Shader.hpp>
#include <Log/Log.hpp>
#include <fstream>

namespace
{
    GLenum getType(smpl::Shader::Type type)
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

smpl::Shader::Shader()
    : is_loaded   { false }
    , is_compiled { false }
    , shader_id   { 0 }
    , shader_type { 0 }
{
    //Do nothing
}

bool smpl::Shader::loadFromFile(const std::string& filename, smpl::Shader::Type type)
{
    std::vector<char> shader;
    if (!getFileContents(filename, shader))
    {
        LOG_ERROR("Failed to open shader file \"{}\".", filename );
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
    return true;
}

bool smpl::Shader::isLoaded() const
{
    return is_loaded;
}

bool smpl::Shader::isCompiled() const
{
    return is_compiled;
}

void smpl::Shader::release()
{
    glDeleteShader(shader_id);
}

void smpl::Shader::bind()
{
}

GLuint smpl::Shader::getID() const
{
    return shader_id;
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

smpl::ShaderProgram::ShaderProgram()
    : program_id { 0 }
    , is_linked  { false }
{
    // Do nothing
}

void smpl::ShaderProgram::create()
{
    program_id = glCreateProgram();
}

bool smpl::ShaderProgram::bind(const Shader& shader) const
{
    if (!shader.isCompiled())
        return false;

    glAttachShader(program_id, shader.getID());
    return true;
}

bool smpl::ShaderProgram::link()
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

void smpl::ShaderProgram::use() const
{
    if (is_linked)
    {
        glUseProgram(program_id);
    }
}

GLuint smpl::ShaderProgram::getID() const
{
    return program_id;
}
