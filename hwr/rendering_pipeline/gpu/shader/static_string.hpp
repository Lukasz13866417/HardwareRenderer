#ifndef HWR_STATIC_STRING_HPP
#define HWR_STATIC_STRING_HPP

namespace hwr {

template<std::size_t N>
struct StaticString {
    char value[N]{};

    // constexpr constructor from string literal
    consteval StaticString(const char (&str)[N]) {
        for (std::size_t i = 0; i < N; ++i) {
            value[i] = str[i];
        }
    }

    // Implicit conversion to string_view for usability
    constexpr operator std::string_view() const {
        return std::string_view(value, N - 1); // exclude null terminator
    }

    // Comparison (optional)
    constexpr bool operator==(const StaticString& other) const {
        for (std::size_t i = 0; i < N; ++i)
            if (value[i] != other.value[i]) return false;
        return true;
    }
};

} // namespace hwr

#endif // HWR_STATIC_STRING_HPP