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
    using type = T;
    using err_type = std::string;

    bool is_ok() const {
        return std::holds_alternative<type>(_v);
    }
    bool is_err() const {
        return std::holds_alternative<err_type>(_v);
    }

    type get_ok() const {
        if (is_err())
            throw RcpError("Tried to get result ok, when it is err.");
        return std::get<type>(_v);
    }
    type get_ok_or(const type& or_val) {
        if (is_ok())
            return std::get<type>(_v);
        return or_val;
    }
    const err_type& get_err() const {
        if (is_ok())
            throw RcpError("Tried to get result err, when it is ok.");
        return std::get<err_type>(_v);
    }

private:
    Result() = default;

    std::variant<type, err_type> _v;

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

    template<typename T>
    static Result<T> err(const std::string& err_str) {
        Result<T> res;
        res._v = err_str;
        return res;
    }

    static Result<> err(const std::string& err_str) {
        Result<> res;
        res._v = err_str;
        return res;
    }
};

}

#endif // _RCP_RESULT_H_