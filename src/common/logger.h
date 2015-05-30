//
// Created by Matthew Hinkle on 5/17/15.
//

#ifndef FORGE_LOGGER_H
#define FORGE_LOGGER_H

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace forge {

class Logger final {
public:

    /**
     * Log level filter.
     */
    enum Level {
        LOG_LEVEL_TRACE = 0,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_FATAL
    };

    /**
     * Create logger that writes to the specified file and filters
     * on the specified level.
     *
     * @name a label prepended to each log message.
     * @param file the output file.
     * @param level the minimum log level to write.  Any logging
     *              attempt with a lesser level is ignored.
     */
    Logger(const std::string & name, FILE * file = stderr, Level level = LOG_LEVEL_TRACE) noexcept;
    Logger(const Logger & l) noexcept;
    Logger(Logger && l) noexcept;
    Logger & operator=(const Logger & l) noexcept;
    Logger & operator=(Logger && l) noexcept;
    ~Logger() noexcept;

    const std::string & get_name() const;
    const FILE * get_file() const;
    Level get_level() const;

    void trace(const char * fmt, ...);
    void info(const char * fmt, ...);
    void debug(const char * fmt, ...);
    void warn(const char * fmt, ...);
    void error(const char * fmt, ...);
    void fatal(const char * fmt, ...);

private:
    std::string name;
    FILE * file;
    Level level;
};

}

#endif //FORGE_LOGGER_H
