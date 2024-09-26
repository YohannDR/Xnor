#include "resource/compute_shader.hpp"

#include "file/file.hpp"
#include "rendering/rhi.hpp"

using namespace XnorCore;

bool_t ComputeShader::Load(const Pointer<File>& shader)
{
    if (!Load(shader->GetData(), shader->GetSize()))
        return false;

    m_File = shader;
    m_Loaded = true;
    return m_Loaded;
}

bool_t ComputeShader::Load(const char_t* const buffer, const int64_t length)
{

    ShaderCode& code = m_ShaderCode;
    code.code = buffer;
    code.codeLength = static_cast<int32_t>(length);
    code.type = ShaderType::Compute;
    m_Loaded = true;

    return true;
}

void ComputeShader::CreateInInterface()
{
    constexpr  ShaderCreateInfo shaderCreateInfo;
    
    m_Id = Rhi::CreateShaders({ m_ShaderCode }, shaderCreateInfo);
}

void ComputeShader::DestroyInInterface()
{
    Resource::DestroyInInterface();
}

void ComputeShader::Recompile()
{
}

void ComputeShader::Unload()
{
    Resource::Unload();
}

void ComputeShader::SetInt(const std::string& keyName, const int32_t value) const
{
    Rhi::SetUniform(UniformType::Int, &value, m_Id, keyName.c_str());
}

void ComputeShader::SetBool(const std::string& keyName, const bool_t value) const
{
    Rhi::SetUniform(UniformType::Bool, &value, m_Id, keyName.c_str());
}

void ComputeShader::SetFloat(const std::string& keyName, const float_t value) const
{
    Rhi::SetUniform(UniformType::Float, &value, m_Id, keyName.c_str());
}

void ComputeShader::SetVec2(const std::string& keyName, const Vector2& value) const
{
    Rhi::SetUniform(UniformType::Vec2, &value, m_Id, keyName.c_str());
}

void ComputeShader::SetVec3(const std::string& keyName, const Vector3& value) const
{
    Rhi::SetUniform(UniformType::Vec3, &value, m_Id, keyName.c_str());
}

void ComputeShader::SetVec4(const std::string& keyName, const Vector4& value) const
{
    Rhi::SetUniform(UniformType::Vec4, &value, m_Id, keyName.c_str());
}

void ComputeShader::SetMat4(const std::string& keyName, const Matrix& value) const
{
    Rhi::SetUniform(UniformType::Mat4, &value, m_Id, keyName.c_str());
}

uint32_t ComputeShader::GetId() const
{
   return m_Id;
}

void ComputeShader::Use() const
{
    Rhi::UseShader(m_Id);
}

void ComputeShader::Unuse() const
{
    Rhi::UnuseShader();
}

void ComputeShader::DispatchCompute(const uint32_t numberOfGroupX, const uint32_t numberOfGroupY, const uint32_t numberOfGroupZ) const
{
    Rhi::DispatchCompute(numberOfGroupX, numberOfGroupY, numberOfGroupZ);
}

void ComputeShader::BindImage(const uint32_t unit, const Texture& texture, const uint32_t level, const bool_t layered, const uint32_t layer, const ImageAccess imageAcess) const
{
    Rhi::BindImageTexture(unit,texture.GetId(),level, layered, layer, imageAcess, texture.GetInternalFormat());
}

void ComputeShader::SetMemoryBarrier(const GpuMemoryBarrier memoryBarrier) const
{
    Rhi::SetGpuMemoryBarrier(memoryBarrier);
}
