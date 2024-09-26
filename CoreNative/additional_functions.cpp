#define EXPORT __declspec(dllexport)
#define STDCALL __stdcall

#include "csharp/dotnet_utils.hpp"
#include "scene/component.hpp"

/* Callback for returning strings to C# without leaking memory */
typedef char * (STDCALL* CSharpStringHelperCallback)(const char *);
static CSharpStringHelperCallback csharp_string_callback = NULL;

using namespace XnorCore;

extern "C"
{
    EXPORT void STDCALL RegisterStringCallback_Core(CSharpStringHelperCallback callback)
    {
        csharp_string_callback = callback;
    }

    EXPORT char* STDCALL NativeComponentListGetItem(const List<Component*>& list, size_t index, Component** item)
    {
        std::string temp;
        *item = const_cast<Component*>(&DotnetUtils::ComponentListGetItem(list, index, &temp));
        return csharp_string_callback(temp.c_str());
    }
}
