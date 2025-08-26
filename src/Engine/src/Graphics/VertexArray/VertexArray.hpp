#pragma once
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/IndexBuffer/IndexBuffer.hpp>

namespace smpl
{
    class VertexArray 
    {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const smpl::VertexArray&) = delete;
        smpl::VertexArray& operator=(const smpl::VertexArray&) = delete;
        smpl::VertexArray& operator=(smpl::VertexArray&& vertex_array) noexcept;
        VertexArray(smpl::VertexArray&& vertex_array) noexcept;

        void addVertexBuffer(const smpl::VertexBuffer& vertex_array);
        void setIndexBuffer(const smpl::IndexBuffer& index_array);
        void bind() const;
        static void unbind();

        size_t getIndexesCount() const;

    private:
        unsigned int m_id;
        unsigned int m_elements_count;
        size_t m_indexes;
    };
}
