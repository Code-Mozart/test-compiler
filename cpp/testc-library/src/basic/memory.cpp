#include "memory.h"

namespace testc {

    constexpr size_t INITIAL_BLOCK_COUNT = 10;
    constexpr size_t INITIAL_BLOCK_SIZE = 0xFFFF;

	struct Memory_Block {
		Owner<void> start = nullptr;
		size_t size = 0;
		Ref<void> head = nullptr;
	};

	struct Memory_Manager {
		Set<Owner<Memory_Block>> blocks;
		Ref<Memory_Block> current_block;
	} mem;

    static void initialize() {
        mem.blocks.clear();
        mem.blocks.reserve(INITIAL_BLOCK_COUNT);

        Ref<Memory_Block> initial_block = *(mem.blocks.begin());
        initial_block->start = malloc(INITIAL_BLOCK_SIZE);
        initial_block->size = INITIAL_BLOCK_SIZE;
        initial_block->head = initial_block->start;

        mem.current_block = initial_block;
    }

    inline static bool is_initialized() { return mem.current_block; }

    inline static size_t remaining_size(Ref<Memory_Block> block) {
        return block->size - (block->head - block->start);
    }


    template<typename T, typename... Args>
	T* construct(Args&&... args) {
        if (remaining_size(mem.current_block) < sizeof(T)) {
            // construct in new block
        }
        else {
            // construct in current block
        }
    }

}
