#include "webserv.hpp"

#ifndef LOGGER_HPP
#define LOGGER_HPP

static string get_current_time(void)
{
    char   buffer[80];
    time_t rawtime = time(NULL);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d  %H:%M:%S", localtime(&rawtime));
    return string(buffer);
}

#if defined(__LOG_TO_FILE__)
#define LOG_FILE           "server_log.txt"
#define LOG_INFO(message)  write_log(message, "INFO")
#define LOG_ERROR(message) write_log(message, "ERROR")
#if defined(__DEBUG__)
#define LOG_DEBUG(message) write_log(message, "DEBUG")
#else
#define LOG_DEBUG(message)
#endif
inline void write_log(const string& message, const string& level)
{
    ofstream _logFile(LOG_FILE, std::ios_base::app);
    if (!_logFile.is_open())
        THROW_EXCEPTION_WITH_INFO("Log: could not open log file");

    string logMessage = "[" + get_current_time() + "]\t";
    logMessage += "[" + level + "]\t";
    logMessage += message;

    _logFile << logMessage << endl;
    _logFile.close();
}
#else
#define LOG_INFO(message)  write_log(message, B + string("INFO") + RE)
#define LOG_ERROR(message) write_log(message, R + string("ERROR") + RE)
#if defined(__DEBUG__)
#define LOG_DEBUG(message) write_log(message, M + string("DEBUG") + RE)
#else
#define LOG_DEBUG(message)
#endif
inline void write_log(const string& message, const string& level)
{
    string logMessage = "[" + get_current_time() + "]\t";
    logMessage += "[" + level + "]\t";
    logMessage += message;
    cout << logMessage << endl;
}
#endif

#endif // LOGGER_HPP
