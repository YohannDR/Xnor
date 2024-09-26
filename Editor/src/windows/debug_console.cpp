#include "windows/debug_console.hpp"

#include "magic_enum/magic_enum.hpp"

using namespace XnorEditor;

DebugConsole::DebugConsole(Editor* editor)
    : UiWindow(editor, "Console")
{
}

void DebugConsole::Display()
{
    DisplayHeader();
    DisplayLogs();
}

int scroll;
void DebugConsole::DisplayHeader()
{
    scroll = 0;
    if (ImGui::Button("Scroll up"))
        scroll = -1;
    ImGui::SameLine();
    if (ImGui::Button("Scroll down") || XnorCore::Logger::GetLogList().size() > m_CachedLogs.size())
        scroll = 1;
    ImGui::SameLine();
    if (ImGui::Button("Clear"))
    {
        XnorCore::Logger::Clear();
        m_CachedLogs.clear();
    }
}

void DebugConsole::DisplayLogs()
{
    if (!ImGui::BeginTable("Logs", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
        return;

    // This will only be executed the first time the control flow reaches here
    static auto _ = [this] { return scroll = 1; }();

    auto&& logList = XnorCore::Logger::GetLogList();
    
    if (scroll == -1)
        ImGui::SetScrollHereY(0.f);
    
    ImGui::TableSetupColumn("Level");
    ImGui::TableSetupColumn("Timestamp");
    ImGui::TableSetupColumn("Message");
    ImGui::TableSetupScrollFreeze(0, 1);

    // Custom header for ComboBox in the Level header
    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

    // Level header
    ImGui::TableSetColumnIndex(0);
    ImGui::PushID(0);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{});
    if (ImGui::BeginCombo("##level", magic_enum::enum_name(minimumLogLevel).data()))
    {
        for (auto&& level : magic_enum::enum_values<XnorCore::Logger::LogLevel>())
        {
            if (ImGui::Selectable(magic_enum::enum_name(level).data()))
            {
                minimumLogLevel = level;
                scroll = 1;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopStyleVar();
    ImGui::SameLine();
    XnorCore::Logger::Synchronize();
    ImGui::Text("(%llu)", std::ranges::count_if(logList, [this] (auto&& val) -> bool_t { return val->level >= minimumLogLevel; }));
    ImGui::PopID();

    // Timestamp header
    ImGui::TableSetColumnIndex(1);
    ImGui::PushID(1);
    ImGui::TableHeader(ImGui::TableGetColumnName(1)); // Retrieve name passed to TableSetupColumn()
    ImGui::PopID();

    // Message header
    ImGui::TableSetColumnIndex(2);
    ImGui::PushID(2);
    ImGui::TableHeader(ImGui::TableGetColumnName(2)); // Retrieve name passed to TableSetupColumn()
    ImGui::PopID();
    
    for (size_t i = m_CachedLogs.size(); i < logList.size(); i++)
    {
        const auto& log = logList[i];
        
        const XnorCore::Logger::LogLevel level = log->level;

        static uint64_t sameLastLogs;

        if (log->sameAsPrevious)
            sameLastLogs++;
        else
            sameLastLogs = 0;

        std::string message;

        XnorCore::Color color;
        std::string prefix;
        switch (level)
        {
            case XnorCore::Logger::LogLevel::TemporaryDebug:
                color = XnorCore::Color::LightGreen();
                prefix = "Temporary Debug";
                message = log->file + '(' + std::to_string(log->line) + "): ";
                break;
                
            case XnorCore::Logger::LogLevel::Debug:
                color = XnorCore::Color::DarkGray();
                prefix = "Debug";
                break;
                
            case XnorCore::Logger::LogLevel::Info:
                color = XnorCore::Color::White();
                prefix = "Info";
                break;
                
            case XnorCore::Logger::LogLevel::Warning:
                color = XnorCore::Color::Orange();
                prefix = "Warning";
                break;
                
            case XnorCore::Logger::LogLevel::Error:
                color = XnorCore::Color::Red();
                prefix = "Error";
                break;
                
            case XnorCore::Logger::LogLevel::Fatal:
                color = XnorCore::Color::DarkRed();
                prefix = "Fatal";
                break;
        }

        if (sameLastLogs > 0)
            message = std::format("[...and {} more]", sameLastLogs);
        else
            message += log->message;

        const ImVec4 col = static_cast<Vector4>(color);
        
        // Get the message time and format it in [hh:mm:ss:ms]
        const auto&& t = std::chrono::duration_cast<std::chrono::milliseconds, int64_t>(log->time.time_since_epoch());
        std::string time = std::format("{:%T}", t);

        m_CachedLogs.push_back({ col, level, std::move(prefix), std::move(time), std::move(message), sameLastLogs });
    }

    for (size_t i = 0; i < m_CachedLogs.size(); i++)
    {
        const LogData& log = m_CachedLogs[i];
        
        if (log.level < minimumLogLevel)
            continue;

        if (log.sameLastLogs > 0 && i + 1 != logList.size())
            continue;

        if (i > 0)
        {
            const LogData& previousLog = m_CachedLogs[i - 1];
            if (previousLog.sameLastLogs > 0 && log.sameLastLogs == 0)
                DisplayLog(previousLog);
        }

        DisplayLog(log);
    }

    if (scroll == 1)
        ImGui::SetScrollHereY(1.f);

    ImGui::EndTable();
}

void DebugConsole::DisplayLog(const LogData& data)
{
    ImGui::TableNextColumn();
    ImGui::TextColored(data.color, "%s", data.prefix.c_str());
        
    ImGui::TableNextColumn();
    ImGui::TextColored(data.color, "%s", data.time.c_str());
        
    ImGui::TableNextColumn();
    ImGui::PushTextWrapPos();
    ImGui::TextColored(data.color, "%s", data.message.c_str());
    ImGui::PopTextWrapPos();
}
