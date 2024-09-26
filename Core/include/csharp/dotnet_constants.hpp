#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

namespace Dotnet
{
    constexpr const char_t* const GameProjectName = "ExampleGame";
    
    constexpr const char_t* const AssembliesDirectory = "DotnetAssemblies";
    
    constexpr const char_t* const CoreProjectName = "CoreManaged";
    
    constexpr const char_t* const CoreProjectLocation = CoreProjectName;
    
    constexpr const char_t* const GameProjectLocation = GameProjectName;
    
    constexpr const char_t* const GameProjectBuildOptions = "--no-self-contained";

    /// @brief The XNOR Engine Core .NET default namespace.
    const std::string XnorCoreNamespace = "Xnor.Core";
}

END_XNOR_CORE
