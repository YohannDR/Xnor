#include "editor.hpp"
#include "reflection/dotnet_reflection.hpp"
#include "reflection/xnor_factory.hpp"
#include "utils/logger.hpp"

// These global definitions make OpenGL use the dedicated GPU instead of the integrated one
// Source: https://www.reddit.com/r/opengl/comments/unc3fy/how_to_programatically_set_the_gpu_to_my_opengl/?onetap_auto=true&one_tap=true 
extern "C"
{
	_declspec(dllexport) uint32_t NvOptimusEnablement = 1;
	_declspec(dllexport) int32_t AmdPowerXpressRequestHighPerformance = 1;
}

int32_t main(const int32_t argc, const char_t* const* const argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();
	XnorFactory::RegisterAllTypes();
	DotnetReflection::RegisterAllTypes();

	Editor editor(FORWARD(argc), FORWARD(argv));

	editor.Update();

	return EXIT_SUCCESS;
}
