#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/types.h"

memoryp_manager default_mem_mgr;

void memoryp_init() {
  __memoryp_init__(&default_mem_mgr);
}

void* memoryp_alloc(size_t size) {
  return __memoryp_alloc__(&default_mem_mgr, size);
}

void memoryp_free(void* ptr) {
  __memoryp_free__(&default_mem_mgr, ptr);
}

void __memoryp_init__(memoryp_manager* mgr) {
  mgr->free_blocks = (memoryp_block*)mgr->memoryp;
  mgr->free_blocks->next = null;
  mgr->free_blocks->size = POOL_SIZE - sizeof(memoryp_block);
  mgr->free_blocks->used = false;
}

void* __memoryp_alloc__(memoryp_manager* mgr, size_t size) {
  memoryp_block* block = mgr->free_blocks;
  memoryp_block* previus = null;
  size = (size + sizeof(memoryp_block) - 1) / sizeof(memoryp_block) *
         sizeof(memoryp_block);
  while (block) {
    if (!block->used && block->size >= size) {
      if (block->size > size + sizeof(memoryp_block)) {
        memoryp_block* new_block =
            (memoryp_block*)((byte*)block + sizeof(memoryp_block) + size);
        new_block->size = block->size - size - sizeof(memoryp_block);
        new_block->used = false;
        new_block->next = block->next;

        block->next = new_block;
        block->size = size;
      }
      block->used = true;
      return (byte*)block + sizeof(memoryp_block);
    }
    previus = block;
    block = block->next;
  }
  return null;
}

void __memoryp_free__(memoryp_manager* mgr, void* ptr) {
  if (ptr == null)
    return;
  memoryp_block* block = (memoryp_block*)((byte*)ptr - sizeof(memoryp_block));
  block->used = false;

  memoryp_block* current = mgr->free_blocks;
  while (current) {
    if (((byte*)current + sizeof(memoryp_block) + current->size) ==
        (byte*)block) {
      current->size += sizeof(memoryp_block) + block->size;
      current->next = block->next;
      return;
    }
    if (((byte*)block + sizeof(memoryp_block) + block->size) ==
        (byte*)current) {
      block->size += sizeof(memoryp_block) + current->size;
      block->next = current->next;
      if (mgr->free_blocks == current) {
        mgr->free_blocks = block;
      }
      return;
    }
    current = current->next;
  }
  block->next = mgr->free_blocks;
  mgr->free_blocks = block;
}