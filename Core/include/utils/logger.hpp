﻿#pragma once

#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <thread>

#include "core.hpp"
#include "csharp/dotnet_constants.hpp"
#include "utils/concepts.hpp"
#include "utils/ts_queue.hpp"

/// @file logger.hpp
/// @brief Defines the XnorCore::Logger static class

BEGIN_XNOR_CORE

/// @brief Logs a @ref XnorCore::Logger::LogLevel::TemporaryDebug "temporary debug message" using the given format message and arguments.
#define DEBUG_LOG(message, ...) XnorCore::Logger::LogTempDebug(message, __FILE__, __LINE__, __VA_ARGS__)

/// @brief Static class used to log messages to the console and/or a file.
///
/// ### Requirements
/// Thread-safe logger that starts logging once @c Logger::Start() gets called, which is already done in @c Application::Application(). You can synchronize the calling thread
/// with the logger one at any time by calling @c Logger::Synchronize. The logger needs to be stopped by calling @c Logger::Stop() gets called, which is also already done in @c Application::~Application().
///
/// ### Options
/// By default, the logger doesn't log to a file. This can be changed by either calling @c Logger::OpenDefaultFile or @c Logger::OpenFile.
/// You can also stop logging to the file whenever you want by calling @c Logger::CloseFile.
/// 
/// You can change at any time the minimum LogLevel for either the console or the file by respectively setting @c Logger::minimumConsoleLevel or @c Logger::minimumFileLevel
/// to a different value.
///
/// ### Usage
/// The most generic way of logging is by using the @c Logger::Log function, which allows you to pass a LogLevel to describe the severity
/// of the log. Shortcuts are also available through the use of @c Logger::LogTempDebug, @c Logger::LogDebug, @c Logger::LogInfo, @c Logger::LogWarning, @c Logger::LogError and @c Logger::LogFatal.
/// Those functions take a format string and format parameters to follow the usage of <a href="https://en.cppreference.com/w/cpp/utility/format/format">std::format</a>.
/// This means that any new parameter type that is directly printed must satisfy the requirements of the <a href="https://en.cppreference.com/w/cpp/utility/format/formattable">std::formattable</a>
/// concept (defined a Concepts::Formattable in the XnorCore namespace), and therefore needs to implement its own version of the <a href="https://en.cppreference.com/w/cpp/utility/format/formatter">std::formatter</a> struct.
///
/// ### Example
/// All logs are preceded by their timestamp (the exact time at which the @c Logger::Log function was called), and a string representation of their LogLevel.
/// A typical log looks like the following:
/// @code
/// [11:26:05.751] [INFO] Starting logging to file.
/// @endcode
/// Also, @ref Logger::LogLevel::TemporaryDebug "temporary debug logs" automatically specify the file and line at which the log was made.
class Logger final
{
    STATIC_CLASS(Logger)

public:
    /// @brief Describes the severity of a log.
    enum class LogLevel
    {
        /// @brief Log intended for temporary debugging only.
        ///
        /// Preceded by '[TEMP DEBUG]' and appears green in the console.
        /// Temporary debug logs are not printed in the log file by default, and they are only printed in the console if in a debug build.
        TemporaryDebug,
        /// @brief Log intended for debugging only.
        ///
        /// Preceded by '[DEBUG]' and appears gray in the console.
        /// Debug logs are not printed in the log file by default, and they are only printed in the console if in a debug build.
        Debug,
        /// @brief Log intended for general information.
        ///
        /// Preceded by '[INFO]' and appears white in the console.
        /// Info logs are not printed in the log file by default.
        Info,
        /// @brief Log intended for warnings.
        ///
        /// Preceded by '[WARN]' and appears orange in the console.
        Warning,
        /// @brief Log intended for errors.
        ///
        /// Preceded by '[ERROR]' and appears red in the console.
        Error,
        /// @brief Log intended for fatal errors.
        ///
        /// Preceded by '[FATAL]' and appears red and bold in the console.
        /// After such a log, the program is not intended to continue to function normally and should instead exit ASAP.
        Fatal
    };
    
private:
    struct LogEntry
    {
        std::string message;
        LogLevel level;
        std::chrono::system_clock::time_point time;
        
        bool_t printToConsole, printToFile;
        
        std::string file;
        int32_t line = -1;
        
        bool_t sameAsPrevious = false;

        /// @brief Whether the log was made from a .NET call
        bool_t fromDotnet = false;

        std::shared_ptr<LogEntry> previousLog = nullptr;

        XNOR_ENGINE LogEntry();

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level);

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level, const std::string& file, int32_t line);

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level, std::chrono::system_clock::time_point timePoint);

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level, std::chrono::system_clock::duration duration);

        XNOR_ENGINE bool_t operator==(const LogEntry& other) const;
    };

    // We thought about using std::list here instead but because the allocations are made on the logger thread anyway we can make it a std::vector
    XNOR_ENGINE static inline std::vector<std::shared_ptr<LogEntry>> m_Logs;
    
public:
    /// @brief The minimum necessary LogLevel for a log to be printed in the console.
    ///
    /// Defaults to LogLevel::TemporaryDebug in a debug build, or LogLevel::Info otherwise.
    XNOR_ENGINE static inline LogLevel minimumConsoleLevel =
#ifdef _DEBUG
        LogLevel::TemporaryDebug;
#else
        LogLevel::Info;
