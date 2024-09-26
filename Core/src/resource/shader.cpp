#include "resource/shader.hpp"

#include "rendering/rhi.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

ShaderPipeline::ShaderPipeline Shader::ShaderTypeToShaderPipeline(ShaderType::ShaderType shaderType)
{
    switch (shaderType)
    {
        case ShaderType::Vertex: 
            return ShaderPipeline::Vertex;
        
        case ShaderType::Fragment: 
            return ShaderPipeline::Fragment;
        
        case ShaderType::Geometry: 
            return ShaderPipeline::Geometry;
    }

    return ShaderPipeline::Vertex;
}

ShaderType::ShaderType Shader::ShaderPipelineToShaderType(ShaderPipeline::ShaderPipeline shaderPipeline)
{
    switch (shaderPipeline)
    {
        case ShaderPipeline::Vertex: 
            return ShaderType::Vertex;
        
        case ShaderPipeline::Fragment: 
            return ShaderType::Fragment;
        
        case ShaderPipeline::Geometry: 
            return ShaderType::Geometry;
    }

    return ShaderType::Vertex;
}

ShaderPipeline::ShaderPipeline Shader::FileExtensionToType(const std::string& extension)
{
    if (std::ranges::find(VertexFileExtensions, extension) != VertexFileExtensions.end())
        return ShaderPipeline::Vertex;

    if (std::ranges::find(FragmentFileExtensions, extension) != FragmentFileExtensions.end())
        return ShaderPipeline::Fragment;
    
    if (std::ranges::find(GeometryFileExtensions, extension) != GeometryFileExtensions.end())
        return ShaderPipeline::Geometry;

    Logger::LogError("Invalid file extension for shader : {}", extension);
    throw std::invalid_argument("Invalid file extension for shader");
}

bool_t Shader::Load(const Pointer<File>& shader)
{
    const ShaderPipeline::ShaderPipeline type = FileExtensionToType(shader->GetExtension());

    if (!Load(shader->GetData(), shader->GetSize(), type))
        return false;

    m_Files[static_cast<size_t>(type)] = shader;
    
    m_Loaded = true;
    
    return true;
}

bool_t Shader::Load(const char_t* const buffer, const int64_t length, const ShaderPipeline::ShaderPipeline type)
{
    ShaderCode& code = m_Code[static_cast<size_t>(type)];
    code.code = buffer;
    code.codeLength = static_cast<int32_t>(length);
    code.type = ShaderPipelineToShaderType(type);
    
    m_Loaded = true;
    
    return true;
}

void Shader::CreateInInterface()
{
    std::vector<ShaderCode> code(m_Code.size());
    std::ranges::copy(m_Code, code.begin());
    
    m_Id = Rhi::CreateShaders(code, { m_DepthFunction, m_BlendFunction, m_ShaderProgramCullInfo });
    m_LoadedInInterface = true;
}

void Shader::DestroyInInterface()
{
    Rhi::DestroyProgram(m_Id);
    
    m_Id = 0;
    m_LoadedInInterface = false;
}

void Shader::Recompile()
{
    for (Pointer<File>& file : m_Files)
    {
        if (!file)
            continue;
        
        file->Reload();
        Load(file->GetData(), file->GetSize(), FileExtensionToType(file->GetExtension()));
    }

    std::vector<ShaderCode> code(m_Code.size());
    std::ranges::copy(m_Code, code.begin());
    m_Id = Rhi::ReloadProgram(m_Id, code);
}

void Shader::Unload()
{
    m_Files.fill(nullptr);
    m_Code.fill({});
}

void Shader::SetInt(const std::string& keyName, const int32_t value) const
{
    Rhi::SetUniform(UniformType::Int, &value, m_Id, keyName.c_str());
}

void Shader::SetBool(const std::string& keyName, const bool_t value) const
{
    Rhi::SetUniform(UniformType::Bool, &value, m_Id, keyName.c_str());
}

void Shader::SetFloat(const std::string& keyName, const float_t value) const
{
    Rhi::SetUniform(UniformType::Float, &value, m_Id, keyName.c_str());
}

void Shader::SetVec3(const std::string& keyName, const Vector3& value) const
{
    Rhi::SetUniform(UniformType::Vec3, &value, m_Id, keyName.c_str());
}
void Shader::SetVec2(const std::string& keyName, const Vector2& value) const
{
    Rhi::SetUniform(UniformType::Vec2, &value, m_Id, keyName.c_str());
}

void Shader::SetVec4(const std::string& keyName, const Vector4& value) const
{
    Rhi::SetUniform(UniformType::Vec4, &value, m_Id, keyName.c_str());
}

void Shader::SetMat4(const std::string& keyName, const Matrix& value) const
{
    Rhi::SetUniform(UniformType::Mat4, &value, m_Id, keyName.c_str());
}

uint32_t Shader::GetId() const
{
    return m_Id;
}

void Shader::Use() const
{
    Rhi::UseShader(m_Id);
}

void Shader::Unuse() const
{
    Rhi::UnuseShader();
}

void Shader::SetDepthFunction(const DepthFunction::DepthFunction depthFunction)
{
    if (m_LoadedInInterface)
    {
        Logger::LogError("Can't modify depth function in already loaded shader\n Shader id = {}",m_Id);
        return;
    }
    
    m_DepthFunction = depthFunction;
}

void Shader::SetBlendFunction(const BlendFunction& blendFunction)
{
    if (m_LoadedInInterface)
    {
        Logger::LogError("Can't modify blend function in already loaded shader\n Shader id = {}",m_Id);
        return;
    }
    
    m_BlendFunction = blendFunction;
}

void Shader::SetFaceCullingInfo(const ShaderProgramCullInfo& shaderProgramCullInfo)
{
    if (m_LoadedInInterface)
    {
        Logger::LogError("Can't modify blend function in already loaded shader\n Shader id = {}",m_Id);
        return;
    }
    
    
    m_ShaderProgramCullInfo = shaderProgramCullInfo;
}
