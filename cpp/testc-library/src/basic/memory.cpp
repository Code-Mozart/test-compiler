// This implementation provides a arena allocator that satisfies
// the memory interface.

#include "memory.h"
#include "memory_metrics.h"
#include <algorithm>

namespace testc {

    constexpr size_t INITIAL_BLOCK_COUNT = 1;
    constexpr size_t DEFAULT_BLOCK_SIZE = 0xFFFF;
    constexpr size_t UNUSED_MEMORY_THRESHOLD = 0xFF;

    Memory_Metrics mem_stats;

    struct Memory_Block {
        size_t size = 0;
        size_t allocations = 0;
        size_t deallocations = 0;
        Owner<byte> begin = nullptr;
        Ref<byte> head = nullptr;
    };

    struct Memory {
        bool initialized = false;
        List<Owner<Memory_Block>> blocks;
    } mem;

    static void initialize() {
        mem.blocks.clear();
        mem.blocks.resize(INITIAL_BLOCK_COUNT);
        for (int i = 0; i < INITIAL_BLOCK_COUNT; i++) {
            Ref<Memory_Block> block = new Memory_Block{};
            mem.blocks[i] = block;
            block->begin = (byte*)malloc(DEFAULT_BLOCK_SIZE);
            block->size = DEFAULT_BLOCK_SIZE;
            block->head = block->begin;

            if (!block->begin) {
                ALLOC_FAILED(DEFAULT_BLOCK_SIZE);
            }
        }
        mem_stats.system_allocations += INITIAL_BLOCK_COUNT;
        mem_stats.total_system_memory += INITIAL_BLOCK_COUNT * DEFAULT_BLOCK_SIZE;

        mem.initialized = true;
    }

    inline static size_t remaining_size(Ref<Memory_Block> block) {
        return block->size - (block->head - block->begin);
    }

    // sorts the blocks by remaining size in ascending order, putting empty blocks at the end
    static void sort_blocks() {
        std::sort(mem.blocks.begin(), mem.blocks.end(),
            [](Ref<Memory_Block> b1, Ref<Memory_Block> b2) -> bool {
                size_t r1 = remaining_size(b1);
                size_t r2 = remaining_size(b2);
                return r1 > 0 && r1 <= r2;
            }
        );
    }


    const Memory_Metrics& get_memory_metrics() {
        return mem_stats;
    }

	Owner<void> allocate(size_t size) {
        // 1. check if initialized
        if (!mem.initialized) {
            initialize();
        }

        // 2. find a suitable existing memory block
        for (Ref<Memory_Block>& block : mem.blocks) {
            if (remaining_size(block) >= size) {
                mem_stats.total_memory_used += size;
                mem_stats.total_allocations++;
                void* ptr = block->head;
                block->head += size;
                block->allocations++;
                return ptr;
            }
        }

        // 3. create a new memory block
        size_t block_size = std::max(DEFAULT_BLOCK_SIZE, size);
        Owner<byte> begin = (byte*)malloc(block_size);
        if (!begin) {
            ALLOC_FAILED(block_size);
        }
        mem_stats.total_system_memory += block_size;
        mem_stats.total_memory_used += size;
        mem_stats.system_allocations++;
        mem_stats.total_allocations++;
        mem.blocks.push_back(::new Memory_Block{ block_size, 1, 0, begin, begin + size });
        sort_blocks();
        return begin;
    }

    void deallocate(Owner<void> ptr) {
        // 1. find the memory block where the ptr exists in
        for (size_t i = 0; i < mem.blocks.size(); i++) {
            Ref<Memory_Block> block = mem.blocks[i];
            if (ptr >= block->begin && ptr < (block->begin + block->size)) {
                // 2. check if the memory block can be deallocated
                mem_stats.total_deallocations++;
                block->deallocations++;
                ASSERT(block->deallocations <= block->allocations);
                if ((block->deallocations == block->allocations) && (remaining_size(block) <= UNUSED_MEMORY_THRESHOLD)) {
                    free(block->begin);
                    delete block;
                    mem.blocks.erase(mem.blocks.begin() + i);
                }
                return;
            }
        }

        // 3. instance was not in any memory block -> exception
        throw Invalid_Argument_Exception(
            "Tried to destruct an instance that was not constructed by this memory");
    }
}
