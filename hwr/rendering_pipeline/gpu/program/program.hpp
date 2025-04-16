#ifndef HWR_PROGRAM_HPP
#define HWR_PROGRAM_HPP

#include <string>
#include <type_traits>
#include "./arg.hpp"

namespace hwr {

template<typename Result, typename... Args>
class Program {
    static_assert(detail::are_these_args<Args...>(), "All Args must be of type Arg<T, Name>");
    static_assert(detail::is_arg_type_v<Result>, "Result must be an Arg<T, Name>");

private:
    const std::string code_;

public:
    Program(const std::string& code) : code_(code) {}

    const std::string& getCode() {
        return code_;
    }
};

} // namespace hwr

#endif // HWR_PROGRAM_HPP
