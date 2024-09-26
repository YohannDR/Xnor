#include "csharp/dotnet_utils.hpp"

#include "csharp/dotnet_assembly.hpp"
#include "reflection/xnor_factory.hpp"

using namespace XnorCore;

const Component& DotnetUtils::ComponentListGetItem(const List<Component*>& list, const size_t index, std::string* managedTypeName)
{
    const Component* result = list[index];

    if (managedTypeName == nullptr)
        return *result;
    
    // Get type name from raw pointer using C++ reflection
    const std::string typeName = XnorFactory::GetTypeName(Utils::GetTypeHash<Component>(result));

    // Convert these names to their C# type equivalent
    auto&& dotnetAssembly = DotnetAssembly::xnorCoreAssembly->GetCoralAssembly();

    *managedTypeName = dotnetAssembly->GetType(Dotnet::XnorCoreNamespace + '.' + typeName).GetFullName();
    
    return *result;
}
