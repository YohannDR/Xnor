%module CoreNative

%include "typemaps/cs_static_class.i"

%csmethodmodifiers XnorCore::Logger::Log<> "private";
%csmethodmodifiers XnorCore::Logger::LogTempDebug<> "private";
%csmethodmodifiers XnorCore::Logger::LogDebug<> "private";
%csmethodmodifiers XnorCore::Logger::LogInfo<> "private";
%csmethodmodifiers XnorCore::Logger::LogWarning<> "private";
%csmethodmodifiers XnorCore::Logger::LogError<> "private";
%csmethodmodifiers XnorCore::Logger::LogFatal<> "private";

%ignore XnorCore::Logger::Start;
%ignore XnorCore::Logger::Stop;
%ignore XnorCore::Logger::GetLogList;

%cs_static_class(XnorCore::Logger);

%typemap(cscode) XnorCore::Logger
%{
    public static void Log(Logger.LogLevel level, string format, params object[] args)
        => LogString(level, "[" + global::System.Reflection.Assembly.GetCallingAssembly().GetName().Name + "] " + string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));

    public static void LogTempDebug(string message, [global::System.Runtime.CompilerServices.CallerFilePath] string file = "", [global::System.Runtime.CompilerServices.CallerLineNumber] int line = 0)
        => LogTempDebugString("[" + global::System.Reflection.Assembly.GetCallingAssembly().GetName().Name + "] " + message, file, line);

    public static void LogDebug(string format, params object[] args)
        => LogDebugString("[" + global::System.Reflection.Assembly.GetCallingAssembly().GetName().Name + "] " + string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));

    public static void LogInfo(string format, params object[] args)
        => LogInfoString("[" + global::System.Reflection.Assembly.GetCallingAssembly().GetName().Name + "] " + string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));

    public static void LogWarning(string format, params object[] args)
        => LogWarningString("[" + global::System.Reflection.Assembly.GetCallingAssembly().GetName().Name + "] " + string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));

    public static void LogError(string format, params object[] args)
        => LogErrorString("[" + global::System.Reflection.Assembly.GetCallingAssembly().GetName().Name + "] " + string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));

    public static void LogFatal(string format, params object[] args)
        => LogFatalString("[" + global::System.Reflection.Assembly.GetCallingAssembly().GetName().Name + "] " + string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
%}

%include "utils/logger.hpp"

%template(LogString) XnorCore::Logger::Log<>;
%template(LogTempDebugString) XnorCore::Logger::LogTempDebug<>;
%template(LogDebugString) XnorCore::Logger::LogDebug<>;
%template(LogInfoString) XnorCore::Logger::LogInfo<>;
%template(LogWarningString) XnorCore::Logger::LogWarning<>;
%template(LogErrorString) XnorCore::Logger::LogError<>;
%template(LogFatalString) XnorCore::Logger::LogFatal<>;
