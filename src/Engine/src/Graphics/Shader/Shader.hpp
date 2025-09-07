#pragma once
#include <glm/glm.hpp>

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

        void bind();

        uint32_t getID()   const;

    private:
        bool getFileContents(const std::string& filename, std::vector<char>& buffer);

        uint32_t shader_id;
        uint32_t shader_type;
        bool     is_loaded;
        bool     is_compiled;
    };

    class ShaderProgram
    {
    public:
        ShaderProgram();
        ~ShaderProgram();
        ShaderProgram(smpl::ShaderProgram&& other) noexcept;
        ShaderProgram(const smpl::ShaderProgram&) = delete;
        smpl::ShaderProgram& operator=(const smpl::ShaderProgram&) = delete;
        smpl::ShaderProgram& operator=(smpl::ShaderProgram&& other) noexcept;

        void create();
        bool bind(const Shader& shader) const;
        bool link();
        void use() const;

        void setUniform1i(const std::string& name, int x);
        void setUniform1f(const std::string& name, float x);
        void setUniform2f(const std::string& name, float x, float y);
        void setUniform2f(const std::string& name, const glm::vec2& xy);
        void setUniform2i(const std::string& name, const glm::ivec2& xy);
        void setUniform3f(const std::string& name, float x, float y, float z);
        void setUniform3f(const std::string& name, const glm::vec3& xyz);
        void setUniform4f(const std::string& name, const glm::vec4& xyzw);

        void setUniform1v(const std::string& name, int length, const int* v);
        void setUniform1v(const std::string& name, int length, const float* v);
        void setUniform2v(const std::string& name, int length, const float* v);
        void setUniform3v(const std::string& name, int length, const float* v);
        void setUniform4v(const std::string& name, int length, const float* v);

        void setUniformMatrix(const std::string& name, const glm::mat4& matrix);
        void setUniformMatrix(const std::string& name, const glm::mat3& matrix);


        uint32_t getID() const;
        const bool isLinked() const;

    private:
        uint32_t program_id;
        bool     is_linked;
    };
}
