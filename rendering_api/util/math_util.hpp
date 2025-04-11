#ifndef MATH_UTIL_HPP
#define MATH_UTIL_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cassert>

namespace hwr {

    struct SimpleVec4 {
        float x, y, z, w;
        float& operator[](size_t i) {
            assert(i < 4); return *(&x + i);
        }
        const float& operator[](size_t i) const {
            assert(i < 4); return *(&x + i);
        }
    };

    struct SafeVec4 {
        union {
            float values[4];
            struct { float x, y, z, w; };
        };
        float& operator[](size_t i) {
            assert(i < 4); return values[i];
        }
        const float& operator[](size_t i) const {
            assert(i < 4); return values[i];
        }
    };

    using vec4f = std::conditional_t<sizeof(SimpleVec4) == 16, SimpleVec4, SafeVec4>;

    static_assert(sizeof(vec4f) == 16, "vec4f must be exactly 16 bytes");

    template<typename T, size_t Rows, size_t Cols>
    struct Matrix {
        static_assert(Rows > 0 && Cols > 0, "Matrix dimensions must be non-zero");
        T data[Rows][Cols];
        constexpr T& operator()(size_t row, size_t col) {
            assert(row < Rows && col < Cols); return data[row][col];
        }
        constexpr const T& operator()(size_t row, size_t col) const {
            assert(row < Rows && col < Cols); return data[row][col];
        }
        T* raw() { return &data[0][0]; }
        const T* raw() const { return &data[0][0]; }
    };

    using mat4f = Matrix<float, 4, 4>;

} // namespace hwr

#endif // MATH_UTIL_HPP
