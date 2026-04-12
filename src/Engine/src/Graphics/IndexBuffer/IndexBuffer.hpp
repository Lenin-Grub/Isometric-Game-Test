#pragma once
#include <glad/glad.h>

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBuffer() = default;
    IndexBuffer(const unsigned int* data, unsigned int count)
        : m_Count(count)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }

    ~IndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline unsigned int getCount() const { return m_Count; }

    void create(const unsigned int* data, unsigned int count) {
        if (m_RendererID) {
            glDeleteBuffers(1, &m_RendererID);
        }
        m_Count = count;
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }
};