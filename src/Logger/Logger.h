#pragma once
#include <string>
#include <iostream>
#include <fstream>

namespace ent {
	// Utility namespace
	namespace util {
		// Log levels
		enum class level {
			INFO,
			WARN,
			ERROR,
			FATAL,
			DEBUG

		};

		// Singleton instance logger. Logs to cmd or/and file
		class Logger {
		public:
			// Returns current instance to the main object, or creates it, if none exists
			static Logger* getInstance();
			// Enable or disable logging to console
			void setLogToConsole(bool state);
			// Enable or disable logging to file
			void setLogToFile(bool state);
			// Set log filepath. (By default it's "assets/logs/DATE.txt")
			void setFilePath(std::string& path);

			// Sets log level up to desired
			// @param level: INFO, WARN, ERROR, FATAL, DEBUG
			void setLogLevel(level logLevel);

			// Add log message with different levels
			void addLog(std::string msg, level logLevel);


			~Logger();
		private:
			// Output bools
			bool logToConsole;
			bool logToFile;

			// log level
			level logLevel;

			// Filepath
			std::string filePath;
			std::ofstream file;

			// Returns current time
			std::string getTime();
			// Returns current date
			std::string getDate();

			// Singleton
			Logger();
			static Logger* _instance;
		};

	}
}