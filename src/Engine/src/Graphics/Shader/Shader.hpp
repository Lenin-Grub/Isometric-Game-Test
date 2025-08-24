#pragma once
#include <glad/glad.h>
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


        GLuint getID()   const;

    private:
        bool getFileContents(const std::string& filename, std::vector<char>& buffer);

        GLuint shader_id;
        GLenum shader_type;
        bool   is_loaded;
        bool   is_compiled;
    };

    class ShaderProgram
    {
    public:
        ShaderProgram();

        void create();
        bool bind(const Shader& shader) const;
        bool link();
        void use() const;
        void release();

        GLuint getID() const;

    private:
        GLuint program_id;
        bool is_linked;
    };
}
