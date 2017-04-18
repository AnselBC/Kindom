//
// Created by 宋辰伟 on 2017/4/18.
//

#ifndef TEST_LOCK_P_IOBUFFER_H
#define TEST_LOCK_P_IOBUFFER_H

inline int64_t
index_to_buffer_size(int64_t idx)
{
    if (BUFFER_SIZE_INDEX_IS_FAST_ALLOCATED(idx))
        return BUFFER_SIZE_FOR_INDEX(idx);
    else if (BUFFER_SIZE_INDEX_IS_XMALLOCED(idx))
        return BUFFER_SIZE_FOR_XMALLOC(idx);
// coverity[dead_error_condition]
    else if (BUFFER_SIZE_INDEX_IS_CONSTANT(idx))
        return BUFFER_SIZE_FOR_CONSTANT(idx);
// coverity[dead_error_line]
    return 0;
}

inline int64_t
IOBufferData::block_size()
{
    return index_to_buffer_size(_size_index);
}

inline void
IOBufferData::dealloc()
{
    kfree(data);
    _data = 0;
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
    _data = (char *)kmalloc(BUFFER_SIZE_FOR_XMALLOC(size_index));
}

#endif //TEST_LOCK_P_IOBUFFER_H
