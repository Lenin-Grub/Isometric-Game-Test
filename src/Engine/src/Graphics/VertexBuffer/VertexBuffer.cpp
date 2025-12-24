#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Log/Log.hpp>

#include <glad/glad.h>

namespace 
{
    constexpr GLenum usageToGLenum(const smpl::VertexBuffer::Usage usage)
    {
        switch (usage)
        {
        case smpl::VertexBuffer::Usage::Static:  return GL_STATIC_DRAW;
        case smpl::VertexBuffer::Usage::Dynamic: return GL_DYNAMIC_DRAW;
        case smpl::VertexBuffer::Usage::Stream:  return GL_STREAM_DRAW;
        }

        LOG_ERROR("Unknown VertexBuffer usage");
        return GL_STREAM_DRAW;
    }

    constexpr unsigned int shaderDataTypeToComponentsCount(const smpl::ShaderDataType type)
    {
        switch (type)
        {
        case smpl::ShaderDataType::Float:
        case smpl::ShaderDataType::Int:
            return 1;

        case smpl::ShaderDataType::Float2:
        case smpl::ShaderDataType::Int2:
            return 2;

        case smpl::ShaderDataType::Float3:
        case smpl::ShaderDataType::Int3:
            return 3;

        case smpl::ShaderDataType::Float4:
        case smpl::ShaderDataType::Int4:
            return 4;
        }

        LOG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
        return 0;
    }


    constexpr size_t shaderDataTypeSize(const smpl::ShaderDataType type)
    {
        switch (type)
        {
        case smpl::ShaderDataType::Float:
        case smpl::ShaderDataType::Float2:
        case smpl::ShaderDataType::Float3:
        case smpl::ShaderDataType::Float4:
            return sizeof(GLfloat) * shaderDataTypeToComponentsCount(type);

        case smpl::ShaderDataType::Int:
        case smpl::ShaderDataType::Int2:
        case smpl::ShaderDataType::Int3:
        case smpl::ShaderDataType::Int4:
            return sizeof(GLint) * shaderDataTypeToComponentsCount(type);
        }

        LOG_ERROR("shader_data_type_size: unknown ShaderDataType!");
        return 0;
    }


    constexpr unsigned int shaderDataTypeToComponentType(const smpl::ShaderDataType type)
    {
        switch (type)
        {
        case smpl::ShaderDataType::Float:
        case smpl::ShaderDataType::Float2:
        case smpl::ShaderDataType::Float3:
        case smpl::ShaderDataType::Float4:
            return GL_FLOAT;

        case smpl::ShaderDataType::Int:
        case smpl::ShaderDataType::Int2:
        case smpl::ShaderDataType::Int3:
        case smpl::ShaderDataType::Int4:
            return GL_INT;
        }

        LOG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
        return GL_FLOAT;
    }
}

smpl::BufferElement::BufferElement(const ShaderDataType type)
    : type(type)
    , component_type(shaderDataTypeToComponentType(type))
    , components_count(shaderDataTypeToComponentsCount(type))
    , size(shaderDataTypeSize(type))
    , offset(0)
{
}

smpl::VertexBuffer::VertexBuffer(const void* data, const size_t size, smpl::BufferLayout buffer_layout, const smpl::VertexBuffer::Usage usage)
    //: m_id { 0 }
    : buffer_layout(std::move(buffer_layout))
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, usageToGLenum(usage));
}

smpl::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

smpl::VertexBuffer& smpl::VertexBuffer::operator=(smpl::VertexBuffer&& vertex_buffer) noexcept
{
    m_id = vertex_buffer.m_id;
    vertex_buffer.m_id = 0;
    return *this;
}

smpl::VertexBuffer::VertexBuffer(VertexBuffer&& vertex_buffer) noexcept
    : m_id(vertex_buffer.m_id)
    , buffer_layout(std::move(vertex_buffer.buffer_layout))
{
    //m_id = vertex_buffer.m_id;
    vertex_buffer.m_id = 0;
}

void smpl::VertexBuffer::bind() const
{
    if (m_id == 0)
    {
        LOG_ERROR("Attempt to bind uninitialized VertexBuffer!");
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void smpl::VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const smpl::BufferLayout& smpl::VertexBuffer::getLayout() const
{
    return buffer_layout;
}
