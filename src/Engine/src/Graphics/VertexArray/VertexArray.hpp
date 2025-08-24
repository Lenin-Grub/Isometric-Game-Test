#pragma once
#include <Graphics/VertexBuffer/VertexBuffer.hpp>

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

        void addBuffer(const smpl::VertexBuffer& vertex_array);
        void bind() const;
        static void unbind();

    private:
        unsigned int m_id = 0;
        unsigned int m_elements_count = 0;
    };
}
