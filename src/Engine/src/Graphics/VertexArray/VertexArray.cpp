#include <Graphics/VertexArray/VertexArray.hpp>
#include <Log/Log.hpp>

#include <glad/glad.h>
    
smpl::VertexArray::VertexArray()
    : m_id(0)
    , m_elements_count(0)
    , m_indexes(0)
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
    , m_indexes(0)
{
    vertex_array.m_id = 0;
    vertex_array.m_elements_count = 0;
}

void smpl::VertexArray::bind() const
{
    if (m_id == 0)
    {
        LOG_ERROR("Attempt to bind uninitialized VertexArray!");
        return;
    }
    glBindVertexArray(m_id);
}

void smpl::VertexArray::unbind()
{
    glBindVertexArray(0);
}

size_t smpl::VertexArray::getIndexesCount() const
{
    return m_indexes;
}

void smpl::VertexArray::addVertexBuffer(const smpl::VertexBuffer& vertex_array)
{
    bind();
    vertex_array.bind();

    for (const BufferElement& current_element : vertex_array.getLayout().getElements())
    {
        glEnableVertexAttribArray(m_elements_count);
        glVertexAttribPointer(m_elements_count, static_cast<GLint>(current_element.components_count), current_element.component_type, GL_FALSE, static_cast<GLsizei>(vertex_array.getLayout().getStride()), reinterpret_cast<const void*>(current_element.offset));
        ++m_elements_count;
    }
}

void smpl::VertexArray::setIndexBuffer(const smpl::IndexBuffer& index_array)
{
    bind();
    index_array.bind();
    m_indexes = index_array.getCount();
}
