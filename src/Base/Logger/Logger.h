/*
 * Logger.h
 *
 *  Created on: Apr 4, 2020
 *      Author: zou
 */

#ifndef SRC_LOGGER_LOGGER_H_
#define SRC_LOGGER_LOGGER_H_
#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <memory>
#define SIMLOG(logLevel,logEvent)                                                                               \
    do {                                                                                                        \
        if (!SimLogger::Logger::Bequiet() && logLevel >= SimLogger::Logger::GetLogLevel()) {  			                            \
        	SimLogger::Logger::start(logLevel, __LINE__, __FUNCTION__) << logEvent <<std::endl << std::flush;               \
		}                                                                                                       \
    } while (0)
namespace SimLogger {

enum LogLevel{
	Trace,
	Debug,
	Info,
	Warning,
	Error
};
enum LogType{
	ConsoleType,
	FileType
};
class Logger {
public:
	static void CreateLoggerInstance();
	static void InitLogger(const LogType type, const std::string &name, const LogLevel level);
	static std::ostream& start(const LogLevel level, const int line, const std::string &function);
	inline static LogLevel GetLogLevel(){return logger_level_;}
	inline static bool Bequiet(){return be_quiet_;}
	inline static void Setquiet(bool quiet){be_quiet_ = quiet;}
	Logger();
	virtual ~Logger();

private:

	static std::ostream& GetOstream();

	static std::string logger_name_;
	static LogLevel logger_level_;
	static std::ofstream file_stream_;
	static LogType logger_type_;
	static std::mutex mutex_;
	static std::shared_ptr<Logger> logger_instance_;
	static bool be_quiet_;

};

} /* namespace SimLogger */

#endif /* SRC_LOGGER_LOGGER_H_ */
