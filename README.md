# LiteLogger

<hr>

A lightweight header-only logging library written in C++, ~100 LOC

Features 4 default loggers with log levels to control which you see at any time. New loggers can be created dynamically, for example per-class or per-thread if that's your thing.

## Use:

#include litelogger.hpp in any file you want to use it

There are 4 default loggers in LiteLogger, but new ones can be created and used dynamically
```cpp
litelogger::logln(litelogger::DEBUG, "debug :D");
litelogger::logln(litelogger::INFO, "info :)");
litelogger::logln(litelogger::WARN, "warning :/");
litelogger::logln(litelogger::ERROR, "error D:");

const litelogger::Logger test("Test", 0, stdout);

litelogger::logln(test, "woah a custom logger?!");
```
The arguments are: name, format (optionally, see code), log level, default output stream

Log level controls whether the message actually prints or not. It needs to be >= the global log level to print to the console. You can change the global log level at any time using the `changeLevel` function
```cpp
const litelogger::Logger test("Test", 0, stdout);

litelogger::changeLevel(1);
litelogger::logln(test, "beep"); // doesn't print; 0 < 1
litelogger::changeLevel(-1);
litelogger::logln(test, "boop"); // does print; 0 >= -1
```
If you don't want a newline to be added automatically when you print, you can use the log function instead of logln. You may have to flush the output stream though
```cpp
litelogger::log(litelogger::INFO, "Description: ");
litelogger::flush(litelogger::INFO);
// TODO accept input
```
you can also write to a specific output file or flush a specific stream if you have a need for that
```cpp
litelogger::log(stdout, litelogger::INFO, "Response: ");
litelogger::flush(stdout);

if(error) {
	FILE *errorFile = fopen("logs/latest.txt", "w");
	litelogger::logln(errorFile, litelogger::ERROR, "something bad happened");
	fclose(errorFile);
	exit(EXIT_FAILURE);
}
```
I also #defined some "separators" in case you want to separate some outputs in your console like minecraft

Here's how they look:
```
DOUBLE_SEPARATOR:
================================================================================
HASHTAG_SEPARATOR:
################################################################################
SMALL_SEPARATOR:
----------------------------------------------------------------
SUPER_SMALL_SEPARATOR:
------------------------------------------------
```

## License:

zlib, included in litelogger.hpp