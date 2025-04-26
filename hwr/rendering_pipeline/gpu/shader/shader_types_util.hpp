#ifndef SHADER_TYPES_UTIL_HPP
#define SHADER_TYPES_UTIL_HPP

#include <stdexcept>

#include "./static_string.hpp"

namespace hwr {

    template<typename T>
    struct is_integer_type : std::false_type {};

    template<> struct is_integer_type<int64_t>   : std::true_type {};
    template<> struct is_integer_type<int32_t>   : std::true_type {};
    template<> struct is_integer_type<int16_t>   : std::true_type {};
    template<> struct is_integer_type<int8_t>   : std::true_type {};
    template<> struct is_integer_type<uint64_t>   : std::true_type {};
    template<> struct is_integer_type<uint32_t>   : std::true_type {};
    template<> struct is_integer_type<uint16_t>   : std::true_type {};
    template<> struct is_integer_type<uint8_t>   : std::true_type {};

    template<typename T>
    constexpr bool is_integer_type_v = is_integer_type<T>::value;

    template<typename T>
    concept IntegerType = is_integer_type_v<T>;

    template<typename T>
    struct is_floating_type : std::false_type {};

    template<> struct is_floating_type<float>   : std::true_type {};
    template<> struct is_floating_type<double>   : std::true_type {};

    template<typename T>
    constexpr bool is_floating_type_v = is_floating_type<T>::value;

    template<typename T>
    concept FloatingType = is_floating_type_v<T>;

    
    template<typename T>
    struct is_allowed_type 
        : std::bool_constant< IntegerType<T> || FloatingType<T> || std::is_same_v<T, bool> > {};
    
    template<typename T>
    constexpr bool is_allowed_type_v = is_allowed_type<T>::value;
    
    template<typename T>
    concept AllowedShaderType = is_allowed_type_v<T>;



    template<AllowedShaderType T>
    std::string toOpenCLCode(T what){
        throw std::runtime_error("toOpenCLCode() not defined for this type");
    }

    template<>
    inline std::string toOpenCLCode(int what){
        return std::to_string(what);
    }

    template<>
    inline std::string toOpenCLCode(double what){
        return std::to_string(what);
    }

    template<>
    inline std::string toOpenCLCode(float what){
        return std::to_string(what);
    }

    template<>
    inline std::string toOpenCLCode(bool what){
        return what ? "true" : "false";
    }

    template<typename T, typename U>
    constexpr bool is_comparable_v = 
        ( (IntegerType<T> || FloatingType<T> || std::is_same_v<T, bool>) &&
          (IntegerType<U> || FloatingType<U> || std::is_same_v<U, bool>) );
    

    template<AllowedShaderType T, AllowedShaderType U, StaticString op>
    struct is_op_allowed : std::bool_constant<
        // Arithmetic (+, -, *, /) allowed if (at least one side is IntegerType or FloatingType)
        ((op == "+" || op == "-" || op == "*" || op == "/") &&
        ( (IntegerType<T> || FloatingType<T> || IntegerType<U> || FloatingType<U>) )) ||
    
        // Modulo (%) only if both sides are IntegerType (bool counts as int)
        (op == "%" &&
        ( (IntegerType<T> || std::is_same_v<T, bool>) &&
            (IntegerType<U> || std::is_same_v<U, bool>) )) ||
    
        // Compound assignment arithmetic (+=, -=, *=, /=) if at least one IntegerType or FloatingType
        ((op == "+=" || op == "-=" || op == "*=" || op == "/=") &&
        ( (IntegerType<T> || FloatingType<T> || IntegerType<U> || FloatingType<U>) )) ||
    
        // Compound assignment modulo (%=) only between IntegerType (bool counts as integer)
        (op == "%=" &&
        ( (IntegerType<T> || std::is_same_v<T, bool>) &&
            (IntegerType<U> || std::is_same_v<U, bool>) )) ||
    
