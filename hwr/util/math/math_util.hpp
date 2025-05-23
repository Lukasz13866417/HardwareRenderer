#ifndef HWR_MATH_UTIL_HPP
#define HWR_MATH_UTIL_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <cmath> 
#include <string>

namespace hwr {

    namespace detail {

        struct SimpleVec4 {
            float x, y, z, w;
        };

       

        #if defined(__clang__) || defined(__GNUC__)
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wpedantic"
        #elif defined(_MSC_VER)
            #pragma warning(push)
            #pragma warning(disable : 4201)
        #endif

        struct SafeVec4 {
            union {
                float values[4];
                struct { float x, y, z, w; };
            };
        };

        #if defined(__clang__) || defined(__GNUC__)
            #pragma GCC diagnostic pop
        #elif defined(_MSC_VER)
            #pragma warning(pop)
        #endif

       

    }; // namespace detail

    using vec4f = std::conditional_t<
        sizeof(detail::SimpleVec4) == 16,
        detail::SimpleVec4,
        detail::SafeVec4
    >;

    static_assert(sizeof(vec4f) == 16, "vec4f must be exactly 16 bytes");

    struct mat4f {
        float m[4][4]; // row-major
    };

    static_assert(sizeof(mat4f) == 64, "mat4f must be exactly 64 bytes");


    vec4f vec_add(const vec4f& a, const vec4f& b);
    vec4f vec_sub(const vec4f& a, const vec4f& b);
    vec4f vec_scale(const vec4f& v, float scalar);
    float vec_dot(const vec4f& a, const vec4f& b);
    float vec_length(const vec4f& v);
    vec4f vec_normalize(const vec4f& v);


    mat4f mat_identity();
    mat4f mat_transpose(const mat4f& m);
    mat4f mat_mul(const mat4f& a, const mat4f& b);
    vec4f mat_mul_vec(const mat4f& m, const vec4f& v);



    std::string to_string(const vec4f& v);
    std::string to_string(const mat4f& m);

} // namespace hwr

#endif // HWR_MATH_UTIL_HPP
