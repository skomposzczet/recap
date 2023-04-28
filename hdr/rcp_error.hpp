#ifndef _RCP_ERROR_H_
#define _RCP_ERROR_H_

#include <stdexcept>

namespace rcp {

constexpr char parse_title[]{"Parsing error"};

class RcpError: public std::exception
{
public:
    RcpError(const std::string& message) : message{message} {}

    const char* what() const noexcept override { return message.data(); }

protected:
    std::string message;

    static std::string titled_message(const std::string& title, const std::string message) {
        return title + "(" + message + ")";
    }
};

class ParseError: public RcpError
{
public:
    ParseError(const std::string& message) : RcpError(titled_message(parse_title, message)) {}
};

}

#endif // _RCP_ERROR_H_