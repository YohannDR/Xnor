#include "reflection/type_renderer.hpp"

#include "reflection/dotnet_reflection.hpp"
#include "reflection/xnor_factory.hpp"

using namespace XnorCore;

void TypeRenderer::DisplayObjectUsingFactory(void* const obj, const size_t hash, std::pair<void*, const char_t*>* const windowInfo)
{
    XnorFactory::DisplayObject(obj, hash, windowInfo);
}
