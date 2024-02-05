#define _CRT_SECURE_NO_WARNINGS
#include "Logger.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <chrono>

namespace ent {
	namespace util {
		Logger* Logger::_instance = nullptr;

		Logger* Logger::getInstance() {
			if (_instance == nullptr) {
				_instance = new Logger();
			}
			return _instance;
		}

		void Logger::setLogToConsole(bool state) {
			logToConsole = state;
		}

		void Logger::setLogToFile(bool state) {
			logToFile = state;
		}

		void Logger::setFilePath(std::string& path) {
			filePath = path;
		}

		void Logger::setLogLevel(level logLevel) {
			this->logLevel = logLevel;
		}

		void Logger::addLog(std::string msg, level logLevel) {
			// Check if msg level is acceptable
			if (logLevel <= this->logLevel) {

				// Defining level text to output
				std::string levelText;
				switch (logLevel) {
					case level::INFO:
						levelText = "[INFO]: ";
						break;
					case level::WARN:
						levelText = "[WARN]: ";
						break;
					case level::ERROR:
						levelText = "[ERROR]: ";
						break;
					case level::FATAL:
						levelText = "[FATAL]: ";
						break;
					case level::DEBUG:
						levelText = "[DEBUG]: ";
						break;
					default:
						break;
				}

				// Output
				if (logToConsole) {
					std::cout << "[" << getTime() + "] " + levelText + msg + "\n";
				}
				if (logToFile) {
					file << "[" << getTime() + "] " + levelText + msg + "\n";
				}
			}
		}

		std::string Logger::getTime() {
			auto now = std::chrono::system_clock::now();
			auto time_t_now = std::chrono::system_clock::to_time_t(now);
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

			auto tm = *std::localtime(&time_t_now);

			std::ostringstream oss;
			oss << std::put_time(&tm, "%H:%M:%S") << ":" << std::setfill('0') << std::setw(3) << ms.count();

			return oss.str();
		}

		std::string Logger::getDate() {
			auto t = std::time(nullptr);
			auto tm = *std::localtime(&t);

			std::ostringstream oss;
			oss << std::put_time(&tm, "%d-%m-%Y");

			return oss.str();
		}

		Logger::Logger() {
			logToConsole = true;
			logToFile = true;
			filePath = "assets/logs/" + getDate() + ".txt";
			file.open(filePath, std::ios::app);
			logLevel = level::DEBUG;
		}

		Logger::~Logger() {
			file.close();
		}
	}
}