#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <iostream>

#define LOG(Logger_, Message_)                  \
  Logger_(                                      \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << Message_  \
    ).str(),                                    \
    __FUNCTION__,                               \
    __FILE__,                                   \
    __LINE__                                    \
  );

#define LOG_DEBUG(Message_) LOG(Logger::Debug(), Message_)
#define LOG_ERROR(Message_) LOG(Logger::Error(), Message_)

class Logger {
	public:
		enum Level {
			DEBUG, ERROR
		};

		Logger(Level l) {
			_level = l;
		}

		std::string levelToString(Level l) {
			switch (l) {
				case DEBUG:
					return "DEBUG";
				case ERROR:
					return "ERROR";
				default:
					return "???";
			}
		}

		void operator()(std::string const& message,
			char const* function,
			char const* file,
			int line) {
			std::cout << "[" << levelToString(_level) << "] " << file << ":" << line << " (" << function << ") - " << message << std::endl;
		}

		static Logger& Debug() {
			static Logger logger(DEBUG);
			return logger;
		}

		static Logger& Error() {
			static Logger logger(ERROR);
			return logger;
		}

	private:
		Level _level;
};

#endif
