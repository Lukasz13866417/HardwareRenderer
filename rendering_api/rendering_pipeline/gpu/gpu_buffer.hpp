#ifndef GPU_BUFFER_HPP
#define GPU_BUFFER_HPP

#include "gpu_context.hpp"
#include <CL/opencl.hpp>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <algorithm>  // for std::copy

namespace hwr {

// Base class holds the common state.
template<typename T>
class BaseBuffer {
protected:
    cl::Buffer m_buffer;
    size_t m_size;
    const GPUContext& m_ctx;

    BaseBuffer(const GPUContext& ctx, size_t elementCount)
        : m_ctx(ctx), m_size(elementCount)
    {
        static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
    }

public:
    virtual ~BaseBuffer() = default;
    size_t size() const { return m_size; }
    const cl::Buffer& getCLBuffer() const { return m_buffer; }
};

// kernels&host can both read and write.
template<typename T>
class GeneralBuffer : public BaseBuffer<T> {
public:
    GeneralBuffer(const GPUContext& ctx, size_t elementCount)
        : BaseBuffer<T>(ctx, elementCount)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_READ_WRITE,
            sizeof(T) * elementCount
        );
    }

    // Host writes data to the device buffer.
    void writeFrom(const std::vector<T>& data) {
        if (data.size() != this->m_size) {
            HWR_FATAL("GeneralBuffer::writeFrom size mismatch");
        }
        cl_int err = this->m_ctx.getQueue().enqueueWriteBuffer(
            this->m_buffer, CL_TRUE, 0,
            sizeof(T) * this->m_size,
            data.data()
        );
        ASSERT_CL_OK(err);
    }
    
    // Host reads data from the device buffer.
    void readTo(std::vector<T>& out) {
        out.resize(this->m_size);
        cl_int err = this->m_ctx.getQueue().enqueueReadBuffer(
            this->m_buffer, CL_TRUE, 0,
            sizeof(T) * this->m_size,
            out.data()
        );
        ASSERT_CL_OK(err);
    }
};


// mapping gives direct host access.
template<typename T>
class HostMappedBuffer : public BaseBuffer<T> {
private:
    T* m_ptr = nullptr;
    bool m_isMapped = false;

public:
    HostMappedBuffer(const GPUContext& ctx, size_t count)
        : BaseBuffer<T>(ctx, count)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,
            sizeof(T) * count
        );
    }

    // Map the buffer. (User can then modify the memory directly.)
    T* map(cl_map_flags flags = CL_MAP_READ | CL_MAP_WRITE) {
        if (m_isMapped){
            HWR_FATAL("Buffer already mapped.");
        }
        cl_int err;
        m_ptr = static_cast<T*>(this->m_ctx.getQueue().enqueueMapBuffer(
            this->m_buffer, CL_TRUE, flags, 0,
            sizeof(T) * this->m_size,
            nullptr, nullptr, &err
        ));
        ASSERT_CL_OK(err);
        m_isMapped = true;
        return m_ptr;
    }

    // Unmap the buffer.
    void unmap() {
        if (m_ptr) {
            cl_int err = this->m_ctx.getQueue().enqueueUnmapMemObject(this->m_buffer, m_ptr);
            ASSERT_CL_OK(err);
            m_ptr = nullptr;
            m_isMapped = false;
        }
    }

    // Convenience write: map, copy, unmap.
    void writeFrom(const std::vector<T>& data) {
        if (data.size() != this->m_size){
            HWR_FATAL("HostMappedBuffer::writeFrom size mismatch");
        }
        T* ptr = this->map(CL_MAP_WRITE);
        std::copy(data.begin(), data.end(), ptr);
        this->unmap();
    }

    // Convenience read: map, copy, unmap.
    void readTo(std::vector<T>& out) {
        out.resize(this->m_size);
        T* ptr = this->map(CL_MAP_READ);
        std::copy(ptr, ptr + this->m_size, out.begin());
        this->unmap();
    }

    ~HostMappedBuffer() {
        if (m_ptr) unmap();
    }
};

// for buffers that the device reads (CL_MEM_READ_ONLY)
// Host writes the data (writeFrom provided).
template<typename T>
class HostMappedInputBuffer : public BaseBuffer<T> {
public:
    HostMappedInputBuffer(const GPUContext& ctx, size_t count)
        : BaseBuffer<T>(ctx, count)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR,
            sizeof(T) * count
        );
    }

    // Write data by mapping with write access.
    void writeFrom(const std::vector<T>& data) {
        if (data.size() != this->m_size){
            HWR_FATAL("HostMappedInputBuffer::writeFrom size mismatch");
        }
        cl_int err;
        T* ptr = static_cast<T*>(this->m_ctx.getQueue().enqueueMapBuffer(
            this->m_buffer, CL_TRUE, CL_MAP_WRITE, 0,
            sizeof(T) * this->m_size, nullptr, nullptr, &err
        ));
        ASSERT_CL_OK(err);
        std::copy(data.begin(), data.end(), ptr);
        err = this->m_ctx.getQueue().enqueueUnmapMemObject(this->m_buffer, ptr);
        ASSERT_CL_OK(err);
    }
};

