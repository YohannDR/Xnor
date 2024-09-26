%module CoreNative

%rename(GetFileType) XnorCore::File::GetType;

%include "file/file.hpp"
