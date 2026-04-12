#pragma once
#include <Log/Log.hpp>
#include <glad/glad.h>

#include <cstdint>
#include <vector>
#include <string>

namespace smpl 
{
    enum class ShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static unsigned int GetShaderDataComponentCount(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:   return 1;
        case ShaderDataType::Float2:  return 2;
        case ShaderDataType::Float3:  return 3;
        case ShaderDataType::Float4:  return 4;
        case ShaderDataType::Mat3:    return 12;
        case ShaderDataType::Mat4:    return 16;
        case ShaderDataType::Int:     return 1;
        case ShaderDataType::Int2:    return 2;
        case ShaderDataType::Int3:    return 3;
        case ShaderDataType::Int4:    return 4;
        case ShaderDataType::Bool:    return 1;
        }
        return 0;
    }

    static unsigned int GetShaderDataSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:   return 4;
        case ShaderDataType::Float2:  return 8;
        case ShaderDataType::Float3:  return 12;
        case ShaderDataType::Float4:  return 16;
        case ShaderDataType::Mat3:    return 36;
        case ShaderDataType::Mat4:    return 64;
        case ShaderDataType::Int:     return 4;
        case ShaderDataType::Int2:    return 8;
        case ShaderDataType::Int3:    return 12;
        case ShaderDataType::Int4:    return 16;
        case ShaderDataType::Bool:    return 1;
        }
        return 0;
    }

    static GLenum GetShaderDataTypeOpenGL(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return GL_FLOAT;

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;

        case ShaderDataType::Bool:
            return GL_BOOL;

        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        }
        return 0;
    }

    class BufferLayout
    {
    public:
        struct Element
        {
            ShaderDataType type;
            size_t         offset;
            unsigned int   count;
            unsigned int   index;

            Element(ShaderDataType type, unsigned int index);
            ~Element() = default;

            unsigned int getSize() const;
        };

    private:
        std::vector<Element> m_elements;
        size_t m_stride;

    public:
        BufferLayout(std::initializer_list<ShaderDataType> types);
        ~BufferLayout() = default;

        void setLayout();
        const std::vector<Element>& getElements() const;
        size_t getStride() const;
        void printInfo(); //tmp delete later

    private:
        std::string toString(ShaderDataType type);
        void calculate();
    };
};