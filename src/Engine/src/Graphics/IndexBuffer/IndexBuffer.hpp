#pragma once
#include <Graphics/VertexBuffer/VertexBuffer.hpp>

namespace smpl
{
    class IndexBuffer 
    {
    public:

        IndexBuffer(const void* data, const size_t count, const smpl::VertexBuffer::Usage usage = smpl::VertexBuffer::Usage::Static);
        ~IndexBuffer();

        IndexBuffer(const smpl::IndexBuffer&) = delete;
        smpl::IndexBuffer& operator=(const smpl::IndexBuffer&) = delete;
        smpl::IndexBuffer& operator=(smpl::IndexBuffer&& index_buffer) noexcept;
        IndexBuffer(smpl::IndexBuffer&& index_buffer) noexcept;

        void bind() const;
        static void unbind();
        size_t getCount() const;

    private:
        unsigned int m_id;
        size_t m_count;
    };
}
