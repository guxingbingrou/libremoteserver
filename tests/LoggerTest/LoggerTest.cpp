#include "Logger/Logger.h"
int main(){
	SimLogger::Logger::CreateLoggerInstance();
	SimLogger::Logger::InitLogger(SimLogger::ConsoleType, "LoggerTest", SimLogger::Info);
	SIMLOG(SimLogger::Debug, "this is the first test: " << 1);


}