// For buffers that the device writes to (CL_MEM_WRITE_ONLY)
// Host reads the data (readTo provided).
template<typename T>
class HostMappedOutputBuffer : public BaseBuffer<T> {
public:
    HostMappedOutputBuffer(const GPUContext& ctx, size_t count)
        : BaseBuffer<T>(ctx, count)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR,
            sizeof(T) * count
        );
    }

    // Read data by mapping with read access.
    void readTo(std::vector<T>& out) {
        out.resize(this->m_size);
        cl_int err;
        T* ptr = static_cast<T*>(this->m_ctx.getQueue().enqueueMapBuffer(
            this->m_buffer, CL_TRUE, CL_MAP_READ, 0,
            sizeof(T) * this->m_size, nullptr, nullptr, &err
        ));
        ASSERT_CL_OK(err);
        std::copy(ptr, ptr + this->m_size, out.begin());
        err = this->m_ctx.getQueue().enqueueUnmapMemObject(this->m_buffer, ptr);
        ASSERT_CL_OK(err);
    }
};

// for input buffers (CL_MEM_READ_ONLY) where the host provides
// a pointer to pre-allocated memory. Host is allowed to write to the host memory before use.
template<typename T>
class HostPinnedInputBuffer : public BaseBuffer<T> {
public:
    HostPinnedInputBuffer(const GPUContext& ctx, T* hostPtr, size_t count)
        : BaseBuffer<T>(ctx, count)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
            sizeof(T) * count,
            hostPtr
        );
    }

    // Write data to the underlying host pointer using enqueueWriteBuffer.
    void writeFrom(const std::vector<T>& data) {
        if (data.size() != this->m_size){
            HWR_FATAL("HostPinnedInputBuffer::writeFrom size mismatch");
        }
        cl_int err = this->m_ctx.getQueue().enqueueWriteBuffer(
            this->m_buffer, CL_TRUE, 0,
            sizeof(T) * this->m_size,
            data.data()
        );
        ASSERT_CL_OK(err);
    }
};

// for output buffers where the host provides
// a pointer to pre-allocated memory. Host can read the results.
template<typename T>
class HostPinnedOutputBuffer : public BaseBuffer<T> {
public:
    HostPinnedOutputBuffer(const GPUContext& ctx, T* hostPtr, size_t count)
        : BaseBuffer<T>(ctx, count)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
            sizeof(T) * count,
            hostPtr
        );
    }

    // Read data from the underlying host pointer using enqueueReadBuffer.
    void readTo(std::vector<T>& out) {
        out.resize(this->m_size);
        cl_int err = this->m_ctx.getQueue().enqueueReadBuffer(
            this->m_buffer, CL_TRUE, 0,
            sizeof(T) * this->m_size,
            out.data()
        );
        ASSERT_CL_OK(err);
    }
};

// HostPinnedReadWriteBuffer: using CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR.
// Both writing to and reading from the buffer are supported via queue operations.
template<typename T>
class HostPinnedReadWriteBuffer : public BaseBuffer<T> {
public:
    HostPinnedReadWriteBuffer(const GPUContext& ctx, T* hostPtr, size_t count)
        : BaseBuffer<T>(ctx, count)
    {
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
            sizeof(T) * count,
            hostPtr
        );
    }

    void writeFrom(const std::vector<T>& data) {
        if (data.size() != this->m_size){
            HWR_FATAL("HostPinnedReadWriteBuffer::writeFrom size mismatch");
        }
        cl_int err = this->m_ctx.getQueue().enqueueWriteBuffer(
            this->m_buffer, CL_TRUE, 0,
            sizeof(T) * this->m_size,
            data.data()
        );
        ASSERT_CL_OK(err);
    }
    
    void readTo(std::vector<T>& out) {
        out.resize(this->m_size);
        cl_int err = this->m_ctx.getQueue().enqueueReadBuffer(
            this->m_buffer, CL_TRUE, 0,
            sizeof(T) * this->m_size,
            out.data()
        );
        ASSERT_CL_OK(err);
    }
};

} // namespace hwr

#endif // GPU_BUFFER_HPP
