#ifndef _RCP_RESULT_H_
#define _RCP_RESULT_H_

#include "rcp_error.hpp"
#include <optional>
#include <string>
#include <variant>

namespace rcp {

template<typename T = nullptr_t>
class Result
{
friend class ResultFactory;
public:
    bool is_ok() const {
        return std::holds_alternative<T>(_v);
    }
    bool is_err() const {
        return std::holds_alternative<std::string>(_v);
    }

    T get_ok() const {
        if (is_err())
            throw RcpError("Tried to get result ok, when it is err.");
        return std::get<T>(_v);
    }
    const std::string& get_err() const {
        if (is_ok())
            throw RcpError("Tried to get result err, when it is ok.");
        return std::get<std::string>(_v);
    }

private:
    Result() = default;

    std::variant<T, std::string> _v;

};

class ResultFactory
{
public:
    template<typename T>
    static Result<T> ok(const T& t) {
        Result<T> res;
        res._v = t;
        return res;
    }

    static Result<> ok() {
        return Result<>{};
    }

    static Result<> err(const std::string& err_str) {
        Result<> res;
        res._v = err_str;
        return res;
    }
};

}

#endif // _RCP_RESULT_H_