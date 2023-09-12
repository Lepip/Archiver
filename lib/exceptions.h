#pragma once
#include <string.h>

class TOO_FEW_ARGUMENTS : public std::exception {
public:
    TOO_FEW_ARGUMENTS() = default;
    const char* what() const noexcept {
        return "missing arguments";
    }
};

/*
class TOO_MANY_ARGUMENTS : public std::exception{
public:
    TOO_MANY_ARGUMENTS() = default;
    const char* what() const noexcept {
        return "too many arguments";
    }
};
*/

class WRONG_ARGUMENT : public std::exception {
public:
    WRONG_ARGUMENT() = default;
    const char* what() const noexcept {
        return "got wrong argument";
    }
};

class TERMINATED_BY_USER : public std::exception {
public:
    TERMINATED_BY_USER() = default;
    const char* what() const noexcept {
        return "program got terminated by user";
    }
};

class COULD_NOT_READ : public std::exception {
    char* message_;
    bool in_ = false;

public:
    COULD_NOT_READ() = default;
    COULD_NOT_READ(const char* message) {
        in_ = true;
        message_ = static_cast<char*>(malloc(strlen(message) + 1));
        strcpy(message_, message);
    }
    const char* what() const noexcept {
        if (!in_) {
            return "got corrupted or wrong files";
        } else {
            return message_;
        }
    }
};

class COULD_NOT_WRITE : public std::exception {
public:
    COULD_NOT_WRITE() = default;
    const char* what() const noexcept {
        return "got unwritable output file";
    }
};
