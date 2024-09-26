#include "rendering/buffer/vao.hpp"

#include <glad/glad.h>

#include "rendering/rhi.hpp"

using namespace XnorCore;

void Vao::BindBuffer() const
{
    glBindVertexArray(m_Id);
}

void Vao::UnBindBuffer() const
{
    glBindVertexArray(0);
}

uint32_t Vao::GetId() const
{
    return m_Id;
}

void Vao::ComputeDescriptor(const VaoDescriptor& vaoDescriptor) const
{
    for (size_t i = 0; i < vaoDescriptor.vertexAttributeBindingSize; i++)
    {
        const VertexAttributeBinding& vertexAttributeBinding = vaoDescriptor.vertexAttributeBindings[i];

        const VertexAttribFormat& vertexAttribFormats = vaoDescriptor.vertexAttribFormats[i];
        glEnableVertexArrayAttrib(m_Id,static_cast<uint32_t>(i));
        glVertexArrayAttribBinding(m_Id, vertexAttributeBinding.attribIndex, vertexAttributeBinding.bindingIndex);
        glVertexArrayAttribFormat(m_Id, vertexAttribFormats.attribIndex, static_cast<int32_t>(vertexAttribFormats.size), Rhi::GetOpenglDataType(vertexAttribFormats.type), vertexAttribFormats.normalized, vertexAttribFormats.relativeOffset);
    }
    
    glVertexArrayVertexBuffer(m_Id, 0, vaoDescriptor.vboId, 0, vaoDescriptor.vertexAttribFormats[0].size * 4);


}

void Vao::Init()
{
    glCreateVertexArrays(1, &m_Id);
}

Vao::~Vao()
{
    if (glIsBuffer(m_Id))
        glDeleteBuffers(1, &m_Id);
}
