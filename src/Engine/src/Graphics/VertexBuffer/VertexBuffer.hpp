#pragma once

namespace smpl
{
    enum class ShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
    };

    struct BufferElement
    {
        ShaderDataType type;
        uint32_t component_type;
        size_t components_count;
        size_t size;
        size_t offset;

        BufferElement(const ShaderDataType type);
    };

    class BufferLayout
    {
    public:
        BufferLayout(std::initializer_list<smpl::BufferElement> elements)
            : m_elements(std::move(elements))
        {
            size_t offset = 0;
            m_stride = 0;
            for (auto& element : m_elements)
            {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

        const std::vector<smpl::BufferElement>& getElements() const { return m_elements; }
        size_t getStride() const { return m_stride; }

    private:
        std::vector<smpl::BufferElement> m_elements;
        size_t m_stride = 0;
    };

    class VertexBuffer
    {
    public:

        enum class Usage
        {
            Static,
            Dynamic,
            Stream
        };

        VertexBuffer(const void* data, const size_t size, smpl::BufferLayout buffer_layout,const smpl::VertexBuffer::Usage usage = smpl::VertexBuffer::Usage::Static);
        ~VertexBuffer();

        VertexBuffer(const smpl::VertexBuffer&) = delete;
        smpl::VertexBuffer& operator=(const smpl::VertexBuffer&) = delete;
        smpl::VertexBuffer& operator=(smpl::VertexBuffer&& vertex_buffer) noexcept;
        VertexBuffer(smpl::VertexBuffer&& vertex_buffer) noexcept;

        void bind() const;
        static void unbind();

        const smpl::BufferLayout& getLayout() const;
    private:
        unsigned int m_id;
        smpl::BufferLayout buffer_layout;
    };
}