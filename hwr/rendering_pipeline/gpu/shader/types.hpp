#ifndef SHADER_TYPES_HPP
#define SHADER_TYPES_HPP

#include<hwr/buffer.hpp>
#include<hwr/math.hpp>
#include<hwr/log.hpp>
#include "./shader.hpp"

using Float = hwr::ShaderValue<float>;
using Int = hwr::ShaderValue<int32_t>;
using Int64 = hwr::ShaderValue<int64_t>;
using Int32 = hwr::ShaderValue<int32_t>;
using Int16 = hwr::ShaderValue<int16_t>;
using Int8 = hwr::ShaderValue<int8_t>;
using UInt = hwr::ShaderValue<uint32_t>;
using UInt64 = hwr::ShaderValue<uint64_t>;
using UInt32 = hwr::ShaderValue<uint32_t>;
using UInt16 = hwr::ShaderValue<uint16_t>;
using UInt8 = hwr::ShaderValue<uint8_t>;


using Double = hwr::ShaderValue<double>;
using Bool = hwr::ShaderValue<bool>;

#endif // SHADER_TYPES_HPP