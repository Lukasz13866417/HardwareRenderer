#ifndef HWR_SHADER_HPP
#define HWR_SHADER_HPP

#include <string>
#include <functional>
#include <stack>

#include "./shader_types_util.hpp"
#include "./static_string.hpp"

#define HWR_DEFINE_SHADER_BINARY_OP(OP)                                                        \
template<hwr::AllowedShaderType T, hwr::AllowedShaderType U>                                   \
requires hwr::is_op_allowed_v<T, U, hwr::StaticString{#OP}>                                    \
inline hwr::ShaderRValue<hwr::binary_result_type_t<T, U, hwr::StaticString{#OP}>>              \
operator OP(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValue<U>& rhs)                   \
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

template<typename T>
class ShaderRValue;

template<typename T>
class ShaderValue;

class Program;


namespace detail {

    namespace program_context {

       static std::stack<Program*> s_program_stack;

        void push_program(Program& p) {
            s_program_stack.push(&p);
        }

        void pop_program() {
            if (!s_program_stack.empty()) {
                s_program_stack.pop();
            }
        }

        Program& current_program() {
            assert(!s_program_stack.empty());
            return *s_program_stack.top();
        }

        // implemented later in the file
        void appendToProgramCode(Program& p, const std::string& line); 

    }  // namespace program_context 
    


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
    std::string code_;
    std::function<void()> compilable_fn_;
    bool compiled_ = false;

    void append(const std::string& line) {
        code_ += line + "\n";
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
        return code_;
    }

    // Accessor for ProgramContext
    friend void detail::program_context::push_program(Program& p);
    friend void detail::program_context::appendToProgramCode(Program& p, 
                                                        const std::string& s);

}; 

namespace detail {
    namespace program_context {
        inline void appendToProgramCode(Program& p, const std::string& line) {
            p.append(line);
        }
    }

    inline int temp_counter = 0;

    inline std::string make_temp_name() {
        return "tmp" + std::to_string(temp_counter++);
    }

    const std::string COMMON_RVALUE_NAME = "";
    const std::string COMMON_RVALUE_DEFINITION = "";
    
    

    template<typename T>
    std::string getExpression(const ShaderRValue<T>& wrapper);

    template<typename U>
    const std::string& getValName(const ShaderValue<U>& shv);

} // namespace detail

template<typename T>
class ShaderValue{
    static_assert(is_allowed_type_v<T>, "T must be valid shader type");
    public:
        const std::string& getDefinition(){
            return def_;
        }

        public:
            ShaderValue(const ShaderRValue<T>& rhs)
            : ShaderValue(
                std::string(opencl_type_name_v<T>),  // type
                detail::getExpression(rhs)  // expression
            ) {}
    

            ShaderValue(T from) 
                : ShaderValue(std::string(opencl_type_name_v<T>), 
                              toOpenCLCode(from))
                         {
            }

            template<typename U>
            requires (
                is_allowed_type_v<U> &&
                is_shader_convertible_v<U, T> &&
                !std::is_same_v<U, T>
            )
            ShaderValue(const ShaderRValue<U>& rhs)
            : ShaderValue(
                std::string(opencl_type_name_v<T>),  // type
                "(" + std::string(opencl_type_name_v<T>) + ")"
                + detail::getExpression(rhs)  // expression
            ) {}


            template<typename U>
            requires (
                is_allowed_type_v<U> &&
                is_shader_convertible_v<U, T> &&
                !std::is_same_v<U, T>
            )
            ShaderValue(const ShaderValue<U>& other)
                : ShaderValue(std::string(opencl_type_name_v<T>), 
                              "(" + std::string(opencl_type_name_v<T>) + ")(" + other.name_ + ")")
            {}

            ShaderValue(const ShaderValue<T>& rhs)
                : ShaderValue(std::string(opencl_type_name_v<T>), rhs.expression_)
                {}

            

    protected:
        ShaderValue(const std::string& type, const std::string& expr)
            : type_(type), expression_(expr),
              name_(detail::make_temp_name()),
              def_ (type_ + " " + name_ + " = " + expression_ + ";" )
            {
                HWR_DEBUG("Appending code to program: " + def_);
                detail::program_context::appendToProgramCode(
                    detail::program_context::current_program(), def_);
            }

        template<typename U>
        friend const std::string& detail::getValName(const ShaderValue<U>& v);
           

    private:
        const std::string type_;
        const std::string expression_;
        const std::string name_;
        const std::string def_;
    
        

    template<typename>
    friend class ShaderRValue;

    template<typename>
    friend class ShaderValue;

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


// === Global operator overloads for ShaderValue ===b

HWR_DEFINE_SHADER_BINARY_OP(+)
HWR_DEFINE_SHADER_BINARY_OP(-)
HWR_DEFINE_SHADER_BINARY_OP(*)
HWR_DEFINE_SHADER_BINARY_OP(/)
HWR_DEFINE_SHADER_BINARY_OP(==)
HWR_DEFINE_SHADER_BINARY_OP(<=)
HWR_DEFINE_SHADER_BINARY_OP(>=)
HWR_DEFINE_SHADER_BINARY_OP(>)
HWR_DEFINE_SHADER_BINARY_OP(<)


#endif // HWR_SHADER_HPP
