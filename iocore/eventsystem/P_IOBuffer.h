//
// Created by 宋辰伟 on 2017/4/18.
//

#ifndef TEST_LOCK_P_IOBUFFER_H
#define TEST_LOCK_P_IOBUFFER_H

static const int DEFAULT_BUFFER_SIZE = 128;

#include "kmemory.h"

inline int64_t
IOBufferData::block_size()
{
    return index_to_buffer_size(_size_index);
}

///////////////////////////////////
////   IOBufferBlock
///////////////////////////////////

inline void
IOBufferBlock::consume(int64_t len)
{
    _start += len;
    kassert(_start <= _end);
}

inline void
IOBufferBlock::fill(int64_t len)
{
    _end += len;
    ink_assert(_end <= _buf_end);
}

inline void
IOBufferBlock::reset()
{
    _end = _start = buf();
    _buf_end      = buf() + data->block_size();
}

inline IOBufferBlock *
IOBufferBlock::clone()
{
    IOBufferBlock *b = new IOBufferBlock();
    b->data     = data;
    b->_start   = _start;
    b->_end     = _end;
    b->_buf_end = _end;
    return b;
}

inline void
IOBufferBlock::clear()
{
    data = nullptr;

    IOBufferBlock *p = next.get();
    while(p) {
        IOBufferBlock *n = n->next.get();
        n->data = nullptr;
        p->free();
        p = n;
    }

    next.get().data = nullptr;

    _buf_end = _end = _start = nullptr;
}

inline void
IOBufferBlock::alloc(int64_t i)
{
    std::shared_ptr<IOBufferData> tmp(new IOBufferData(i));
    data = tmp;
    reset();
}

inline void
IOBufferBlock::free()
{
    dealloc();
    delete this;
}

inline void
IOBufferBlock::dealloc()
{
    clear();
}

inline void
IOBufferBlock::set(IOBufferData *d, int64_t len, int64_t offset)
{
    data = d;
    _start   = buf() + offset;
    _end     = _start + len;
    _buf_end = buf() + d->block_size();
}

inline void
IOBufferBlock::set_internal(void *b, int64_t len, int64_t asize_index)
{
    std::shared_ptr<IOBufferData> tmp(new IOBufferData(BUFFER_SIZE_NOT_ALLOCATED));
    data = tmp;

    data->_data = (char *)b;
    data->_size_index = asize_index;
    reset();
    _end = _start + len;
}

inline void
IOBufferBlock::realloc_set_internal(void *b, int64_t buf_size, int64_t asize_index)
{
    int64_t data_size = size();
    memcpy(b, _start, size());
    dealloc();
    set_internal(b, buf_size, asize_index);
    _end = _start + data_size;
}

inline void
IOBufferBlock::realloc(void *b, int64_t buf_size)
{
    realloc_set_internal(b, buf_size, BUFFER_SIZE_NOT_ALLOCATED);
}

inline void
IOBufferBlock::realloc(int64_t i)
{
    if (i == data->_size_index) {
        return;
    }

    ink_release_assert(i > data->_size_index && i != BUFFER_SIZE_NOT_ALLOCATED);
    void *b = kmelloc(index_to_buffer_size(i));
    realloc_set_internal(b, BUFFER_SIZE_FOR_INDEX(i), i);
}


///////////////////////////////////
////   IOBufferData
///////////////////////////////////


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
    if (size_index == BUFFER_SIZE_NOT_ALLOCATED)
        return;
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


#endif // TEST_LOCK_P_IOBUFFER_H
