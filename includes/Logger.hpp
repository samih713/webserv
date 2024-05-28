#include "webserv.hpp"

#ifndef LOGGER_HPP
#define LOGGER_HPP

enum LogLevel {
    INFO = 0,
    DEBUG,
    ERROR
};

#define LOG_FILE "logfile.txt"

#define LOG_INFO(message) Logger::log_message(message, INFO)
#define LOG_DEBUG(message) Logger::log_message(message, DEBUG)
#define LOG_ERROR(message) Logger::log_message(message, ERROR)

class Logger {
public:
    static void log_message(const string& message, LogLevel level, bool toFile = false)
    {
        string logMessage = "[" + _get_current_time() + "]\t[" + _level_to_string(level) +
                            "]\t" + message;
        if (toFile) {
            ofstream _logFile(LOG_FILE, std::ios_base::app);
            if (!_logFile.is_open())
                THROW_EXCEPTION_WITH_INFO("Log: could not open log file");

            _logFile << logMessage << endl;
            _logFile.close();
        }
        else
            cout << logMessage << endl;
    }

private:
    static LogLevel _level;

    static string _get_current_time()
    {
        time_t     rawtime;
        struct tm* timeInfo;
        char       buffer[80];

        time(&rawtime);
        timeInfo = localtime(&rawtime);

        strftime(buffer, sizeof(buffer), "%Y-%m-%d  %H:%M:%S", timeInfo);
        return string(buffer);
    }
    static string _level_to_string(LogLevel level)
    {
        switch (level) {
            case INFO:  return string(B "INFO" RE);
            case DEBUG: return string(M "DEBUG" RE);
            case ERROR: return string(R "ERROR" RE);
            default:    return string(W "NULL" RE);
        }
    }

    Logger();
    ~Logger();
};

#endif // LOGGER_HPP
