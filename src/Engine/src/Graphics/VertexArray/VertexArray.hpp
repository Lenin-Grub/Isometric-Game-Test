#pragma once
#include <glad/glad.h>

class VertexArray
{
private:
    unsigned int m_RendererID = 0;
public:
    VertexArray()
    {
        glGenVertexArrays(1, &m_RendererID);
    }
    ~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void unbind() const
    {
        glBindVertexArray(0);
    }

    unsigned int getID() const { return m_RendererID; }
};