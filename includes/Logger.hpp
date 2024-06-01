#include "webserv.hpp"

#ifndef LOGGER_HPP
#define LOGGER_HPP

// LOG_DEBUG should only be used if __DEBUG__ is defined
#if defined(__LOG_TO_FILE__)
#define LOG_FILE           "server_log.txt"
#define LOG_INFO(message)  Logger::log_message(message, "INFO", true)
#define LOG_DEBUG(message) Logger::log_message(message, "DEBUG", true)
#define LOG_ERROR(message) Logger::log_message(message, "ERROR", true)
static void writeLog(const string& logMessage)
{
    ofstream _logFile(LOG_FILE, std::ios_base::app);
    if (!_logFile.is_open())
        THROW_EXCEPTION_WITH_INFO("Log: could not open log file");

    _logFile << logMessage << endl;
    _logFile.close();
}
#else
#define LOG_INFO(message)  Logger::log_message(message, "INFO")
#define LOG_DEBUG(message) Logger::log_message(message, "DEBUG")
#define LOG_ERROR(message) Logger::log_message(message, "ERROR")

static void writeLog(const string& logMessage)
{
    cout << logMessage << endl;
}
#endif

class Logger {
public:
    static void log_message(const string& message, const string& level,
        bool toFile = false)
    {
        string logMessage = "[" + get_current_time() + "]\t[";

        if (level == "INFO" && !toFile)
            logMessage += B + level + RE;
        else if (level == "DEBUG" && !toFile)
            logMessage += M + level + RE;
        else if (level == "ERROR" && !toFile)
            logMessage += R + level + RE;
        else
            logMessage += level;

        logMessage += "]\t" + message;
        writeLog(logMessage);
    }

private:
    static string get_current_time()
    {
        time_t     rawtime;
        struct tm* timeInfo;
        char       buffer[80];

        time(&rawtime);
        timeInfo = localtime(&rawtime);

        strftime(buffer, sizeof(buffer), "%Y-%m-%d  %H:%M:%S", timeInfo);
        return string(buffer);
    }

    Logger();
    ~Logger();
};

#endif // LOGGER_HPP
