#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "Basic.h"

struct PoolFreeNode {
    PoolFreeNode* next;
};

class MemoryPool {
public:
    void Init(usize chunkSize, usize size, u8* data) {
        ASSERT(chunkSize >= sizeof(PoolFreeNode), "Chunk size too small");
        ASSERT((size % chunkSize) == 0, "Size must be a multiple of chunkSize");
        
        m_chunkSize = chunkSize;
        m_size      = size;
        m_data      = data;

        // init free list
        FreeAll();
    }

    void* Alloc() {
        ASSERT(m_head, "Out of memory");
        
        PoolFreeNode* node = m_head;
        m_head = m_head->next;

        return memset(node, 0, m_chunkSize);
    }

    template<typename T>
    T* Alloc() {
        ASSERT(sizeof(T) <= m_chunkSize, "Type T does not fit in chunk");
        return (T*)Alloc();
    }

    void Free(void* ptr) {
        PoolFreeNode* node;

        void* start = m_data;
        void* end   = &m_data[m_size];

        ASSERT(start <= ptr && ptr < end, "Cannot free ptr, not in pool");

        if (!ptr) {
            return;
        }

        node       = reinterpret_cast<PoolFreeNode*>(ptr);
        node->next = m_head;
        m_head     = node;
    }

    void FreeAll() {
        usize chunkCount = m_size / m_chunkSize;
        
        for (usize i = 0; i < chunkCount; i++) {
            usize chunkIndex = i * m_chunkSize;
            PoolFreeNode* node = reinterpret_cast<PoolFreeNode*>(&m_data[chunkIndex]);

            node->next = m_head;
            m_head     = node;
        }
    }

private:
    PoolFreeNode* m_head = NULL;
    
    usize m_chunkSize = 0;
    usize m_size      = 0;
    u8* m_data        = NULL;
};

#endif