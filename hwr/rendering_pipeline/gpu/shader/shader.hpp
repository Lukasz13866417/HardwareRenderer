#ifndef HWR_SHADER_HPP
#define HWR_SHADER_HPP

#include <string>
#include <functional>
#include <stack>

#include "./shader_types_util.hpp"

namespace hwr {

template<typename T>
class ShaderValueWrapper;

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
    
    

    // stupid wrapper, necessary only due to C++ compiler imperfections.
    template<typename T>
    std::tuple<std::string, std::string, bool> stupidWrapper(const ShaderValueWrapper<T>& wrapper);

    template<typename U>
    const std::string& getValName(const ShaderValue<U>& shv);

} // namespace detail

template<typename T>
class ShaderValue{
    static_assert(detail::is_allowed_type_v<T>, "T must be a valid shader type");
    public:
        const std::string& getDefinition(){
            if(!is_lvalue_){
                HWR_ERR("Trying to get definition of rvalue-like OpenCL variable");
                return expression_;
            }
            return def_;
        }

        public:
            ShaderValue(const ShaderValueWrapper<T>& wrapper)
            : ShaderValue(
                std::get<0>(detail::stupidWrapper(wrapper)),  // type
                std::get<1>(detail::stupidWrapper(wrapper)),  // expression
                std::get<2>(detail::stupidWrapper(wrapper))   // is_lvalue
            ) {}
    

            ShaderValue(T from) 
                : ShaderValue(std::string(opencl_type_name_v<T>), 
                              std::to_string(from), true)
                         {
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

        template<typename U>
        friend const std::string& detail::getValName(const ShaderValue<U>& shv);
           

    private:
        const std::string type_;
        const std::string expression_;
        const bool is_lvalue_;
        const std::string name_;
        const std::string def_;
    
        

    template<typename>
    friend class ShaderValueWrapper;

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
class ShaderValueWrapper {
    static_assert(detail::is_allowed_type_v<U>, "U must be a valid shader type");
public:
    explicit ShaderValueWrapper(std::string&& expr)
        : expr_(std::move(expr)) {}

    ShaderValueWrapper(U val) :
                    expr_(std::to_string(val)){
    }

    const std::string& expression() const { return expr_; }

    std::string expr_;

};


namespace detail {

    // stupid impl
    template<typename T>
    std::tuple<std::string, std::string, bool> stupidWrapper(const ShaderValueWrapper<T>& wrapper) {
        static_assert(detail::is_allowed_type_v<T>, "T must be a valid shader type");
        return { std::string(opencl_type_name_v<T>), wrapper.expression(), true };
    }

} // namespace detail


}  // namespace hwr


// === Global operator overloads for ShaderValue ===b

template<typename T>
hwr::ShaderValueWrapper<T> operator+(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " + " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator-(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " - " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator*(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " * " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator/(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " / " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator+(const hwr::ShaderValue<T>& lhs, const T& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " + " + std::to_string(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator-(const hwr::ShaderValue<T>& lhs, const T& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " - " + std::to_string(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator*(const hwr::ShaderValue<T>& lhs, const T& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " * " + std::to_string(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator/(const hwr::ShaderValue<T>& lhs, const T& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " / " + std::to_string(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator+(const T& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + std::to_string(lhs) + " + " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator-(const T& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + std::to_string(lhs) + " - " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator*(const T& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + std::to_string(lhs) + " * " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator/(const T& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + std::to_string(lhs) + " / " + hwr::detail::getValName(rhs) + ")");
}

// === ShaderValue<T> op ShaderValueWrapper<T> ===

template<typename T>
inline hwr::ShaderValueWrapper<T> operator+(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " + " + rhs.expression() + ")");
}

template<typename T>
inline hwr::ShaderValueWrapper<T> operator-(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " - " + rhs.expression() + ")");
}

template<typename T>
inline hwr::ShaderValueWrapper<T> operator*(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " * " + rhs.expression() + ")");
}

template<typename T>
inline hwr::ShaderValueWrapper<T> operator/(const hwr::ShaderValue<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + hwr::detail::getValName(lhs) + " / " + rhs.expression() + ")");
}

// === ShaderValueWrapper<T> op ShaderValue<T> ===

template<typename T>
inline hwr::ShaderValueWrapper<T> operator+(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " + " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
inline hwr::ShaderValueWrapper<T> operator-(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " - " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
inline hwr::ShaderValueWrapper<T> operator*(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " * " + hwr::detail::getValName(rhs) + ")");
}

template<typename T>
inline hwr::ShaderValueWrapper<T> operator/(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValue<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " / " + hwr::detail::getValName(rhs) + ")");
}



// Arithmetic ops: wrapper op T
template<typename T>
hwr::ShaderValueWrapper<T> operator+(const hwr::ShaderValueWrapper<T>& lhs, const T& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " + " + std::to_string(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator-(const hwr::ShaderValueWrapper<T>& lhs, const T& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " - " + std::to_string(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator*(const hwr::ShaderValueWrapper<T>& lhs, const T& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " * " + std::to_string(rhs) + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator/(const hwr::ShaderValueWrapper<T>& lhs, const T& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " / " + std::to_string(rhs) + ")");
}

// Arithmetic ops: T op wrapper
template<typename T>
hwr::ShaderValueWrapper<T> operator+(const T& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + std::to_string(rhs) + " + " + rhs.expression() + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator-(const T& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + std::to_string(rhs) + " - " + rhs.expression() + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator*(const T& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + std::to_string(rhs) + " * " + rhs.expression() + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator/(const T& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + std::to_string(rhs) + " / " + rhs.expression() + ")");
}

// Chained: wrapper op wrapper
template<typename T>
hwr::ShaderValueWrapper<T> operator+(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " + " + rhs.expression() + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator-(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " - " + rhs.expression() + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator*(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " * " + rhs.expression() + ")");
}

template<typename T>
hwr::ShaderValueWrapper<T> operator/(const hwr::ShaderValueWrapper<T>& lhs, const hwr::ShaderValueWrapper<T>& rhs) {
    static_assert(hwr::detail::is_allowed_type_v<T>, "T must be a valid shader type");
    return hwr::ShaderValueWrapper<T>("(" + lhs.expression() + " / " + rhs.expression() + ")");
}

#endif // HWR_SHADER_HPP
