#ifndef SHADER_TYPES_UTIL_HPP
#define SHADER_TYPES_UTIL_HPP

#include <stdexcept>

#include "./static_string.hpp"

namespace hwr {

    template<typename T>
    struct is_allowed_type : std::false_type {};

    // Built-in allowed types
    template<> struct is_allowed_type<int>   : std::true_type {};
    template<> struct is_allowed_type<float> : std::true_type {};
    template<> struct is_allowed_type<double>: std::true_type {};
    template<> struct is_allowed_type<bool>  : std::true_type {};

    template<typename T>
    constexpr bool is_allowed_type_v = is_allowed_type<T>::value;

    template<typename T>
    concept AllowedShaderType = is_allowed_type_v<T>;


    template<typename T>
    requires(is_allowed_type_v<T>)
    std::string toOpenCLCode(T what){
        throw std::runtime_error("toOpenCLCode() not defined for this type");
    }

    template<>
    std::string toOpenCLCode(int what){
        return std::to_string(what);
    }

    template<>
    std::string toOpenCLCode(double what){
        return std::to_string(what);
    }

    template<>
    std::string toOpenCLCode(float what){
        return std::to_string(what);
    }

    template<>
    std::string toOpenCLCode(bool what){
        return what ? "true" : "false";
    }

    
    template<AllowedShaderType T, AllowedShaderType U, StaticString op>
    struct is_op_allowed : std::false_type {};

    template<AllowedShaderType T, AllowedShaderType U, StaticString op>
    constexpr bool is_op_allowed_v = is_op_allowed<T, U, op>::value;

    // Macro to declare valid op overloads
    #define DEFINE_OP(T1, T2, op) \
        template<> struct is_op_allowed<T1, T2, op> : std::true_type {}

    // Arithmetic
    DEFINE_OP(int,    int,    "+");
    DEFINE_OP(int,    int,    "-");
    DEFINE_OP(int,    int,    "*");
    DEFINE_OP(int,    int,    "/");

    DEFINE_OP(float,  float,  "+");
    DEFINE_OP(float,  float,  "-");
    DEFINE_OP(float,  float,  "*");
    DEFINE_OP(float,  float,  "/");

    DEFINE_OP(double, double, "+");
    DEFINE_OP(double, double, "-");
    DEFINE_OP(double, double, "*");
    DEFINE_OP(double, double, "/");

    // Mixed Arithmetic (int <-> float/double)
    DEFINE_OP(int, float,   "+");
    DEFINE_OP(float, int,   "+");
    DEFINE_OP(int, double,  "+");
    DEFINE_OP(double, int,  "+");
    DEFINE_OP(float, double,"+");
    DEFINE_OP(double,float, "+");

    DEFINE_OP(int, float,   "-");
    DEFINE_OP(float, int,   "-");
    DEFINE_OP(int, double,  "-");
    DEFINE_OP(double, int,  "-");
    DEFINE_OP(float, double,"-");
    DEFINE_OP(double,float, "-");

    DEFINE_OP(int, float,   "*");
    DEFINE_OP(float, int,   "*");
    DEFINE_OP(int, double,  "*");
    DEFINE_OP(double, int,  "*");
    DEFINE_OP(float, double,"*");
    DEFINE_OP(double,float, "*");

    DEFINE_OP(int, float,   "/");
    DEFINE_OP(float, int,   "/");
    DEFINE_OP(int, double,  "/");
    DEFINE_OP(double, int,  "/");
    DEFINE_OP(float, double,"/");
    DEFINE_OP(double,float, "/");

    // Equality
    DEFINE_OP(int,    int,    "==");
    DEFINE_OP(float,  float,  "==");
    DEFINE_OP(double, double, "==");
    DEFINE_OP(bool,   bool,   "==");

    DEFINE_OP(int,    int,    "!=");
    DEFINE_OP(float,  float,  "!=");
    DEFINE_OP(double, double, "!=");
    DEFINE_OP(bool,   bool,   "!=");

    // Relational
    DEFINE_OP(int,    int,    "<");
    DEFINE_OP(float,  float,  "<");
    DEFINE_OP(double, double, "<");

    DEFINE_OP(int,    int,    "<=");
    DEFINE_OP(float,  float,  "<=");
    DEFINE_OP(double, double, "<=");

    DEFINE_OP(int,    int,    ">");
    DEFINE_OP(float,  float,  ">");
    DEFINE_OP(double, double, ">");

    DEFINE_OP(int,    int,    ">=");
    DEFINE_OP(float,  float,  ">=");
    DEFINE_OP(double, double, ">=");

    template<typename T, typename U, StaticString Op>
    struct binary_result_type;

    template<typename T, typename U, StaticString Op>
    using binary_result_type_t = typename binary_result_type<T, U, Op>::type;

    // 🧠 Utility
    #define DEFINE_BINARY_RESULT(T1, T2, OP, RES) \
    template<> struct binary_result_type<T1, T2, StaticString{OP}> { using type = RES; };

    // ─── Arithmetic ────────────────────────────────
    // int
    DEFINE_BINARY_RESULT(int, int, "+", int)
    DEFINE_BINARY_RESULT(int, int, "-", int)
    DEFINE_BINARY_RESULT(int, int, "*", int)
    DEFINE_BINARY_RESULT(int, int, "/", int)

    // float
    DEFINE_BINARY_RESULT(float, float, "+", float)
    DEFINE_BINARY_RESULT(float, float, "-", float)
    DEFINE_BINARY_RESULT(float, float, "*", float)
    DEFINE_BINARY_RESULT(float, float, "/", float)

