#include<hwr/buffer.hpp>
#include<hwr/math.hpp>
#include<hwr/log.hpp>
#include "./shader.hpp"

class Float : public hwr::ShaderValue {

    private:
        // Constructor for expressions (rvalue-like)
        Float(const std::string& expr, bool is_lvalue = false)
            : ShaderValue(CL_TYPE_NAME, expr, is_lvalue)
        {
            HWR_DEBUG("rvalue float created: " + expr);
        }

    public:
        static const inline std::string CL_TYPE_NAME = "float";
    
        // Constructor for literals / named values (lvalue)
        Float(float f)
            : ShaderValue(CL_TYPE_NAME, std::to_string(f), true)
        {
            HWR_DEBUG("lvalue float created");
        }

        // situations like Float c = a+b;
        Float(const hwr::ShaderValueWrapper<Float>& wrapper)
            : ShaderValue(CL_TYPE_NAME, wrapper.expression(), true)
        {
            HWR_DEBUG("constructed Float from ShaderValueWrapper");
        }


        // situations like Float a = b;
        Float(const Float& other)
            : ShaderValue(CL_TYPE_NAME, other.getExpression(), true) 
        {
            HWR_DEBUG("promoted rvalue float to lvalue");
        }

        friend hwr::ShaderValueWrapper<Float> operator+(const Float& a, const Float& b) {
            return hwr::ShaderValueWrapper<Float>("(" + a.getName() + " + " + b.getName() + ")");
        }
        
        friend hwr::ShaderValueWrapper<Float> operator-(const Float& a, const Float& b) {
            return hwr::ShaderValueWrapper<Float>("(" + a.getName() + " - " + b.getName() + ")");
        }

        friend hwr::ShaderValueWrapper<Float> operator*(const Float& a, const Float& b) {
            return hwr::ShaderValueWrapper<Float>("(" + a.getName() + " * " + b.getName() + ")");
        }
        
        friend hwr::ShaderValueWrapper<Float> operator/(const Float& a, const Float& b) {
            return hwr::ShaderValueWrapper<Float>("(" + a.getName() + " / " + b.getName() + ")");
        }
        
        // etc...
        

};

template<>
struct hwr::detail::is_shader_type<Float> : std::true_type {};
