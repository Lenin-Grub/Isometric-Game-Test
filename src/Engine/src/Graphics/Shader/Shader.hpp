#pragma once
#include <vector>
#include <string>

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

        uint32_t getID() const;

    private:
        uint32_t program_id;
        bool     is_linked;
    };
}
