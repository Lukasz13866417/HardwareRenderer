#include "math_util.hpp"

namespace hwr {

    vec4f vec_add(const vec4f& a, const vec4f& b) {
        return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
    }

    vec4f vec_sub(const vec4f& a, const vec4f& b) {
        return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
    }

    vec4f vec_scale(const vec4f& v, float scalar) {
        return { v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar };
    }

    float vec_dot(const vec4f& a, const vec4f& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    float vec_length(const vec4f& v) {
        return std::sqrt(vec_dot(v, v));
    }

    vec4f vec_normalize(const vec4f& v) {
        float len = vec_length(v);
        if (len == 0.0f) return {0, 0, 0, 0};
        return vec_scale(v, 1.0f / len);
    }



    mat4f mat_identity() {
        mat4f m = {};
        for (int i = 0; i < 4; ++i)
            m.m[i][i] = 1.0f;
        return m;
    }

    mat4f mat_transpose(const mat4f& in) {
        mat4f out = {};
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                out.m[i][j] = in.m[j][i];
        return out;
    }

    mat4f mat_mul(const mat4f& a, const mat4f& b) {
        mat4f result = {};
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                for (int k = 0; k < 4; ++k) {
                    result.m[row][col] += a.m[row][k] * b.m[k][col];
                }
            }
        }
        return result;
    }

    vec4f mat_mul_vec(const mat4f& m, const vec4f& v) {
        return {
            m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z + m.m[0][3]*v.w,
            m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z + m.m[1][3]*v.w,
            m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z + m.m[2][3]*v.w,
            m.m[3][0]*v.x + m.m[3][1]*v.y + m.m[3][2]*v.z + m.m[3][3]*v.w
        };
    }

    std::string to_string(const vec4f& v) {
        return "vec4f(" +
            std::to_string(v.x) + ", " +
            std::to_string(v.y) + ", " +
            std::to_string(v.z) + ", " +
            std::to_string(v.w) + ")";
    }
    
    std::string to_string(const mat4f& m) {
        std::string out = "mat4f(\n";
        for (int i = 0; i < 4; ++i) {
            out += "  [ ";
            for (int j = 0; j < 4; ++j) {
                out += std::to_string(m.m[i][j]) + (j < 3 ? ", " : "");
            }
            out += " ]\n";
        }
        out += ")";
        return out;
    }

} // namespace hwr
