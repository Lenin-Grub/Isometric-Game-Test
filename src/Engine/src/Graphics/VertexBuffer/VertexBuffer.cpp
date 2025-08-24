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
}

smpl::VertexBuffer::VertexBuffer(const void* data, const size_t size, const smpl::VertexBuffer::Usage usage)
    : m_id { 0 }
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
{
    m_id = vertex_buffer.m_id;
    vertex_buffer.m_id = 0;
}

void smpl::VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void smpl::VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
