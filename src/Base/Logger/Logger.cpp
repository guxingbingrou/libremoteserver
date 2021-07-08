/*
 * Logger.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: zou
 */

#include "Logger.h"

#include <ctime>
namespace SimLogger {
LogLevel Logger::logger_level_ = Info;
LogType Logger::logger_type_ = ConsoleType;
std::ofstream Logger::file_stream_;
std::string Logger::logger_name_;
std::shared_ptr<Logger> Logger::logger_instance_;
std::mutex Logger::mutex_;
bool Logger::be_quiet_ = false;
void Logger::CreateLoggerInstance(){
	if(!logger_instance_ ){
		std::lock_guard<std::mutex> lock(mutex_);
		if(!logger_instance_)
			logger_instance_ = std::make_shared<Logger>();
	}
}
void Logger::InitLogger(const LogType type, const std::string &name, const LogLevel level){
	logger_name_ = name;
	logger_level_ = level;
	logger_type_ = type;
	if(FileType == logger_type_)
		file_stream_.open(logger_name_, std::ios::out | std::ios::app);
}
std::ostream& Logger::start(const LogLevel level, const int line, const std::string &function){
	time_t timep;
	time(&timep);
	struct tm *p;
	p = localtime(&timep); /*转换为struct tm结构的当地时间*/
	std::string str_flg;
	switch(level){
		case Trace: str_flg = "<Trace> ";
			break;
		case Debug: str_flg = "<Debug> ";
			break;
		case Info: str_flg = "<Info> ";
			break;
		case Warning: str_flg = "<Warning> ";
			break;
		case Error: str_flg = "<Error> ";
			break;
		default:
			break;
	}

	return GetOstream() << str_flg << 1900 + p->tm_year << "/" << 1+ p->tm_mon << "/" << p->tm_mday << "  "
			<< p->tm_hour << ":" << p->tm_min << ":" << p->tm_sec
			<< " function ("<< function << ") " << " line :" << line << "  " <<  std::flush;
}
std::ostream& Logger::GetOstream(){
	switch (logger_type_){
		case ConsoleType:
			return std::cout;
		case FileType:
			return file_stream_;
		defalut:
			return std::cout;
	}
	return std::cout;
}
Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

} /* namespace SimLogger */
