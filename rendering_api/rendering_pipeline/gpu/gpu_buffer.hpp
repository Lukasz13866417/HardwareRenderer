#ifndef GPU_BUFFER_HPP
#define GPU_BUFFER_HPP

#include "gpu_context.hpp"
#include "../../util/log/log.hpp"
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <algorithm> 
#include <span>

namespace hwr {

// Base class holds the common state.
template<typename T>
class BaseBuffer {
protected:
    size_t m_size;
    const GPUContext& m_ctx;
    cl::Buffer m_buffer;

    BaseBuffer(const GPUContext& ctx, size_t elementCount)
        : m_size(elementCount),m_ctx(ctx)
    {
        static_assert(std::is_trivially_copyable<T>::value, 
                            "T must be trivially copyable");
    }

public:
    virtual ~BaseBuffer() = default;
    size_t size() const { return m_size; }
    // Used when we want to pass it to a kernel. 
    const cl::Buffer& getCLBuffer() const { return m_buffer; }
};

// These flags determine what can be done with the buffer.
// More user-friendly than OpenCL’s raw flags.
// Used only at compile time to deduce actual cl_mem_flags.
enum BufferFlag {
    HOST_WRITE,   // Host is allowed to write (used by writeFrom()).
    HOST_READ,    // Host is allowed to read (used by readTo()).
    GPU_WRITE,    // Device (kernel) is allowed to write.
    GPU_READ,     // Device (kernel) is allowed to read.

    // Aliases to match function static_asserts.
    Writable = HOST_WRITE,   // writeFrom() requires this flag.
    Readable = HOST_READ     // readTo()   requires this flag.
};

// Helper to check if a flag is present in the parameter pack
template<BufferFlag Target, BufferFlag... Flags>
constexpr inline bool has_flag() {
    return ((Flags == Target) || ...);
}

// Helper that translates user-friendly BufferFlags into OpenCL memory flags
template<BufferFlag... Flags>
constexpr inline cl_mem_flags deduceFlags(){
    // Deduce device (GPU) access flags:
    constexpr bool gpuRead  = has_flag<GPU_READ, Flags...>();
    constexpr bool gpuWrite = has_flag<GPU_WRITE, Flags...>();
    cl_mem_flags clFlags = 0;
    if (gpuRead && !gpuWrite){
        clFlags |= CL_MEM_READ_ONLY;
    } else if (gpuWrite && !gpuRead){
        clFlags |= CL_MEM_WRITE_ONLY;
    } else{
        clFlags |= CL_MEM_READ_WRITE; // Default: both read & write.
        // Same as in OpenCL - default flag is CL_MEM_READ_WRITE.
    }

    // Deduce host access flags:
    constexpr bool hostRead  = has_flag<HOST_READ, Flags...>();
    constexpr bool hostWrite = has_flag<HOST_WRITE, Flags...>();
    if (hostRead && !hostWrite)
        clFlags |= CL_MEM_HOST_READ_ONLY;
    else if (hostWrite && !hostRead)
        clFlags |= CL_MEM_HOST_WRITE_ONLY;
    // If both or neither are specified, we don’t restrict host access further.
    return clFlags;
} 

// The GeneralBuffer class deduces the correct OpenCL flags based on provided
// template BufferFlag args. It allows using enqueueWriteBuffer() in writeFrom
// and enqueueReadBuffer() in readTo() if allowed by the host flags.

template<typename T, BufferFlag... Flags>
class GeneralBuffer : public BaseBuffer<T> {
public:
    // 'data' is an optional vector to initialize the buffer.
    GeneralBuffer(const GPUContext& ctx, size_t elementCount, 
                  const std::vector<T> &data = {})
    : BaseBuffer<T>(ctx, elementCount){
        
        cl_mem_flags clFlags = deduceFlags<Flags...>();
        // Use initial data if provided:
        const T* hostPtr = nullptr;
        if (!data.empty()) {
            if (data.size() != elementCount) {
                HWR_FATAL("GeneralBuffer: Data size doesn't match element count");
            }
            clFlags |= CL_MEM_COPY_HOST_PTR;
            hostPtr = data.data();
        }

        // Create the OpenCL buffer.
        this->m_buffer = cl::Buffer(
            ctx.getContext(),
            clFlags,
            sizeof(T) * elementCount,
            // OpenCL wants void* anyway.
            const_cast<void*>(static_cast<const void*>(hostPtr))
        );
    }

