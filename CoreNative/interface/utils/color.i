%module CoreNative

%include "typemaps/cs_struct.i"

%ignore XnorCore::Color;
%ignore XnorCore::ColorHsva;

%cs_struct2(XnorCore::Color, Data16, Color);
%cs_struct2(XnorCore::ColorHsva, Data16, ColorHsva);

namespace XnorCore
{
    struct Color {};
    struct ColorHsva {};
}
