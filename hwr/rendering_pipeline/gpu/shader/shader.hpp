#ifndef HWR_SHADER_HPP
#define HWR_SHADER_HPP

#include <string>
#include <functional>
#include <stack>

#include "./shader_types_util.hpp"
#include "./static_string.hpp"
#include "./program_context.hpp"
#include "./string_parsing.hpp"

#define HWR_DEFINE_SHADER_BINARY_OP(OP)                                                        \
template<hwr::AllowedShaderType T, hwr::AllowedShaderType U>                                   \
requires hwr::is_op_allowed_v<T, U, hwr::StaticString{#OP}>                                    \
inline hwr::ShaderRValue<hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>>              \
operator OP(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValue<U>& rhs)                    \
{                                                                                              \
    using result_t = hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>;                  \
    return hwr::ShaderRValue<result_t>(hwr::detail::make_expr(                                 \
        hwr::detail::expr(lhs), #OP, hwr::detail::expr(rhs)));                                 \
}                                                                                              \
                                                                                               \
template<hwr::AllowedShaderType T, hwr::AllowedShaderType U>                                   \
requires hwr::is_op_allowed_v<T, U, hwr::StaticString{#OP}>                                    \
inline hwr::ShaderRValue<hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>>              \
operator OP(const hwr::ShaderValue<T>& lhs, const U& rhs)                                      \
{                                                                                              \
    using result_t = hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>;                  \
    return hwr::ShaderRValue<result_t>(hwr::detail::make_expr(                                 \
        hwr::detail::expr(lhs), #OP, hwr::detail::expr(rhs)));                                 \
}                                                                                              \
                                                                                               \
template<hwr::AllowedShaderType T, hwr::AllowedShaderType U>                                   \
requires hwr::is_op_allowed_v<T, U, hwr::StaticString{#OP}>                                    \
inline hwr::ShaderRValue<hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>>              \
operator OP(const T& lhs, const hwr::ShaderValue<U>& rhs)                                      \
{                                                                                              \
    using result_t = hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>;                  \
    return hwr::ShaderRValue<result_t>(hwr::detail::make_expr(                                 \
        hwr::detail::expr(lhs), #OP, hwr::detail::expr(rhs)));                                 \
}                                                                                              \
                                                                                               \
template<hwr::AllowedShaderType T, hwr::AllowedShaderType U>                                   \
requires hwr::is_op_allowed_v<T, U, hwr::StaticString{#OP}>                                    \
inline hwr::ShaderRValue<hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>>              \
operator OP(const hwr::ShaderValue<T>& lhs, const hwr::ShaderRValue<U>& rhs)                  \
{                                                                                              \
    using result_t = hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>;                  \
    return hwr::ShaderRValue<result_t>(hwr::detail::make_expr(                                 \
        hwr::detail::expr(lhs), #OP, hwr::detail::expr(rhs)));                                 \
}                                                                                              \
                                                                                               \
template<hwr::AllowedShaderType T, hwr::AllowedShaderType U>                                   \
requires hwr::is_op_allowed_v<T, U, hwr::StaticString{#OP}>                                    \
inline hwr::ShaderRValue<hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>>              \
operator OP(const hwr::ShaderRValue<T>& lhs, const hwr::ShaderValue<U>& rhs)                   \
{                                                                                              \
    using result_t = hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>;                  \
    return hwr::ShaderRValue<result_t>(hwr::detail::make_expr(                                 \
        hwr::detail::expr(lhs), #OP, hwr::detail::expr(rhs)));                                 \
}                                                                                              \
                                                                                               \
template<hwr::AllowedShaderType T, hwr::AllowedShaderType U>                                   \
requires hwr::is_op_allowed_v<T, U, hwr::StaticString{#OP}>                                    \
inline hwr::ShaderRValue<hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>>              \
operator OP(const hwr::ShaderRValue<T>& lhs, const U& rhs)                                     \
{                                                                                              \
    using result_t = hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>;                  \
    return hwr::ShaderRValue<result_t>(hwr::detail::make_expr(                                 \
        hwr::detail::expr(lhs), #OP, hwr::detail::expr(rhs)));                                 \
}                                                                                              \
                                                                                               \
template<hwr::AllowedShaderType T, hwr::AllowedShaderType U>                                   \
requires hwr::is_op_allowed_v<T, U, hwr::StaticString{#OP}>                                    \
inline hwr::ShaderRValue<hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>>              \
operator OP(const T& lhs, const hwr::ShaderRValue<U>& rhs)                                     \
{                                                                                              \
    using result_t = hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>;                  \
    return hwr::ShaderRValue<result_t>(hwr::detail::make_expr(                                 \
        hwr::detail::expr(lhs), #OP, hwr::detail::expr(rhs)));                                 \
}                                                                                              \
                                                                                               \
template<hwr::AllowedShaderType T, hwr::AllowedShaderType U>                                   \
requires hwr::is_op_allowed_v<T, U, hwr::StaticString{#OP}>                                    \
inline hwr::ShaderRValue<hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>>              \
operator OP(const hwr::ShaderRValue<T>& lhs, const hwr::ShaderRValue<U>& rhs)                 \
{                                                                                              \
    using result_t = hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>;                  \
    return hwr::ShaderRValue<result_t>(hwr::detail::make_expr(                                 \
        hwr::detail::expr(lhs), #OP, hwr::detail::expr(rhs)));                                 \
}

namespace hwr {
    template<typename T> class ShaderValue; // forward decl
    template<typename T> class ShaderRValue; // forward decl

    namespace detail {
        template<typename T> std::string expr(const ShaderValue<T>& v);
        template<typename T> std::string expr(const ShaderRValue<T>& v);
        template<typename T> std::string expr(const T& v);
    }

    class Program;


namespace detail {


    template<typename T>
    struct is_shader_type : std::false_type {};
    // There will be specializations that gives std::true_type. 
    // Each type like Float, Int etc will have such corresponding specialization
            
    template<typename T>
    inline constexpr bool is_shader_type_v = is_shader_type<T>::value;
    
    template<typename... Ts>
    constexpr bool are_shader_types_v = (is_shader_type_v<Ts> && ...);



    // Lambda traits to introspect lambda's args
    template<typename T>
    struct lambda_traits;

    // Specialization for lambdas with operator()
    template<typename Ret, typename ClassType, typename... Args>
    struct lambda_traits<Ret(ClassType::*)(Args...) const> {
        using return_type = Ret;
        using arg_types = std::tuple<Args...>;
    };

    template<typename Lambda>
    using lambda_traits_t = lambda_traits<decltype(&Lambda::operator())>;

    template<typename Lambda>
    using lambda_args_t = typename lambda_traits_t<Lambda>::arg_types;

    template<typename Lambda>
    using lambda_return_t = typename lambda_traits_t<Lambda>::return_type;

    

}  // namespace detail


class Program {

private:
    int32_t _remaining_to_ignore = 0;
    std::vector<std::string> code_;
    std::function<void()> compilable_fn_;
    bool compiled_ = false;


    void append(const std::string& what) {
        if(what.empty()){
            HWR_FATAL("Appending empty code is illegal");
        }
        if(!detail::program_context::is_forloop_header_being_generated()){
            if (_remaining_to_ignore == 0) {
                code_.push_back(what);
            } else {
                --_remaining_to_ignore;
            }
        }else{
            if(code_.empty()){
                code_.push_back(what);
            }else{
                code_.back() += what;
            }
            if(what.back() == ';'){
                code_.back().back() = ',';
            }
        }
    }

    void replace_possible_comma_with_semicolon(){
        if(code_.empty()){
            HWR_FATAL("Program has no code");
        }else{
            code_.back().back() = ';';
        }
    }

    void remove_last_char(){
        if(code_.empty()){
            HWR_FATAL("Program has no code");
        }else{
            code_.back().pop_back();
        }
    }

    void ignore_next_k_appends(int32_t k){
        _remaining_to_ignore = k;
    }

    void undo_last_k_appends(int32_t k){
        while(k--){
            code_.pop_back();
        }
    }

public:

    template<typename Lambda, 
        typename = std::enable_if_t<std::is_invocable_v<Lambda>>>
    Program(Lambda&& fn)
    : compilable_fn_(std::forward<Lambda>(fn)) {}


    std::string compile() {
        // Push before generating code
        detail::program_context::push_program(*this);

        // Invoke user-provided generation lambda
        if (compilable_fn_) compilable_fn_();

        // Pop after generation
        detail::program_context::pop_program();
        compiled_ = true;
        std::string res="";
        for(const std::string& str : code_){
            res += str+"\n";
        }
        return res;
    }

    // Accessor for ProgramContext
    friend void detail::program_context::push_program(Program& p);
    friend void detail::program_context::appendToProgramCode(
                                                        const std::string& s);
    friend void detail::program_context::replace_possible_comma_with_semicolon();
    friend void detail::program_context::remove_last_char();

    friend void detail::program_context::ignore_next_k_appends(int32_t k);
    friend void detail::program_context::undo_last_k_appends(int32_t k);

}; 

namespace detail {
    

    const inline std::string COMMON_RVALUE_NAME = "";
    const inline std::string COMMON_RVALUE_DEFINITION = "";
    
    
    template<typename T>
    std::string getExpression(const ShaderRValue<T>& wrapper);

    template<typename U>
    const std::string& getValName(const ShaderValue<U>& shv);

} // namespace detail
template<typename T>
class ShaderValue {
    static_assert(is_allowed_type_v<T>, "T must be valid shader type");

public:
    const std::string& getDefinition() { return def_; }

    const std::string& getOpenCLType() const { return type_; }

    ShaderValue(const ShaderRValue<T>& rhs)
        : ShaderValue(
            std::string(opencl_type_name_v<T>),
            detail::getExpression(rhs)
        ) {}

    ShaderValue(T from)
        : ShaderValue(
            std::string(opencl_type_name_v<T>),
            toOpenCLCode(from)
        ) {}

    template<typename U>
    requires (is_allowed_type_v<U> && !std::is_same_v<U, T>)
    ShaderValue(const ShaderRValue<U>& rhs)
        : ShaderValue(
            std::string(opencl_type_name_v<T>),
            makeConversionCode<U>(detail::getExpression(rhs))
        ) {}

    template<typename U>
    requires (is_allowed_type_v<U> && !std::is_same_v<U, T>)
    ShaderValue(const ShaderValue<U>& other)
        : ShaderValue(
            std::string(opencl_type_name_v<T>),
            makeConversionCode<U>("(" + other.name_ + ")")
        ) {}

    ShaderValue(const ShaderValue<T>& rhs)
        : ShaderValue(
            std::string(opencl_type_name_v<T>),
            rhs.expression_
        ) {}

    // Assignment operators

    template<typename U>
    requires(is_allowed_type_v<U>)
    void operator=(const ShaderValue<U>& rhs) {
        detail::program_context::appendToProgramCode(
            name_ + " = " + makeConversionCode<U>(detail::expr(rhs)) + ";"
        );
    }

    template<typename U>
    requires(is_allowed_type_v<U>)
    void operator=(const ShaderRValue<U>& rhs) {
        detail::program_context::appendToProgramCode(
            name_ + " = " + makeConversionCode<U>(detail::expr(rhs)) + ";"
        );
    }

    void operator=(T v) {
        detail::program_context::appendToProgramCode(
            name_ + " = " + toOpenCLCode(v) + ";"
        );
    }

    operator bool() {
        int32_t res = detail::program_context::get_counter();
        if (res > 0 && detail::program_context::is_forloop_header_being_generated()) {
            detail::program_context::replace_possible_comma_with_semicolon();
            detail::program_context::appendToProgramCode(name_ + ";");
            detail::program_context::replace_possible_comma_with_semicolon();
        }
        HWR_INFO("counter: " + std::to_string(res));
        if (res > 0) detail::program_context::decr_counter();
        detail::program_context::unset_first_def();
        return res > 0;
    }

protected:
    ShaderValue(const std::string& type, const std::string& expr)
        : type_(type), expression_(expr),
          name_(detail::program_context::make_temp_name()),
          def_(type_ + " " + name_ + " = " + expression_ + ";") {
              
        HWR_DEBUG("Appending code to program: " + def_);

        if (detail::program_context::is_forloop_header_being_generated()) {
            if (detail::program_context::is_first_def()) {
                detail::program_context::appendToProgramCode(def_);
            } else {
                detail::program_context::appendToProgramCode(name_ + " = " + expression_ + ";");
            }
        } else {
            detail::program_context::appendToProgramCode(def_);
        }
        detail::program_context::push_opencl_type(type_);
    }

private:
    const std::string type_;
    const std::string expression_;
    const std::string name_;
    const std::string def_;

    template<typename U>
    std::string makeConversionCode(const std::string& expr) const {
        if constexpr (is_shader_convertible_v<U, T>) {
            return expr; // No cast needed
        } else {
            return "(" + std::string(opencl_type_name_v<T>) + ")(" + expr + ")";
        }
    }

    template<typename>
    friend class ShaderValue;
    template<typename>
    friend class ShaderRValue;
    template<typename U>
    friend const std::string& detail::getValName(const ShaderValue<U>& v);
};


namespace detail {

    template<typename T>
    const std::string& getValName(const ShaderValue<T>& shv) {
        return shv.name_;
    }

}

// Used to skip C++ compiler optimization that won't call move ctor like here:
// Float a = b+c;
// a will instantly become the same as b+c, without intermediate call of ctor.
// We don't want that. 
// b+c is a "rvalue", and for generation OpenCL code, a must be a "lvalue".
// By returning wrapper and defining ctor for Float that accepts this wrapper,
// we force Float's ctor to be called.
template<typename U>
class ShaderRValue {
    static_assert(is_allowed_type_v<U>, "U must be valid shader type");
public:
    explicit ShaderRValue(std::string&& expr)
        : expr_(std::move(expr)) {}

    ShaderRValue(U val) :
                    expr_(toOpenCLCode(val)){
    }

    const std::string& expression() const { return expr_; }

    std::string expr_;

    operator bool(){
        int32_t res = detail::program_context::get_counter();
        if(res > 0 && detail::program_context::is_forloop_header_being_generated()){
            detail::program_context::replace_possible_comma_with_semicolon();
            detail::program_context::appendToProgramCode(
               
                expr_+";"
            );
            detail::program_context::replace_possible_comma_with_semicolon();
        }
        HWR_INFO("counter: "+std::to_string(res));
        if(res>0){
            detail::program_context::decr_counter();
        }
        detail::program_context::unset_first_def();
        return res>0;
    }
};

namespace detail {

    // impl of forward declared.
    template<typename T>
    std::string getExpression(const ShaderRValue<T>& wr) {
        static_assert(
            is_allowed_type_v<T>, 
            "T must be a valid shader type"
        );
        return wr.expression();
    }

} // namespace detail


}  // namespace hwr



namespace hwr::detail
{
    // ── String views for the two wrapper classes
    template<typename T> inline std::string expr(const ShaderValue<T>&  v) { 
        return getValName(v); 
    }
    template<typename T> inline std::string expr(const ShaderRValue<T>& v) { 
        return v.expression(); 
    }

    // ── Fallback for a bare scalar/vector T
    template<typename T> inline std::string expr(const T& v)              {
         return toOpenCLCode(v); 
    }

    // Tiny helper that actually assembles "(lhs OP rhs)"
    inline std::string make_expr(const std::string& lhs,
                                 const char*        op,
                                 const std::string& rhs)
    {
        return '(' + lhs + ' ' + op + ' ' + rhs + ')';
    }
} // namespace hwr::detail


HWR_DEFINE_SHADER_BINARY_OP(+)
HWR_DEFINE_SHADER_BINARY_OP(-)
HWR_DEFINE_SHADER_BINARY_OP(*)
HWR_DEFINE_SHADER_BINARY_OP(/)
HWR_DEFINE_SHADER_BINARY_OP(%)
HWR_DEFINE_SHADER_BINARY_OP(<<)
HWR_DEFINE_SHADER_BINARY_OP(>>)
HWR_DEFINE_SHADER_BINARY_OP(&)
HWR_DEFINE_SHADER_BINARY_OP(|)
HWR_DEFINE_SHADER_BINARY_OP(^)

HWR_DEFINE_SHADER_BINARY_OP(+=)
HWR_DEFINE_SHADER_BINARY_OP(-=)
HWR_DEFINE_SHADER_BINARY_OP(*=)
HWR_DEFINE_SHADER_BINARY_OP(/=)
HWR_DEFINE_SHADER_BINARY_OP(%=)
HWR_DEFINE_SHADER_BINARY_OP(<<=)
HWR_DEFINE_SHADER_BINARY_OP(>>=)
HWR_DEFINE_SHADER_BINARY_OP(&=)
HWR_DEFINE_SHADER_BINARY_OP(|=)
HWR_DEFINE_SHADER_BINARY_OP(^=)

HWR_DEFINE_SHADER_BINARY_OP(&&)
HWR_DEFINE_SHADER_BINARY_OP(||)

HWR_DEFINE_SHADER_BINARY_OP(==)
HWR_DEFINE_SHADER_BINARY_OP(!=)
HWR_DEFINE_SHADER_BINARY_OP(<)
HWR_DEFINE_SHADER_BINARY_OP(<=)
HWR_DEFINE_SHADER_BINARY_OP(>)
HWR_DEFINE_SHADER_BINARY_OP(>=)


// === Unary operators ===
namespace hwr {

// ---- Unary minus ----

template<AllowedShaderType T>
requires(!std::is_same_v<T, bool>)
inline ShaderRValue<T> operator-(const ShaderValue<T>& v) {
    return ShaderRValue<T>("(-" + detail::expr(v) + ")");
}

template<AllowedShaderType T>
requires(!std::is_same_v<T, bool>)
inline ShaderRValue<T> operator-(const ShaderRValue<T>& v) {
    return ShaderRValue<T>("(-" + detail::expr(v) + ")");
}

// ---- Prefix ++ / -- ----

template<AllowedShaderType T>
requires(!std::is_same_v<T, bool>)
inline ShaderValue<T>& operator++(ShaderValue<T>& v) { // prefix ++
    detail::program_context::appendToProgramCode(
  
        detail::expr(v) + "++;"
    );
    return v;
}

template<AllowedShaderType T>
requires(!std::is_same_v<T, bool>)
inline ShaderValue<T>& operator--(ShaderValue<T>& v) { // prefix --
    detail::program_context::appendToProgramCode(
        detail::expr(v) + "--;"
    );
    return v;
}

// ---- Postfix ++ / -- ----

template<AllowedShaderType T>
requires(!std::is_same_v<T, bool>)
inline ShaderValue<T>& operator++(ShaderValue<T>& v, int) { // postfix ++
    detail::program_context::appendToProgramCode(
        detail::expr(v) + "++;"
    );
    return v;
}

template<AllowedShaderType T>
requires(!std::is_same_v<T, bool>)
inline ShaderValue<T>& operator--(ShaderValue<T>& v, int) { // postfix --
    detail::program_context::appendToProgramCode(
        detail::expr(v) + "--;"
    );
    return v;
}

// ---- Logical NOT ! ----

template<AllowedShaderType T>
inline ShaderRValue<bool> operator!(const ShaderValue<T>& v) {
    return ShaderRValue<bool>("(!" + detail::expr(v) + ")");
}

template<AllowedShaderType T>
inline ShaderRValue<bool> operator!(const ShaderRValue<T>& v) {
    return ShaderRValue<bool>("(!" + detail::expr(v) + ")");
}

} // namespace hwr

// Helper to extract OpenCL type from a ShaderValue initialization
#define HWR_GET_OPENCL_TYPE(var_type, var_name, var_init)    \
    var_type var_name = var_init;                           \
    std::string _opencl_type = hwr::detail::program_context::pop_opencl_type();

// Helpers to concatenate tokens for unique loop variables
#define HWR_CONCAT_INNER(a, b) a##b
#define HWR_CONCAT(a, b) HWR_CONCAT_INNER(a, b)

#define HWR_STRINGIFY(x) #x

#define HWR_FOR(...) \
auto HWR_CONCAT(_hwr_for_parsed_, __LINE__) = hwr::detail::parseForHeader(HWR_STRINGIFY(__VA_ARGS__));  \
if(!HWR_CONCAT(_hwr_for_parsed_, __LINE__)){    \
    HWR_FATAL("Invalid forloop header"); \
} \
auto [HWR_CONCAT(_hwr_for_init_, __LINE__),HWR_CONCAT(_hwr_for_cond_, __LINE__),HWR_CONCAT(_hwr_for_upd_, __LINE__)] = HWR_CONCAT(_hwr_for_parsed_, __LINE__).value().tie(); \
{ \
    hwr::detail::program_context::appendToProgramCode(std::string("for(")); \
    hwr::detail::program_context::set_forloop_header_generation(); \
    hwr::detail::program_context::incr_counter();                  \
    hwr::detail::program_context::set_first_def();                      \
    for (__VA_ARGS__) {} \
    hwr::detail::program_context::remove_last_char();                  \
    hwr::detail::program_context::appendToProgramCode(std::string("){")); \
    hwr::detail::program_context::unset_forloop_header_generation(); \
} \
hwr::detail::program_context::incr_counter();                  \
int32_t HWR_CONCAT(_hwr_for_declcount_, __LINE__) = hwr::detail::countInitDeclarations(HWR_CONCAT(_hwr_for_init_, __LINE__)); \
hwr::detail::program_context::rollback_name_counter(HWR_CONCAT(_hwr_for_declcount_, __LINE__)); \
int32_t HWR_CONCAT(_hwr_for_updcount_, __LINE__) = hwr::detail::countUpdateOperations(HWR_CONCAT(_hwr_for_upd_, __LINE__)); \
assert(HWR_CONCAT(_hwr_for_declcount_, __LINE__)!=-1); \
assert(HWR_CONCAT(_hwr_for_updcount_, __LINE__)!=-1); \
hwr::detail::program_context::ignore_next_k_appends(HWR_CONCAT(_hwr_for_declcount_, __LINE__)); \
for (__VA_ARGS__,hwr::detail::program_context::undo_last_k_appends(HWR_CONCAT(_hwr_for_updcount_, __LINE__)), hwr::detail::program_context::appendToProgramCode(std::string("}")))  \


         
// C++-style while loop
#define HWR_WHILE(cond)                                                         \
    for (bool HWR_CONCAT(_hwr_while_once_, __LINE__) =                          \
             (hwr::detail::program_context::appendToProgramCode(                \
                  std::string("while(") + hwr::detail::expr(cond) + std::string(") {")), true);                             \
         HWR_CONCAT(_hwr_while_once_, __LINE__);                                \
         hwr::detail::program_context::appendToProgramCode(                     \
             std::string("}")                                                 \
         ), HWR_CONCAT(_hwr_while_once_, __LINE__) = false)

// C++-style if statement injection
#define HWR_IF(cond)                                                             \
    for (bool HWR_CONCAT(_hwr_if_once_, __LINE__) =                             \
             (hwr::detail::program_context::appendToProgramCode(                \
                  std::string("if(") + hwr::detail::expr(cond) + std::string(") {") \
              ), true);                                                        \
         HWR_CONCAT(_hwr_if_once_, __LINE__);                                    \
         hwr::detail::program_context::appendToProgramCode(                     \
             std::string("}")                                                 \
         ), HWR_CONCAT(_hwr_if_once_, __LINE__) = false)

// C++-style else if statement injection
#define HWR_ELSE_IF(cond)                                                        \
    for (bool HWR_CONCAT(_hwr_else_if_once_, __LINE__) =                         \
             (hwr::detail::program_context::appendToProgramCode(                \
                  std::string("else if(") + hwr::detail::expr(cond) + std::string(") {") \
              ), true);                                                        \
         HWR_CONCAT(_hwr_else_if_once_, __LINE__);                               \
         hwr::detail::program_context::appendToProgramCode(                     \
             std::string("}")                                                 \
         ), HWR_CONCAT(_hwr_else_if_once_, __LINE__) = false)

// C++-style else statement injection
#define HWR_ELSE                                                                 \
    for (bool HWR_CONCAT(_hwr_else_once_, __LINE__) =                            \
             (hwr::detail::program_context::appendToProgramCode(                \
                  std::string("else {")                                       \
              ), true);                                                        \
         HWR_CONCAT(_hwr_else_once_, __LINE__);                                  \
         hwr::detail::program_context::appendToProgramCode(                     \
             std::string("}")                                                 \
         ), HWR_CONCAT(_hwr_else_once_, __LINE__) = false)

#endif // HWR_SHADER_HPP