    // double
    DEFINE_BINARY_RESULT(double, double, "+", double)
    DEFINE_BINARY_RESULT(double, double, "-", double)
    DEFINE_BINARY_RESULT(double, double, "*", double)
    DEFINE_BINARY_RESULT(double, double, "/", double)

    // mixed: int <-> float
    DEFINE_BINARY_RESULT(int, float, "+", float)
    DEFINE_BINARY_RESULT(float, int, "+", float)
    DEFINE_BINARY_RESULT(int, float, "-", float)
    DEFINE_BINARY_RESULT(float, int, "-", float)
    DEFINE_BINARY_RESULT(int, float, "*", float)
    DEFINE_BINARY_RESULT(float, int, "*", float)
    DEFINE_BINARY_RESULT(int, float, "/", float)
    DEFINE_BINARY_RESULT(float, int, "/", float)

    // mixed: int <-> double
    DEFINE_BINARY_RESULT(int, double, "+", double)
    DEFINE_BINARY_RESULT(double, int, "+", double)
    DEFINE_BINARY_RESULT(int, double, "-", double)
    DEFINE_BINARY_RESULT(double, int, "-", double)
    DEFINE_BINARY_RESULT(int, double, "*", double)
    DEFINE_BINARY_RESULT(double, int, "*", double)
    DEFINE_BINARY_RESULT(int, double, "/", double)
    DEFINE_BINARY_RESULT(double, int, "/", double)

    // mixed: float <-> double
    DEFINE_BINARY_RESULT(float, double, "+", double)
    DEFINE_BINARY_RESULT(double, float, "+", double)
    DEFINE_BINARY_RESULT(float, double, "-", double)
    DEFINE_BINARY_RESULT(double, float, "-", double)
    DEFINE_BINARY_RESULT(float, double, "*", double)
    DEFINE_BINARY_RESULT(double, float, "*", double)
    DEFINE_BINARY_RESULT(float, double, "/", double)
    DEFINE_BINARY_RESULT(double, float, "/", double)


    // ─── Equality ────────────────────────────────
    DEFINE_BINARY_RESULT(int, int, "==", bool)
    DEFINE_BINARY_RESULT(float, float, "==", bool)
    DEFINE_BINARY_RESULT(double, double, "==", bool)
    DEFINE_BINARY_RESULT(bool, bool, "==", bool)

    DEFINE_BINARY_RESULT(int, int, "!=", bool)
    DEFINE_BINARY_RESULT(float, float, "!=", bool)
    DEFINE_BINARY_RESULT(double, double, "!=", bool)
    DEFINE_BINARY_RESULT(bool, bool, "!=", bool)


    // ─── Relational ────────────────────────────────
    DEFINE_BINARY_RESULT(int, int, "<", bool)
    DEFINE_BINARY_RESULT(float, float, "<", bool)
    DEFINE_BINARY_RESULT(double, double, "<", bool)

    DEFINE_BINARY_RESULT(int, int, "<=", bool)
    DEFINE_BINARY_RESULT(float, float, "<=", bool)
    DEFINE_BINARY_RESULT(double, double, "<=", bool)

    DEFINE_BINARY_RESULT(int, int, ">", bool)
    DEFINE_BINARY_RESULT(float, float, ">", bool)
    DEFINE_BINARY_RESULT(double, double, ">", bool)

    DEFINE_BINARY_RESULT(int, int, ">=", bool)
    DEFINE_BINARY_RESULT(float, float, ">=", bool)
    DEFINE_BINARY_RESULT(double, double, ">=", bool)



    // Primary template
    template<typename From, typename To>
    struct is_shader_convertible : std::false_type {};

    // Identity conversion
    template<typename T>
    struct is_shader_convertible<T, T> : std::true_type {};

    // ── Integer -> Float & Double
    template<> struct is_shader_convertible<int, float>  : std::true_type {};
    template<> struct is_shader_convertible<int, double> : std::true_type {};

    // ── Float -> Double, Int
    template<> struct is_shader_convertible<float, double> : std::true_type {};
    template<> struct is_shader_convertible<float, int>    : std::true_type {};  // lossy

    // ── Double -> Float, Int
    template<> struct is_shader_convertible<double, float> : std::true_type {};  // lossy
    template<> struct is_shader_convertible<double, int>   : std::true_type {};  // lossy

    // ── Bool to numeric types (OpenCL allows this, maps true = 1, false = 0)
    template<> struct is_shader_convertible<bool, int>     : std::true_type {};
    template<> struct is_shader_convertible<bool, float>   : std::true_type {};
    template<> struct is_shader_convertible<bool, double>  : std::true_type {};

    // ── Optionally: Numeric to bool (via `!= 0`)
    template<> struct is_shader_convertible<int, bool>     : std::true_type {};
    template<> struct is_shader_convertible<float, bool>   : std::true_type {};
    template<> struct is_shader_convertible<double, bool>  : std::true_type {};

    // Helper
    template<typename From, typename To>
    inline constexpr bool is_shader_convertible_v = is_shader_convertible<From, To>::value;



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

    template<>
    struct OpenCLTypeName<bool> {
        static constexpr std::string_view value = "bool";
    };

    // Convenience helper
    template<typename T>
    constexpr std::string_view opencl_type_name_v = OpenCLTypeName<T>::value;

} // namespace hwr

#endif // SHADER_TYPES_UTIL_HPP