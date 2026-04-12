#include <Graphics/VertexBuffer/VertexBuffer.hpp>

namespace smpl
{
    BufferLayout::Element::Element(ShaderDataType type, unsigned int index)
        : type(type)
        , index(index)
    {
        count = GetShaderDataComponentCount(type);
        offset = 0;
    }
    unsigned int BufferLayout::Element::getSize() const
    {
        return GetShaderDataSize(type);
    }

    BufferLayout::BufferLayout(std::initializer_list<ShaderDataType> types)
    {
        unsigned int index = 0;
        for (auto type : types)
        {
            m_elements.emplace_back(type, index++);
        }
        calculate();
    }

    std::string BufferLayout::toString(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:   return "Float";
        case ShaderDataType::Float2:  return "Float2";
        case ShaderDataType::Float3:  return "Float3";
        case ShaderDataType::Float4:  return "Float4";
        case ShaderDataType::Mat3:    return "Mat3";
        case ShaderDataType::Mat4:    return "Mat4";
        case ShaderDataType::Int:     return "Int";
        case ShaderDataType::Int2:    return "Int2";
        case ShaderDataType::Int3:    return "Int3";
        case ShaderDataType::Int4:    return "Int4";
        case ShaderDataType::Bool:    return "Bool";
        default:                      return "Unknown";
        }
    }

    void BufferLayout::setLayout()
    {
        // tmp
        printInfo();

        for (const auto& element : m_elements)
        {
            glVertexAttribPointer(
                element.index,
                element.count,
                GetShaderDataTypeOpenGL(element.type),
                GL_FALSE,
                static_cast<GLsizei>(getStride()),
                reinterpret_cast<const void*>(element.offset)
            );
            glEnableVertexAttribArray(element.index);
        }
    }

    const std::vector<smpl::BufferLayout::Element>& BufferLayout::getElements() const
    {
        return m_elements;
    }

    size_t BufferLayout::getStride() const
    {
        return m_stride;
    }

    void BufferLayout::printInfo()
    {
        printf("Stride: %zu\n", getStride());
        for (const auto& element : m_elements)
        {
            printf("Index: %u,\tCount: %u,\tOffset: %zu,\tType: %s\n",
                element.index,
                element.count,
                element.offset,
                toString(element.type).c_str());
        }
        printf("\n");
    }

    void BufferLayout::calculate()
    {
        size_t offset = 0;
        for (auto& element : m_elements)
        {
            element.offset = offset;
            offset += element.getSize();
        }
        m_stride = offset;
    }
}
