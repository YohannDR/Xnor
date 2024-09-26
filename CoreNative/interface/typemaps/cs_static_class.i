// The %cs_static_class macro is used to create a C# static class
// (one with only static members)
// This macro must be used BEFORE including the C++ file.
// This effectively removes the default base class (IDisposable),
// its implementation, and any non-static, auto-generated members.

%define %cs_static_class(TYPE)
    %typemap(csbase) TYPE "";
    %typemap(csinterfaces) TYPE "";
    %typemap(csbody) TYPE %{%}
    %typemap(csdispose) TYPE %{%}
    %typemap(csdisposing) TYPE %{%}
    %typemap(csclassmodifiers) TYPE "public static class";
%enddef
