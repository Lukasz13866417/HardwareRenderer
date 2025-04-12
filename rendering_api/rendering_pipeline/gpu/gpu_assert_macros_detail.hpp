#ifndef GPU_ASSERT_MACROS_DETAIL_HPP
#define GPU_ASSERT_MACROS_DETAIL_HPP
#define CL_HPP_TARGET_OPENCL_VERSION 300

#ifdef NDEBUG
#define NO_CL_ASSERT 1
#endif

#ifndef NO_CL_ASSERT
#define NO_CL_ASSERT 0
#endif

#if NO_CL_ASSERT == 0

#define __HWR_GET_MACRO(_1, _2, NAME, ...) NAME
#define __HWR_ASSERT_CL_OK_1(code)           hwr::__assert_cl_ok(code)
#define __HWR_ASSERT_CL_OK_2(code, when)     hwr::__assert_cl_ok(code, when)

#define HWR_ASSERT_CL_OK(...) __HWR_GET_MACRO(__VA_ARGS__, __HWR_ASSERT_CL_OK_2, __HWR_ASSERT_CL_OK_1)(__VA_ARGS__)

#else

#define HWR_ASSERT_CL_OK(...) (void*)(nullptr)

#endif

#endif //GPU_ASSERT_MACROS_DETAIL_HPP