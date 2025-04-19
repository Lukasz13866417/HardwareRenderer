#ifndef SHADER_TYPES_UTIL_HPP
#define SHADER_TYPES_UTIL_HPP

namespace hwr {

    namespace detail {

        template<typename T>
        struct is_allowed_type : std::disjunction<
            std::is_same<T, int>,
            std::is_same<T, float>,
            std::is_same<T, double>
        > {};

        template<typename T>
        constexpr bool is_allowed_type_v = is_allowed_type<T>::value;
        
    } // namespace detail

    // Primary template (undefined on purpose — forces specialization)
    template<typename T>
    struct OpenCLTypeName;

    // Specializations
    template<>
    struct OpenCLTypeName<int> {
        static constexpr std::string_view value = "int";
    };

    template<>
    struct OpenCLTypeName<float> {
        static constexpr std::string_view value = "float";
    };

    template<>
    struct OpenCLTypeName<double> {
        static constexpr std::string_view value = "double";
    };

    // Convenience helper
    template<typename T>
    constexpr std::string_view opencl_type_name_v = OpenCLTypeName<T>::value;

} // namespace hwr

#endif // SHADER_TYPES_UTIL_HPP