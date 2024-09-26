#pragma once

#include "core.hpp"

/// @file uniform_buffer.hpp
/// @brief Defines the XnorCore::UniformBuffer class

BEGIN_XNOR_CORE

/// @brief Encapsulates a uniform buffer, which is used to send bulks of data in shaders
class UniformBuffer
{
public:
    UniformBuffer();
    ~UniformBuffer();

    DEFAULT_COPY_MOVE_OPERATIONS(UniformBuffer)

    /// @brief Allocates a uniform buffer on the GPU
    /// @param size Data size
    /// @param data Data
    void Allocate(size_t size, const void* data) const;

    /// @brief Updates the data of the buffer on the GPU
    /// @param size Data size
    /// @param offset Data offset
    /// @param data Data
    void Update(size_t size, size_t offset, const void* data) const;

    /// @brief Binds the uniform buffer
    /// @param index Index
    void Bind(uint32_t index) const;
    
private:
    uint32_t m_Id;
};

END_XNOR_CORE
