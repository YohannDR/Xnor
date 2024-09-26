#include "rendering/buffer/uniform_buffer.hpp"

#include <glad/glad.h>

using namespace XnorCore;

UniformBuffer::UniformBuffer()
{
    glCreateBuffers(1, &m_Id);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_Id);
}

void UniformBuffer::Allocate(const size_t size, const void* const data) const
{
    glNamedBufferStorage(m_Id, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_STORAGE_BIT);
}

void UniformBuffer::Update(const size_t size, const size_t offset, const void* const data) const
{
    glNamedBufferSubData(m_Id, static_cast<GLsizeiptr>(offset), static_cast<GLsizeiptr>(size), data);   
}

void UniformBuffer::Bind(const uint32_t index) const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, m_Id);
}
