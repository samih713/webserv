#include "webserv.hpp"

#ifndef LOGGER_HPP
#define LOGGER_HPP

static void log_message(const string& message, const string& level);

#if defined(__LOG_TO_FILE__)
#define LOG_FILE           "server_log.txt"
#define LOG_INFO(message)  log_message(message, "INFO")
#define LOG_ERROR(message) log_message(message, "ERROR")
#if defined(__DEBUG__)
#define LOG_DEBUG(message) log_message(message, "DEBUG")
#else
#define LOG_DEBUG(message)
#endif
static void writeLog(const string& logMessage)
{
    ofstream _logFile(LOG_FILE, std::ios_base::app);
    if (!_logFile.is_open())
        THROW_EXCEPTION_WITH_INFO("Log: could not open log file");

    _logFile << logMessage << endl;
    _logFile.close();
}
#else
#define LOG_INFO(message)  log_message(message, B + string("INFO") + RE)
#define LOG_ERROR(message) log_message(message, R + string("ERROR") + RE)
#if defined(__DEBUG__)
#define LOG_DEBUG(message) log_message(message, M + string("DEBUG") + RE)
#else
#define LOG_DEBUG(message)
#endif
static void writeLog(const string& logMessage)
{
    cout << logMessage << endl;
}
#endif

static string get_current_time(void)
{
    time_t rawtime;
    char   buffer[80];
    time(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d  %H:%M:%S", localtime(&rawtime));
    return string(buffer);
}

static void log_message(const string& message, const string& level)
{
    string logMessage = "[" + get_current_time() + "]\t[" + level + "]\t" + message;
    writeLog(logMessage);
}

#endif // LOGGER_HPP
