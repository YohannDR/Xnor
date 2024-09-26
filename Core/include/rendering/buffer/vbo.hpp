#pragma once

#include "core.hpp"
#include "rendering/rhi_typedef.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Vbo
{
public:
    Vbo() = default;
    
    ~Vbo();

    DEFAULT_COPY_MOVE_OPERATIONS(Vbo)
    
    /// @brief Allocates a vertex buffer on the GPU
    /// @param size Data size
    /// @param data Data
    /// @param usage How the buffer will be used
    void Allocate(size_t size, const void* data, BufferUsage usage);

    void UpdateData(size_t offset, size_t size, const void* data = nullptr);
    
    void BindBuffer() const;

    void UnBind() const;

    [[nodiscard]]
    uint32_t GetId() const;

    void Init();

private:
    uint32_t m_Id = 0;
};

END_XNOR_CORE
