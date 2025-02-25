#include "arena_memory_pool.h"
#include "typedefs.h"
#include <cstddef>
#include <new>

NAMESPACE_BEGIN(geo)

arena_memory_pool::arena_memory_pool(std::size_t size)
    : _size(size)
    , _offset(0)
{ 
    _memory = ::operator new(size);
}

arena_memory_pool::~arena_memory_pool()
{
    ::operator delete(_memory);
}

void* arena_memory_pool::allocate(std::size_t size, std::size_t alignment)
{
    const std::size_t header_size = sizeof(std::size_t);

    std::size_t current_address = reinterpret_cast<std::size_t>(_memory) + _offset;
    std::size_t adjustment = align_forward_adjustment(reinterpret_cast<void*>(current_address + header_size), alignment);

    if (_offset + adjustment + size > _size)
        throw std::bad_alloc();

    _offset += adjustment;

    std::size_t* header = reinterpret_cast<std::size_t*>(reinterpret_cast<unsigned char*>(_memory) + _offset);
    *header = adjustment;
    _offset += header_size;

    void* aligned_address = reinterpret_cast<void*>(current_address + adjustment);
    _offset += size;
    return aligned_address;
}

std::size_t arena_memory_pool::size() const noexcept { return _size; }

std::size_t arena_memory_pool::align_forward_adjustment(const void* address, std::size_t alignment)
{
    std::size_t addr = reinterpret_cast<std::size_t>(address);
    std::size_t misalignment = addr & (alignment - 1);
    return misalignment == 0 ? 0 : alignment - misalignment;
}

NAMESPACE_END
