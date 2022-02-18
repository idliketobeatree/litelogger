/**
 * Copyright (c) 2022 Ashley <stickacupcakeinmyeye@riseup.net>
 * https://github.com/stickacupcakeinmyeye/litelogger
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef LITELOGGER
#define LITELOGGER

#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <cstdlib>

#include <functional>

#define DOUBLE_SEPARATOR        "================================================================================"
#define HASHTAG_SEPARATOR       "################################################################################"
#define SMALL_SEPARATOR         "----------------------------------------------------------------"
#define SUPER_SMALL_SEPARATOR   "------------------------------------------------"

namespace litelogger {
	inline int8_t logLevel = INT8_MIN; // print all messages by default. use `changeLevel` to change this

	struct Logger {
		const char *name;
		const char *format; // %s logger name %s time
		const int8_t level; // only print message if `level` >= `logLevel`
		FILE *stream; // used only if not specified

		inline Logger(const char *name, const int8_t level, FILE *stream): 
			name(name), format("[\033[1;96m%s\033[0m %s]: "), level(level), stream(stream) {}
		inline Logger(const char *name, const char *format, const int8_t level, FILE *stream):
			name(name), format(format), level(level), stream(stream) {}
	};

	inline Logger DEBUG("Debug", "[\033[1;95m%s\033[0m %s]: ", -1, stdout);
	inline Logger INFO("Info", "[\033[1;96m%s\033[0m %s]: ", 0, stdout);
	inline Logger WARN("Warn", "[\033[1;93m%s\033[0m %s]: ", 1, stdout);
	inline Logger ERROR("Error", "[\033[1;91m%s\033[0m %s]: ", 127, stderr);

	/// change the log level. loggers whose levels are below this will not print
	inline void changeLevel(int8_t to) {
		logLevel = to;
	}

	/// flush `stream`, useful if you want to print something without putting a newline
	inline void flush(FILE *stream) {
		fflush(stream);
	}
	/// flush default stream, useful if you want to print something without putting a newline
	inline void flush(const Logger &logger) {
		flush(logger.stream);
	}

	inline void log(FILE *stream, const Logger &logger, const char *format, va_list args) {
		if(logger.level >= logLevel) {
			char timeString[9];
			{
				time_t t;
				time(&t);
				strftime(timeString, 9, "%H:%M:%S", localtime(&t));
			}
			fprintf(stream, logger.format, logger.name, timeString); // print logger stuff
			vfprintf(stream, format, args); // print actual message
		}
	}
	inline void logln(FILE *stream, const Logger &logger, const char *format, va_list args) {
		if(logger.level >= logLevel) {
			log(stream, logger, format, args);
			putc('\n', stream);
		}
	}
	
	/// print to `stream` with no newline
	inline void log(FILE *stream, const Logger &logger, const char *format, ...) {
		va_list args;

		va_start(args, format);
		log(stream, logger, format, args);
		va_end(args);
	}
	/// print to default stream with no newline
	inline void log(const Logger &logger, const char *format, ...) {
		va_list args;

		va_start(args, format);
		log(logger.stream, logger, format, args);
		va_end(args);
	}

	/// print to `stream` with a newline added automatically
	inline void logln(FILE *stream, const Logger &logger, const char *format, ...) {
		va_list args;

		va_start(args, format);
		logln(stream, logger, format, args);
		va_end(args);
	}
	/// print to default stream with a newline added automatically
	inline void logln(const Logger &logger, const char *format, ...) {
		va_list args;

		va_start(args, format);
		logln(logger.stream, logger, format, args);
		va_end(args);
	}

	/// Prints to the error logger and exits the program 
	inline void exitError(const char *format, ...) {
		va_list args;

		va_start(args, format);
		logln(litelogger::ERROR, format, args);
		va_end(args);

		exit(EXIT_FAILURE);
	}
}

#endif // LITELOGGER