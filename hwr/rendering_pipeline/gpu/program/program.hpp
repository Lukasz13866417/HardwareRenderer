#ifndef HWR_PROGRAM_HPP
#define HWR_PROGRAM_HPP

#include<type_traits>
#include<string>
#include<hwr/buffer.hpp>

#define ARG(tp,notAString) hwr::Arg<tp,unString(notAString)>

constexpr int64_t unString(const char* arg){

    return 42 + arg[0];
}

namespace hwr{

namespace detail{

    template<typename T>
    struct is_valid_arg : std::bool_constant<std::is_trivially_copyable_v<T>> {};

    template<typename T>
    struct is_valid_arg<GeneralBuffer<T>> : std::true_type {};

    template<typename T>
    struct is_valid_arg<HostMappedBuffer<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_valid_arg_v = is_valid_arg<T>::value;

    template<typename... Types>
    constexpr bool are_args_valid() {
        return (is_valid_arg_v<Types> && ...);
    }   
};

template<typename T, int64_t notAString>
class Arg{
    static_assert(detail::is_valid_arg_v<T>, "arg type must be valid (either a buffer of trivially copyable)");
    public:
        int64_t getCode(){
            return notAString;
        }
};

template<typename Result, typename... Args>
class Program{
    static_assert(detail::are_args_valid<Args...>(), "All Args must be trivially copyable");
    static_assert(std::is_trivially_copyable_v<Result>, "Result must be trivially copyable");
    public:
        std::string generateCode(){
            return "XD";
        }
};

} // namespace hwr

#endif // HWR_PROGRAM_HPP 