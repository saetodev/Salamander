#ifndef SAL_MEMORY_ARENA_H
#define SAL_MEMORY_ARENA_H

#include "Basic.h"

class MemoryArena {
public:
    MemoryArena(usize size, u8* data) {
        ASSERT(size > 0, "MemoryArena cannot have a size of 0 bytes");
        ASSERT(data, "MemoryArena cannot have NULL backing buffer");

        m_size = size;
        m_data = data;
    }

    void Clear() {
        m_offset = 0;
    }

    void* Alloc(usize size) {
        ASSERT(size > 0, "MemoryArena cannot alloc 0 bytes");
        ASSERT((m_offset + size) <= m_size, "MemoryArena no memory");

        void* ptr = m_data + m_offset;
        m_offset += size;

        return memset(ptr, 0, size);
    }

    template<typename T>
    T* Alloc() {
        return reinterpret_cast<T*>(Alloc(sizeof(T)));
    }

    template<typename T>
    T* Alloc(usize count) {
        return reinterpret_cast<T*>(Alloc(sizeof(T) * count));
    }

private:
    usize m_size   = 0;
    usize m_offset = 0;
    u8* m_data     = NULL;
};

#endif