    GeneralBuffer(const GPUContext& ctx, size_t elementCount, 
        const std::span<const T> &data)
    : BaseBuffer<T>(ctx, elementCount){

        cl_mem_flags clFlags = deduceFlags<Flags...>();
        // Use initial data if provided:
        if (data.size() != elementCount) {
            HWR_FATAL("GeneralBuffer: Data size doesn't match element count");
        }
        clFlags |= CL_MEM_COPY_HOST_PTR;

        const T* hostPtr = data.data();
        
        // Create the OpenCL buffer.
        this->m_buffer = cl::Buffer(
        ctx.getContext(),
        clFlags,
        sizeof(T) * elementCount,
        // OpenCL wants void* anyway.
        const_cast<void*>(static_cast<const void*>(hostPtr))
        );
    }

    // copies data from a host vector to the device buffer.
    void writeFrom(const std::vector<T>& data) {
        static_assert(has_flag<BufferFlag::Writable, Flags...>(),
                      "writeFrom() called, but BufferFlag::Writable not set.");

        if (data.size() != this->m_size) {
            HWR_FATAL("GeneralBuffer::writeFrom size mismatch");
        }
        #ifndef NDEBUG
            cl_int err = this->m_ctx.getQueue().enqueueWriteBuffer(
                this->m_buffer, CL_TRUE, 0,
                sizeof(T) * this->m_size,
                data.data()
            );
            HWR_ASSERT_CL_OK(err, "GeneralBuffer::writeFrom");
        #else // to prevent warning about unused err
            this->m_ctx.getQueue().enqueueWriteBuffer(
                this->m_buffer, CL_TRUE, 0,
                sizeof(T) * this->m_size,
                data.data()
            );
        #endif
    }


    void writeFrom(std::span<const T> data) {
        static_assert(has_flag<BufferFlag::Writable, Flags...>(),
                    "writeFrom(span) called, but BufferFlag::Writable not set.");

        if (data.size() != this->m_size) {
            HWR_FATAL("GeneralBuffer::writeFrom(span) size mismatch");
        }
        #ifndef NDEBUG
            cl_int err = this->m_ctx.getQueue().enqueueWriteBuffer(
                this->m_buffer, CL_TRUE, 0,
                sizeof(T) * this->m_size,
                data.data()
            );
            HWR_ASSERT_CL_OK(err, "GeneralBuffer::writeFrom(span)");
        #else
            this->m_ctx.getQueue().enqueueWriteBuffer(
                this->m_buffer, CL_TRUE, 0,
                sizeof(T) * this->m_size,
                data.data()
            );
        #endif
    }

    // copies data from the device buffer to a host vector.
    void readTo(std::vector<T>& out) {
        static_assert(has_flag<BufferFlag::Readable, Flags...>(),
                      "readTo() called, but BufferFlag::Readable not set.");

        out.resize(this->m_size);
        #ifndef NDEBUG
            cl_int err = this->m_ctx.getQueue().enqueueReadBuffer(
                this->m_buffer, CL_TRUE, 0,
                sizeof(T) * this->m_size,
                out.data()
            );
            HWR_ASSERT_CL_OK(err, "GeneralBuffer::readTo");
        #else
            this->m_ctx.getQueue().enqueueReadBuffer(
                this->m_buffer, CL_TRUE, 0,
                sizeof(T) * this->m_size,
                out.data()
            );
        #endif
    }

