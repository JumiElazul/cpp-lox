#ifndef JUMI_CPPLOX_ARENA_MEMORY_POOL_H
#define JUMI_CPPLOX_ARENA_MEMORY_POOL_H
#include "typedefs.h"
#include <cstddef>
#include <memory>

NAMESPACE_BEGIN(cpplox)

class arena_memory_pool
{
public:
    explicit arena_memory_pool(std::size_t size);
    ~arena_memory_pool();

    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t));
    [[nodiscard]] std::size_t size() const noexcept;

private:
    void* _memory;
    std::size_t _size;
    std::size_t _offset;

    static std::size_t align_forward_adjustment(const void* address, std::size_t alignment);
};

template<typename T>
struct memory_pool_deleter
{
    arena_memory_pool* pool;
    explicit memory_pool_deleter(arena_memory_pool* pool) : pool(pool) { }

    void operator()(T* ptr)
    {
        if (ptr)
            ptr->~T();
    }
};

template<typename T, typename... Args>
std::unique_ptr<T, memory_pool_deleter<T>> make_unique_mem_pool(arena_memory_pool* pool, Args&&... args)
{
    void* mem = pool->allocate(sizeof(T), alignof(T));
    T* obj = new (mem) T(std::forward<Args>(args)...);
    return std::unique_ptr<T, memory_pool_deleter<T>>(obj, memory_pool_deleter<T>());
}

#ifdef CPPLOX_USE_ARENA_ALLOCATOR
template<typename T>
using unique_ptr_t = std::unique_ptr<T, memory_pool_deleter<T>>;
#else
template<typename T>
using unique_ptr_t = std::unique_ptr<T>;
#endif

NAMESPACE_END

#endif