        // Bitwise (&, |, ^, <<, >>) allowed if both are IntegerType or bool
        ((op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>") &&
        ( (IntegerType<T> || std::is_same_v<T, bool>) &&
            (IntegerType<U> || std::is_same_v<U, bool>) )) ||
    
        // Compound assignment bitwise (&=, |=, ^=, <<=, >>=) allowed if both are IntegerType or bool
        ((op == "&=" || op == "|=" || op == "^=" || op == "<<=" || op == ">>=") &&
        ( (IntegerType<T> || std::is_same_v<T, bool>) &&
            (IntegerType<U> || std::is_same_v<U, bool>) )) ||
    
        // Logical operators (&&, ||) allowed if both are IntegerType or bool
        ((op == "&&" || op == "||") &&
        ( (IntegerType<T> || std::is_same_v<T, bool>) &&
            (IntegerType<U> || std::is_same_v<U, bool>) )) ||
    
        // Comparisons (==, !=, <, <=, >, >=) allowed between all numeric types and bools
        ((op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=") &&
        ( (IntegerType<T> || FloatingType<T> || std::is_same_v<T, bool>) &&
            (IntegerType<U> || FloatingType<U> || std::is_same_v<U, bool>) ))
    > {};
          


    template<AllowedShaderType T, AllowedShaderType U, StaticString op>
    constexpr bool is_op_allowed_v = is_op_allowed<T, U, op>::value;
        
    
    template<AllowedShaderType T, AllowedShaderType U, StaticString Op>
    struct binary_result_type {
        static_assert(is_op_allowed<T, U, Op>::value, "This binary operation is not allowed for these types.");

        static constexpr bool is_comparison =
            (Op == "==" || Op == "!=" || Op == "<" || Op == "<=" || Op == ">" || Op == ">=");

        using type = std::conditional_t<
            is_comparison,
            bool,                        // comparisons always return bool
            std::common_type_t<T, U>      // others use common_type
        >;
    };

    template<typename T, typename U, StaticString Op>
    using binary_result_type_t = typename binary_result_type<T, U, Op>::type;

    

    // Primary template
    template<typename From, typename To>
    struct is_shader_convertible : std::false_type {};

    // Identity conversion
    template<typename T>
    struct is_shader_convertible<T, T> : std::true_type {};

    // ── Integer widening
    template<> struct is_shader_convertible<int8_t, int32_t> : std::true_type {};
    template<> struct is_shader_convertible<int16_t, int32_t> : std::true_type {};
    template<> struct is_shader_convertible<uint8_t, uint32_t> : std::true_type {};
    template<> struct is_shader_convertible<uint16_t, uint32_t> : std::true_type {};

    // ── Integer to floating point
    template<> struct is_shader_convertible<int32_t, float> : std::true_type {};
    template<> struct is_shader_convertible<int32_t, double> : std::true_type {};
    template<> struct is_shader_convertible<uint32_t, float> : std::true_type {};
    template<> struct is_shader_convertible<uint32_t, double> : std::true_type {};

    // ── Float widening
    template<> struct is_shader_convertible<float, double> : std::true_type {};

    // ── Bool to integer/float/double
    template<> struct is_shader_convertible<bool, int32_t> : std::true_type {};
    template<> struct is_shader_convertible<bool, uint32_t> : std::true_type {};
    template<> struct is_shader_convertible<bool, float> : std::true_type {};
    template<> struct is_shader_convertible<bool, double> : std::true_type {};

    // ── Numeric to bool (via != 0)
    template<> struct is_shader_convertible<int32_t, bool> : std::true_type {};
    template<> struct is_shader_convertible<uint32_t, bool> : std::true_type {};
    template<> struct is_shader_convertible<float, bool> : std::true_type {};
    template<> struct is_shader_convertible<double, bool> : std::true_type {};

    template<typename From, typename To>
    inline constexpr bool is_shader_convertible_v = is_shader_convertible<From, To>::value;


    // Primary template (undefined on purpose — forces specialization)
    template<typename T>
    struct OpenCLTypeName;

    template<>
    struct OpenCLTypeName<int64_t> {
        static constexpr std::string_view value = "long";
    };

    template<>
    struct OpenCLTypeName<int32_t> {
        static constexpr std::string_view value = "int";
    };

    template<>
    struct OpenCLTypeName<int16_t> {
        static constexpr std::string_view value = "short";
    };

    template<>
    struct OpenCLTypeName<int8_t> {
        static constexpr std::string_view value = "char";
    };

    template<>
    struct OpenCLTypeName<uint64_t> {
        static constexpr std::string_view value = "ulong";
    };

    template<>
    struct OpenCLTypeName<uint32_t> {
        static constexpr std::string_view value = "uint";
    };

    template<>
    struct OpenCLTypeName<uint16_t> {
        static constexpr std::string_view value = "ushort";
    };

    template<>
    struct OpenCLTypeName<uint8_t> {
        static constexpr std::string_view value = "uchar";
    };


    template<>
    struct OpenCLTypeName<float> {
        static constexpr std::string_view value = "float";
    };

    template<>
    struct OpenCLTypeName<double> {
        static constexpr std::string_view value = "double";
    };

    template<>
    struct OpenCLTypeName<bool> {
        static constexpr std::string_view value = "bool";
    };

    // Convenience helper
    template<typename T>
    constexpr std::string_view opencl_type_name_v = OpenCLTypeName<T>::value;

} // namespace hwr

#endif // SHADER_TYPES_UTIL_HPP