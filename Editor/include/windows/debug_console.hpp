#pragma once

#include "definitions.hpp"
#include "utils/logger.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class DebugConsole : public UiWindow
{
public:
    XnorCore::Logger::LogLevel minimumLogLevel = XnorCore::Logger::LogLevel::Info;
    
    explicit DebugConsole(Editor* editor);

    void Display() override;

private:
    struct LogData
    {
        ImVec4 color;

        XnorCore::Logger::LogLevel level;
        
        std::string prefix;
        std::string time;
        std::string message;

        uint64_t sameLastLogs;
    };
    
    std::vector<LogData> m_CachedLogs;
    
    void DisplayHeader();

    void DisplayLogs();

    void DisplayLog(const LogData& data);
};

END_XNOR_EDITOR
