#include "rendering/buffer/vbo.hpp"

#include <glad/glad.h>

#include "rendering/rhi.hpp"
#include "rendering/buffer/vao.hpp"

using namespace XnorCore;

Vbo::~Vbo()
{
    if (glIsBuffer(m_Id))
    glDeleteBuffers(1, &m_Id);
}

void Vbo::  Allocate(const size_t size, const void* const data , const BufferUsage bufferUsage)
{
    glNamedBufferData(m_Id, static_cast<uint32_t>(size), data, Rhi::BufferUsageToOpenglUsage(bufferUsage));
    
}

void Vbo::UpdateData(const size_t offset, const size_t size, const void* const data)
{
    glNamedBufferSubData(m_Id, offset, size, data);
}


void Vbo::BindBuffer() const
{
    glBindBuffer(GL_ARRAY_BUFFER,m_Id);
}

void Vbo::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

uint32_t Vbo::GetId() const
{
    return m_Id;
}

void Vbo::Init()
{
    glCreateBuffers(1, &m_Id);
}