#endif
    
    /// @brief The minimum necessary LogLevel for a log to be printed in the log file.
    ///
    /// Defaults to LogLevel::Info.
    XNOR_ENGINE static inline LogLevel minimumFileLevel = LogLevel::Info;

    /// @brief Logs a message using the specified format string, arguments and LogLevel.
    /// 
    /// @tparam Args The format arguments types. These are generally automatically deducted by the compiler and often don't need to be manually specified. Also, they must satisfy the Concepts::Formattable concept requirements.
    /// @param level The log severity.
    /// @param format The format string to log.
    /// @param args The arguments to replace the format string with.
    ///
    /// @see <a href="https://en.cppreference.com/w/cpp/utility/format/spec">The standard format specification</a>
    template <Concepts::FormattableT... Args>
    static void Log(LogLevel level, const std::string& format, Args&&... args);

    /// @brief Logs a temporary debug message using the current file, line, specified format string and arguments.
    ///
    /// This function shouldn't be used directly. To print a temporary debug log message, instead use DEBUG_LOG.
    ///
    /// @see Log
    /// @see LogLevel::TemporaryDebug
    template <Concepts::FormattableT... Args>
    static void LogTempDebug(const std::string& format, const char_t* file, int32_t line, Args&&... args);

    /// @brief Logs a debug message using the specified format string and arguments.
    ///
    /// @see Log
    template <Concepts::FormattableT... Args>
    static void LogDebug(const std::string& format, Args&&... args);

    /// @brief Logs a information message using the specified format string and arguments.
    ///
    /// @see Log
    template <Concepts::FormattableT... Args>
    static void LogInfo(const std::string& format, Args&&... args);

    /// @brief Logs a warning message using the specified format string and arguments.
    ///
    /// @see Log
    template <Concepts::FormattableT... Args>
    static void LogWarning(const std::string& format, Args&&... args);

    /// @brief Logs an error message using the specified format string and arguments.
    ///
    /// @see Log
    template <Concepts::FormattableT... Args>
    static void LogError(const std::string& format, Args&&... args);

    /// @brief Logs a fatal error message using the specified format string and arguments.
    ///
    /// @see Log
    template <Concepts::FormattableT... Args>
    static void LogFatal(const std::string& format, Args&&... args);

    /// @brief Opens a file for logging.
    ///
    /// If a file is already open for logging e.g. if Logger::HasFileOpen returns @c true, this overwrites it with the new file.
    ///
    /// @param filepath The file to open and to log into.
    XNOR_ENGINE static void OpenFile(const std::filesystem::path& filepath);

    /// @brief Opens the default log file.
    ///
    /// The default log file is an autogenerated one with a name corresponding to the current date and the @c .log file extension.
    /// An example of a default log file: @c 2024-02-13.log.
    /// If this is called multiple times in a day (by launching the program multiple times during the same day for example), this appends to the
    /// already existing log file.
    XNOR_ENGINE static void OpenDefaultFile();

    /// @brief Returns whether the logger is already log to a file.
    XNOR_ENGINE static bool HasFileOpen();

    /// @brief Closes the current log file.
    XNOR_ENGINE static void CloseFile();

    /// @brief Synchronizes the calling thread with the logger one, and makes sure all logs have been printed before returning.
    XNOR_ENGINE static void Synchronize();

    /// @brief Starts the logger.
    ///
    /// This function is called automatically when the Application is constructed.
    /// After a call to this function, you can use the Log functions.
    ///
    /// This function doesn't do anything if the logger has already been started.
    XNOR_ENGINE static void Start();

    /// @brief Synchronizes the threads and stops the logger.
    ///
    /// This function is called automatically when the Application is destroyed.
    /// After a call to this function, logger function calls won't do anything.
    ///
    /// This function doesn't do anything if the logger has already been stopped.
    XNOR_ENGINE static void Stop();

    XNOR_ENGINE static const decltype(m_Logs)& GetLogList();

    XNOR_ENGINE static void Clear();

private:
    XNOR_ENGINE static inline TsQueue<std::shared_ptr<LogEntry>> m_NewLogs;

    XNOR_ENGINE static inline std::shared_ptr<LogEntry> m_LastLog;

    XNOR_ENGINE static inline bool_t m_LastLogCollapsed = false;

    XNOR_ENGINE static inline std::condition_variable m_CondVar;

    XNOR_ENGINE static void Run();

    XNOR_ENGINE static inline std::thread m_Thread;

    XNOR_ENGINE static inline std::mutex m_Mutex;

    XNOR_ENGINE static inline bool_t m_Running = false;

    XNOR_ENGINE static inline bool_t m_Synchronizing = false;

    XNOR_ENGINE static inline std::ofstream m_File;

    XNOR_ENGINE static inline std::filesystem::path m_Filepath;

    XNOR_ENGINE static inline uint32_t m_LogIndex = 0;

    XNOR_ENGINE static inline const std::string DotnetLogPrefix = "[" + std::string(Dotnet::GameProjectName) + "] ";

    XNOR_ENGINE static void PushLog(const std::shared_ptr<LogEntry>& log);

    /// @brief Prints a log to the console and the logging file.
    XNOR_ENGINE static void PrintLog(const std::shared_ptr<LogEntry>& log);

    /// @brief Builds the given log's prefix. Returns the [prefix, color] pair.
    XNOR_ENGINE static std::pair<std::string, const char_t*> BuildLogPrefix(const std::shared_ptr<LogEntry>& log);
};

END_XNOR_CORE

#include "utils/logger.inl"
