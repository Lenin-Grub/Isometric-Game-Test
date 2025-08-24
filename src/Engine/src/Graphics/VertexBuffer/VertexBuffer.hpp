#pragma once

namespace smpl
{
    class VertexBuffer
    {
    public:

        enum class Usage
        {
            Static,
            Dynamic,
            Stream
        };

        VertexBuffer(const void* data, const size_t size, const smpl::VertexBuffer::Usage usage = smpl::VertexBuffer::Usage::Static);
        ~VertexBuffer();

        VertexBuffer(const smpl::VertexBuffer&) = delete;
        smpl::VertexBuffer& operator=(const smpl::VertexBuffer&) = delete;
        smpl::VertexBuffer& operator=(smpl::VertexBuffer&& vertex_buffer) noexcept;
        VertexBuffer(smpl::VertexBuffer&& vertex_buffer) noexcept;

        void bind() const;
        static void unbind();

    private:
        unsigned int m_id;
    };
}