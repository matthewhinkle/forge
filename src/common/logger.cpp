//
// Created by Matthew Hinkle on 5/17/15.
//

#include "logger.h"

using forge::Logger;

Logger::Logger(const std::string & name, FILE * file, Logger::Level level) noexcept
    :
    name(name) {
    assert(file);
    assert(level >= LOG_LEVEL_TRACE);
    assert(level <= LOG_LEVEL_FATAL);

    std::FILE * file_copy = new std::FILE();
    std::memcpy(file_copy, file, sizeof(std::FILE));

    this->file = file_copy;
    this->level = level;
}

Logger::Logger(const Logger::Logger & l) noexcept : Logger::Logger(l.name, l.file, l.level) { }

Logger::Logger(Logger::Logger && l) noexcept
    :
    name(std::move(l.name)),
    file(std::move(l.file)),
    level(l.level) { }

Logger & Logger::operator=(const Logger::Logger & l) noexcept {
    assert(l.file);

    std::FILE * file_copy = new std::FILE();
    std::memcpy(file_copy, l.file, sizeof(std::FILE));

    this->name = l.name;
    this->file = file_copy;
    this->level = l.level;

    return *this;
}

Logger & Logger::operator=(Logger::Logger && l) noexcept {
    this->name = std::move(l.name);
    this->file = std::move(l.file);
    this->level = l.level;
    return *this;
}

Logger::~Logger() noexcept {
    if(this->file) {
        delete this->file;
        this->file = nullptr;
    }
}

const std::string & Logger::get_name() const {
    return this->name;
}

const std::FILE * Logger::get_file() const {
    return this->file;
}

Logger::Level Logger::get_level() const {
    return this->level;
}

void Logger::trace(const char * fmt, ...) const {
    if(LOG_LEVEL_TRACE < this->level) return;

    std::string fmt_copy(fmt);
    fmt_copy.insert(0, "[TRACE] " + this->name + " :: ");
    const char * f = fmt_copy.c_str();

    va_list args;
    va_start(args, fmt);
    std::vfprintf(this->file, f, args);
    va_end(args);
}

void Logger::info(const char * fmt, ...) const {
    if(LOG_LEVEL_INFO < this->level) return;

    std::string fmt_copy(fmt);
    fmt_copy.insert(0, "[INFO] " + this->name + " :: ");
    const char * f = fmt_copy.c_str();

    va_list args;
    va_start(args, fmt);
    std::vfprintf(this->file, f, args);
    va_end(args);
}

void Logger::debug(const char * fmt, ...) const {
    if(LOG_LEVEL_DEBUG < this->level) return;

    std::string fmt_copy(fmt);
    fmt_copy.insert(0, "[DEBUG] " + this->name + " :: ");
    const char * f = fmt_copy.c_str();

    va_list args;
    va_start(args, fmt);
    std::vfprintf(this->file, f, args);
    va_end(args);
}

void Logger::warn(const char * fmt, ...) const {
    if(LOG_LEVEL_WARN < this->level) return;

    std::string fmt_copy(fmt);
    fmt_copy.insert(0, "[WARN] " + this->name + " :: ");
    const char * f = fmt_copy.c_str();

    va_list args;
    va_start(args, fmt);
    std::vfprintf(this->file, f, args);
    va_end(args);
}

void Logger::error(const char * fmt, ...) const {
    if(LOG_LEVEL_ERROR < this->level) return;

    std::string fmt_copy(fmt);
    fmt_copy.insert(0, "[ERROR] " + this->name + " :: ");
    const char * f = fmt_copy.c_str();

    va_list args;
    va_start(args, fmt);
    std::vfprintf(this->file, f, args);
    va_end(args);
}

void Logger::fatal(const char * fmt, ...) const {
    if(LOG_LEVEL_FATAL < this->level) return;

    std::string fmt_copy(fmt);
    fmt_copy.insert(0, "[FATAL] " + this->name + " :: ");
    const char * f = fmt_copy.c_str();

    va_list args;
    va_start(args, fmt);
    std::vfprintf(this->file, f, args);
    va_end(args);
}

