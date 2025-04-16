#ifndef HWR_PROGRAM_HPP
#define HWR_PROGRAM_HPP

#include<type_traits>
#include<string>
#include<hwr/buffer.hpp>

#define ARG(tp,notAString) hwr::Arg<tp,hwr::detail::StaticString(notAString)>

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

    template<std::size_t N>
    struct StaticString {
        char data[N]{};

        constexpr StaticString(const char (&str)[N]) {
            for (std::size_t i = 0; i < N; ++i)
                data[i] = str[i];
        }

        constexpr const char* c_str() const { return data; }

        constexpr std::size_t size() const { return N; }

        constexpr char operator[](std::size_t i) const {
            return (i < N) ? data[i] : '\0';
        }

        constexpr bool operator==(const StaticString& other) const {
            for (std::size_t i = 0; i < N; ++i)
                if (data[i] != other.data[i]) return false;
            return true;
        }
    };

}  // namespace detail

template<typename T, detail::StaticString name>
class Arg{
    static_assert(detail::is_valid_arg_v<T>, "arg type must be valid (either a buffer of trivially copyable)");
    public:
        const char* getName(){
            return name.c_str();
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