    void readTo(std::span<T> out) {
        static_assert(has_flag<BufferFlag::Readable, Flags...>(),
                      "readTo(span) called, but BufferFlag::Readable not set.");
    
        if (out.size() != this->m_size) {
            HWR_FATAL("GeneralBuffer::readTo(span) size mismatch");
        }
        #ifndef NDEBUG
            cl_int err = this->m_ctx.getQueue().enqueueReadBuffer(
                this->m_buffer, CL_TRUE, 0,
                sizeof(T) * this->m_size,
                out.data()
            );
            HWR_ASSERT_CL_OK(err, "GeneralBuffer::readTo(span)");
        #else
            this->m_ctx.getQueue().enqueueReadBuffer(
                this->m_buffer, CL_TRUE, 0,
                sizeof(T) * this->m_size,
                out.data()
            );
        #endif
    }
};
// nicer-looking aliases for some common flag configs
template<typename T>
using AllPurposeBuffer = GeneralBuffer<T, HOST_READ,HOST_WRITE,GPU_READ,GPU_WRITE>;

template<typename T>
using GPUOnlyBuffer = GeneralBuffer<T, GPU_READ,GPU_WRITE>;

template<typename T>
using ConstBuffer = GeneralBuffer<T, GPU_READ>;

template<typename T>
using ConstBufferHostReadable = GeneralBuffer<T, GPU_READ, HOST_READ>;

template<typename T>
using GPUProducedBuffer = GeneralBuffer<T, GPU_WRITE, HOST_READ>;

template<typename T>
using GPUProducedAndReadBuffer = GeneralBuffer<T, GPU_READ, GPU_WRITE, HOST_READ>;

template<typename T>
using HostProducedBuffer = GeneralBuffer<T, HOST_WRITE, GPU_READ>;

template<typename T>
using HostProducedAndReadBuffer = GeneralBuffer<T, HOST_READ, HOST_WRITE, GPU_READ>;


template<typename T>
class HostMappedBuffer; // Forward declaration

template<typename T>
class MappedPtr {
    friend class HostMappedBuffer<T>;
private:
    T* m_ptr = nullptr;
    HostMappedBuffer<T>* m_owner = nullptr;
    MappedPtr(T* ptr, HostMappedBuffer<T>* owner)
        : m_ptr(ptr), m_owner(owner) {
            HWR_ASSERT(owner->hasNoMappedPointerYet(), 
            "Sanity check failed: creating a mapped pointer when owner already has one");
        }
    void assertValidity(){
        HWR_ASSERT(m_owner->isThisMyPointer(m_ptr), "This MappedPtr is no longer valid");
        HWR_ASSERT(m_ptr, "Dereferencing a null mapped pointer!");
    }

public:
    ~MappedPtr() {
        if (m_owner && m_ptr && m_owner->isThisMyPointer(m_ptr)) {
            m_owner->unmap();  // Will null m_ptr internally too
        }
    }

    // Non-copyable
    MappedPtr(const MappedPtr&) = delete;
    MappedPtr& operator=(const MappedPtr&) = delete;

    // Movable
    MappedPtr(MappedPtr&& other) noexcept
        : m_ptr(other.m_ptr), m_owner(other.m_owner) {
        other.m_ptr = nullptr;
        other.m_owner = nullptr;
    }

    MappedPtr& operator=(MappedPtr&& other) noexcept {
        if (this != &other) {
            if (m_owner && m_ptr) {
                m_owner->unmap();
            }
            m_ptr = other.m_ptr;
            m_owner = other.m_owner;
            other.m_ptr = nullptr;
            other.m_owner = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        assertValidity();
        return *m_ptr;
    }

    T* operator->() const {
        assertValidity();
        return m_ptr;
    }

    explicit operator bool() const {
        return m_ptr != nullptr && m_owner->isThisMyPointer(m_ptr);
    }
};

template<typename T>
class HostMappedBuffer : public BaseBuffer<T> {
    friend class MappedPtr<T>;
private:
    T* m_ptr = nullptr;
    bool m_isMapped = false;
    bool isThisMyPointer(T* what){
        return m_ptr != nullptr && m_isMapped && what == m_ptr;
    }

public:
    using BaseBuffer<T>::m_ctx;
    using BaseBuffer<T>::m_size;
    using BaseBuffer<T>::m_buffer;

    HostMappedBuffer(const GPUContext& ctx, size_t count)
        : BaseBuffer<T>(ctx, count)
    {
        m_buffer = cl::Buffer(
            ctx.getContext(),
            CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,
            sizeof(T) * count
        );
    }

    MappedPtr<T> map(cl_map_flags flags = CL_MAP_READ | CL_MAP_WRITE) {
        if (m_isMapped){
            HWR_FATAL("HostMappedBuffer::map - Buffer already mapped.");
        }

        cl_int err;
        m_ptr = static_cast<T*>(m_ctx.getQueue().enqueueMapBuffer(
            m_buffer, CL_TRUE, flags, 0,
            sizeof(T) * m_size,
            nullptr, nullptr, &err
        ));
        HWR_ASSERT_CL_OK(err, "HostMappedBuffer::map - enqueueMapBuffer");

        MappedPtr<T> res(m_ptr, this);
        m_isMapped = true;
        return res;
    }


    bool hasNoMappedPointerYet(){
        return !m_isMapped;
    }

    void unmap() {
        if (m_ptr) {
            cl_int err = m_ctx.getQueue().enqueueUnmapMemObject(m_buffer, m_ptr);
            m_ctx.getQueue().flush();
            m_ptr = nullptr; 
            m_isMapped = false; // if user tries to use the pointer, they'll get error.
            HWR_ASSERT_CL_OK(err, "HostMappedBuffer::unmap - enqueueUnmapMemObject");
        } else {
            HWR_ERR("Attempted to unmap an already unmapped buffer.");
        }
    }

    ~HostMappedBuffer() {
        if (m_ptr) {
            unmap();
        }
    }
};


} // namespace hwr

#endif // GPU_BUFFER_HPP
