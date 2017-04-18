//
// Created by 宋辰伟 on 2017/4/18.
//

#ifndef TEST_LOCK_P_IOBUFFER_H
#define TEST_LOCK_P_IOBUFFER_H

static const int DEFAULT_BUFFER_SIZE = 128;

#include "kmemory.h"

inline int64_t
index_to_buffer_size(int64_t idx)
{
  return idx * DEFAULT_BUFFER_SIZE;
}

inline int64_t
IOBufferData::block_size()
{
  return index_to_buffer_size(_size_index);
}

inline void
IOBufferData::dealloc()
{
  kfree(_data);
  _data       = 0;
  _size_index = BUFFER_SIZE_NOT_ALLOCATED;
  _mem_type   = NO_ALLOC;
}

inline void
IOBufferData::alloc(int64_t size_index, AllocType type)
{
  if (_data)
    dealloc();
  _size_index = size_index;
  _mem_type   = type;
  _data       = (char *)kmalloc(size_index * DEFAULT_BUFFER_SIZE);
}

inline void
IOBufferData::free()
{
  dealloc();
  delete this;
}

inline IOBufferData::~IOBufferData()
{
  dealloc();
}

#endif // TEST_LOCK_P_IOBUFFER_H
