#ifndef HWR_SHADER_HPP
#define HWR_SHADER_HPP

#include <string>
#include <functional>
#include <stack>

namespace hwr {

// Forward declarations
template<typename T>
class ShaderValueWrapper;

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
    // Each type like Float, Int etc will have such a corresponding specialization
            
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
    friend void detail::program_context::appendToProgramCode(Program& p, const std::string& line);

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
    
    
} // namespace detail


class ShaderValue{

    public:
        const std::string& getDefinition(){
            if(!is_lvalue_){
                HWR_ERR("Trying to get definition of rvalue-like OpenCL variable");
                return expression_;
            }
            return def_;
        }

    protected:
        ShaderValue(const std::string& type, const std::string& expr, bool is_lvalue)
            : type_(type), expression_(expr),
            is_lvalue_(is_lvalue),
            name_(is_lvalue 
                ?   detail::make_temp_name() 
                    : detail::COMMON_RVALUE_NAME),
            def_(is_lvalue 
                ?   type_ + " " + name_ + " = " + expression_ + ";" 
                    : detail::COMMON_RVALUE_DEFINITION)
            {
                if (is_lvalue_) {
                    HWR_DEBUG("Appending code to program: " + def_);
                    detail::program_context::appendToProgramCode(
                        detail::program_context::current_program(), def_);
                } else {
                    HWR_DEBUG("Not appending — rvalue");
                }
            }

        const std::string& getName() const {
            return name_;
        }
    
        const std::string& getExpression() const {
            return expression_;
        }
    
        bool isLValue() const {
            return is_lvalue_;
        }

    private:
        const std::string type_;
        const std::string expression_;
        bool is_lvalue_;
        const std::string name_;
        const std::string def_;

    template<typename T>
    friend class ShaderValueWrapper;
    
   

};

namespace detail{
    template<typename T>
    using is_shader_value_subclass = std::is_base_of<hwr::ShaderValue, T>;

    template<typename T>
    inline constexpr bool is_shader_value_subclass_v = is_shader_value_subclass<T>::value;
}

// Used to skip C++ compiler optimization that won't call move ctor like here:
// Float a = b+c;
// a will immediatelly become the same as b+c, without intermediate call of ctor.
// We don't want that. 
// b+c is a "rvalue", and for generation OpenCL code, a must be a "lvalue".
// By returning a wrapper and defining a ctor for Float that accepts this wrapper,
// we force Float's ctor to be called.
template<typename SV>
class ShaderValueWrapper {
    static_assert(std::is_base_of_v<hwr::ShaderValue, SV>, "SV must be a ShaderValue subclass");

public:
    explicit ShaderValueWrapper(std::string expr)
        : expr_(std::move(expr)) {}

    const std::string& expression() const { return expr_; }

private:
    std::string expr_;

// Arithmetic ops: wrapper op T
template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator+(const hwr::ShaderValueWrapper<T>& lhs, const T& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " + " + rhs.getName() + ")");
}

template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator-(const hwr::ShaderValueWrapper<T>& lhs, const T& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " - " + rhs.getName() + ")");
}

template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator*(const hwr::ShaderValueWrapper<T>& lhs, const T& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " * " + rhs.getName() + ")");
}

template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator/(const hwr::ShaderValueWrapper<T>& lhs, const T& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " / " + rhs.getName() + ")");
}

// Arithmetic ops: T op wrapper
template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator+(const T& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.getName() + " + " + rhs.expression() + ")");
}

template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator-(const T& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.getName() + " - " + rhs.expression() + ")");
}

template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator*(const T& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.getName() + " * " + rhs.expression() + ")");
}

template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator/(const T& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.getName() + " / " + rhs.expression() + ")");
}

// Chained: wrapper op wrapper
template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator+(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " + " + rhs.expression() + ")");
}

template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator-(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " - " + rhs.expression() + ")");
}

template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator*(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " * " + rhs.expression() + ")");
}

template<typename T, typename = std::enable_if_t<hwr::detail::is_shader_value_subclass_v<T>>>
friend hwr::ShaderValueWrapper<T> operator/(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " / " + rhs.expression() + ")");
}

};


}  // namespace hwr


#endif // HWR_SHADER_HPP
