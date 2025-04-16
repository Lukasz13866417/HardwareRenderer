#ifndef HWR_ARG_HPP
#define HWR_ARG_HPP

#include <type_traits>
#include <string>
#include <hwr/buffer.hpp>

#define ARG(tp, notAString) hwr::Arg<tp, hwr::detail::StaticString(notAString)>
#define OUT(tp, notAString) hwr::Arg<tp, hwr::detail::StaticString(notAString)>

namespace hwr {

namespace detail {

    // Valid arg type checker
    template<typename T>
    struct is_valid_arg : std::bool_constant<std::is_trivially_copyable_v<T>> {};

    template<typename T>
    struct is_valid_arg<GeneralBuffer<T>> : std::true_type {};

    template<typename T>
    struct is_valid_arg<HostMappedBuffer<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_valid_arg_v = is_valid_arg<T>::value;

    // Static string wrapper (literal NTTP support)
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

} // namespace detail

// ========================
// Arg Class Template
// ========================

template<typename T, detail::StaticString name>
class Arg {
    static_assert(detail::is_valid_arg_v<T>, "arg type must be valid (either a buffer or trivially copyable)");

public:
    const char* getName() {
        return name.c_str();
    }
};

namespace detail {

    // Arg Type Checker
    template<typename T>
    struct is_arg_type : std::false_type {};

    template<typename U, auto Name>
    struct is_arg_type<hwr::Arg<U, Name>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_arg_type_v = is_arg_type<T>::value;

    // Pack validator
    template<typename... Args>
    constexpr bool are_these_args() {
        return (is_arg_type_v<Args> && ...);
    }

    // Trait extractor
    template<typename A>
    struct arg_traits;

    template<typename T, auto Name>
    struct arg_traits<Arg<T, Name>> {
        using type = T;
        static constexpr auto name = Name;
    };

} // namespace detail

} // namespace hwr

#endif // HWR_ARG_HPP
