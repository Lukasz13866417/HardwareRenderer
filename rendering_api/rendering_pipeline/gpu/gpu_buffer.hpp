#ifndef GPU_BUFFER_HPP
#define GPU_BUFFER_HPP

#include "gpu_context.hpp"
#include <CL/opencl.hpp>
#include <memory>
#include <type_traits>

namespace hwr {

template<typename T>
class BaseBuffer {
protected:
    cl::Buffer m_buffer;
    size_t m_size;
    const GPUContext& m_ctx;

    BaseBuffer(const GPUContext& ctx, size_t elementCount)
        : m_ctx(ctx), m_size(elementCount) {}

public:
    virtual ~BaseBuffer() = default;
    size_t size() const { return m_size; }
};


template<typename T>
class WriteOnlyBuffer : public BaseBuffer<T> {
public:
    WriteOnlyBuffer(const GPUContext& ctx, size_t elementCount)
        : BaseBuffer<T>(ctx, elementCount)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_WRITE_ONLY,
            sizeof(T) * elementCount
        );
    }

    void writeFrom(const std::vector<T>& data) {
        if (data.size() != this->m_size){
            throw std::runtime_error("WriteOnlyBuffer::writeFrom size mismatch");
            cl_int err = this->m_ctx.getQueue().enqueueWriteBuffer(
                this->m_buffer, CL_TRUE, 0,
                sizeof(T) * this->m_size,
                data.data()
            );
        }
        HWR_ASSERT_CL_OK(err);
    }
};

template<typename T>
class ReadOnlyBuffer : public BaseBuffer<T> {
public:
    ReadOnlyBuffer(const GPUContext& ctx, size_t elementCount, const std::vector<T>& data = {})
        : BaseBuffer<T>(ctx, elementCount)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_READ_ONLY | (data.empty() ? 0 : CL_MEM_COPY_HOST_PTR),
            sizeof(T) * elementCount,
            data.empty() ? nullptr : data.data()
        );
    }

    void readTo(std::vector<T>& out) {
        out.resize(this->m_size);
        cl_int err = this->m_ctx.getQueue().enqueueReadBuffer(
            this->m_buffer, CL_TRUE, 0,
            sizeof(T) * this->m_size,
            out.data()
        );
        HWR_ASSERT_CL_OK(err);
    }
};


template<typename T>
class ReadWriteBuffer : public BaseBuffer<T> {
public:
    ReadWriteBuffer(const GPUContext& ctx, size_t elementCount)
        : BaseBuffer<T>(ctx, elementCount)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_READ_WRITE,
            sizeof(T) * elementCount
        );
    }

    void writeFrom(const std::vector<T>& data) {
        if (data.size() != this->m_size) {
            throw std::runtime_error("ReadWriteBuffer::writeFrom size mismatch");
        }
        cl_int err = this->m_ctx.getQueue().enqueueWriteBuffer(
            this->m_buffer, CL_TRUE, 0,
            sizeof(T) * this->m_size,
            data.data()
        );
        HWR_ASSERT_CL_OK(err);
    }
    

    void readTo(std::vector<T>& out) {
        out.resize(this->m_size);
        cl_int err = this->m_ctx.getQueue().enqueueReadBuffer(
            this->m_buffer, CL_TRUE, 0,
            sizeof(T) * this->m_size,
            out.data()
        );
        HWR_ASSERT_CL_OK(err);
    }
};

template<typename T>
class MappedBuffer : public BaseBuffer<T> {
private:
    T* m_ptr = nullptr;  // pointer to the mapped memory

public:
    MappedBuffer(const GPUContext& ctx, size_t elementCount)
        : BaseBuffer<T>(ctx, elementCount)  // Call base constructor
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,
            sizeof(T) * elementCount
        );
    }

    T* map(cl_map_flags flags = CL_MAP_READ | CL_MAP_WRITE) {
        cl_int err;
        m_ptr = static_cast<T*>(this->m_ctx.getQueue().enqueueMapBuffer(
            this->m_buffer, CL_TRUE, flags, 0,
            sizeof(T) * this->m_size,
            nullptr, nullptr, &err
        ));
        HWR_ASSERT_CL_OK(err);
        return m_ptr;
    }

    void unmap() {
        if (m_ptr) {
            HWR_ASSERT_CL_OK(this->m_ctx.getQueue().enqueueUnmapMemObject(this->m_buffer, m_ptr));
            m_ptr = nullptr;
        }
    }

    ~MappedBuffer() {
        unmap(); 
    }
};


} // namespace hwr

#endif // GPU_BUFFER_HPP
