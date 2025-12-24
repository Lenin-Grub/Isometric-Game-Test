#include "IndexBuffer.hpp"
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

smpl::IndexBuffer::IndexBuffer(const void* data, const size_t count, const smpl::VertexBuffer::Usage usage)
    : m_count(count)
    , m_id ( 0 )
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, usageToGLenum(usage));
}


smpl::IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}


smpl::IndexBuffer& smpl::IndexBuffer::operator=(smpl::IndexBuffer&& index_buffer) noexcept
{
    m_id = index_buffer.m_id;
    m_count = index_buffer.m_count;
    index_buffer.m_id = 0;
    index_buffer.m_count = 0;
    return *this;
}


smpl::IndexBuffer::IndexBuffer(smpl::IndexBuffer&& index_buffer) noexcept
    : m_id(index_buffer.m_id)
    , m_count(index_buffer.m_count)
{
    index_buffer.m_id = 0;
    index_buffer.m_count = 0;
}


void smpl::IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}


void smpl::IndexBuffer::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

size_t smpl::IndexBuffer::getCount() const
{
    return m_count;
}
