#include <Graphics/VertexArray/VertexArray.hpp>
#include <Log/Log.hpp>

#include <glad/glad.h>
    
smpl::VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_id);
    }

smpl::VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

smpl::VertexArray& smpl::VertexArray::operator=(smpl::VertexArray&& vertex_array) noexcept
{
    m_id = vertex_array.m_id;
    m_elements_count = vertex_array.m_id;
    vertex_array.m_id = 0;
    vertex_array.m_elements_count = 0;
    return *this;
}

smpl::VertexArray::VertexArray(smpl::VertexArray&& vertex_array) noexcept
        : m_id(vertex_array.m_id)
        , m_elements_count(vertex_array.m_elements_count)
    {
        vertex_array.m_id = 0;
        vertex_array.m_elements_count = 0;
    }

    void smpl::VertexArray::bind() const
    {
        glBindVertexArray(m_id);
    }

    void smpl::VertexArray::unbind()
    {
        glBindVertexArray(0);
    }

    void smpl::VertexArray::addBuffer(const smpl::VertexBuffer& vertex_array)
    {
        bind();
        vertex_array.bind();

        //TODO - use buffer layout
        glEnableVertexAttribArray(m_elements_count);
        glVertexAttribPointer(m_elements_count, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        ++m_elements_count;
    }