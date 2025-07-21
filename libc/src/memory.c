#include "kcstd/memory.h"

#include "kcstd/syscalls.h"
#include "kcstd/types.h"

void* memory_alloc(size_t size) {
  void* real_ptr =
      __ASM_MMAP__(null, size + sizeof(memory_block_header),
                   PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  memory_block_header* header = (memory_block_header*)real_ptr;
  header->size = size;
  return (void*)(header + 1);
}

void* memory_realloc(void* ptr, size_t new_size) {
  if (!ptr)
    return memory_alloc(new_size);

  if (new_size == 0) {
    memory_free(ptr);
    return null;
  }

  memory_block_header* old_header = ((memory_block_header*)ptr) - 1;
  size_t old_size = old_header->size;

  void* new_ptr = memory_alloc(new_size);
  if (!new_ptr)
    return null;

  size_t copy_size = old_size < new_size ? old_size : new_size;
  memory_copy(new_ptr, ptr, copy_size);
  memory_free(ptr);

  return new_ptr;
}

void memory_free(void* ptr) {
  if (!ptr)
    return;
  memory_block_header* header = ((memory_block_header*)ptr) - 1;
  size_t size = header->size;
  __ASM_MUNMAP__((void*)header, size + sizeof(memory_block_header));
}

void* memory_move(void* dest, const void* src, size_t size) {
  const byte* s = src;
  byte* d = dest;
  if (d < s) {
    for (size_t i = 0; i < size; i++) {
      d[i] = s[i];  // copy byte in s[i] to d[i]
    }
  } else {
    for (size_t i = size; i > 0; i--) {
      d[i - 1] = s[i - 1];  // copy byte in s[i-1] to d[i-1]
    }
  }
  return dest;
}

void* memory_copy(void* dest, const void* src, size_t size) {
  const byte* s = src;
  byte* d = dest;
  while (size--) {
    *d = *s;  // copy origin byte to dest
    d++;      // advance dest pointer
    s++;      // advance origin pointer
  }
  return dest;
}

void* memory_set(void* src, byte new, size_t size) {
  byte* s = src;
  for (int i = 0; i < size; i++) {
    s[i] = new;
  }
  return src;
}