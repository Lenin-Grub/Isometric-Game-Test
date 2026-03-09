#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <Utils/NonCopyable.hpp>

namespace smpl
{
    class Shader
    {
    public:

        enum Type
        {
            Vertex = 0,
            Fragment
        };

        Shader();
        ~Shader() = default;

        bool loadFromFile(const std::string& filename, smpl::Shader::Type type);
        bool isLoaded() const;
        bool isCompiled() const;
        void release();

        smpl::Shader::Type getCurrentType() const;
        uint32_t getID()   const;

    private:
        bool getFileContents(const std::string& filename, std::vector<char>& buffer);

        uint32_t m_shader_id;
        uint32_t m_shader_type;
        bool     m_is_loaded;
        bool     m_is_compiled;
        smpl::Shader::Type m_current_type;
    };

    class ShaderProgram
        : public smpl::NonCopyable
    {
    public:
        ShaderProgram();
        ~ShaderProgram();
        ShaderProgram(smpl::ShaderProgram&& other) noexcept;
        smpl::ShaderProgram& operator=(smpl::ShaderProgram&& other) noexcept;

        bool create(smpl::Shader& vertex_shader, smpl::Shader& fragment_shader);
        void use() const;

        void setUniform1i(const std::string& name, int x) const;
        void setUniform1f(const std::string& name, float x) const;
        void setUniform2f(const std::string& name, float x, float y) const;
        void setUniform2f(const std::string& name, const glm::vec2& xy) const;
        void setUniform2i(const std::string& name, const glm::ivec2& xy) const;
        void setUniform3f(const std::string& name, float x, float y, float z) const;
        void setUniform3f(const std::string& name, const glm::vec3& xyz) const;
        void setUniform4f(const std::string& name, const glm::vec4& xyzw) const;

        void setUniform1v(const std::string& name, int length, const int* v) const;
        void setUniform1v(const std::string& name, int length, const float* v) const;
        void setUniform2v(const std::string& name, int length, const float* v) const;
        void setUniform3v(const std::string& name, int length, const float* v) const;
        void setUniform4v(const std::string& name, int length, const float* v) const;

        void setUniformMatrix(const std::string& name, const glm::mat4& matrix) const;
        void setUniformMatrix(const std::string& name, const glm::mat3& matrix) const;

        uint32_t getID() const;
    private:

        void createProgram();
        bool bind(const Shader& shader) const;
        bool link();
        const bool isLinked() const;

    private:
        uint32_t m_id;
        bool     m_is_linked;
    };
}
