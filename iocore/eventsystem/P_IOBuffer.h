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
  kassert(_end <= _buf_end);
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
  b->data          = data;
  b->_start        = _start;
  b->_end          = _end;
  b->_buf_end      = _end;
  return b;
}

inline void
IOBufferBlock::clear()
{
  data = nullptr;

  IOBufferBlock *p = next.get();
  while (p) {
    IOBufferBlock *n = n->next.get();
    n->data          = nullptr;
    p->free();
    p = n;
  }

  next.get()->data = nullptr;

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
  data     = std::make_shared<IOBufferData>(d);
  _start   = buf() + offset;
  _end     = _start + len;
  _buf_end = buf() + d->block_size();
}

inline void
IOBufferBlock::set_internal(void *b, int64_t len, int64_t asize_index)
{
  std::shared_ptr<IOBufferData> tmp(new IOBufferData(BUFFER_SIZE_NOT_ALLOCATED));
  data = tmp;

  data->_data       = (char *)b;
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

  krelease_assert(i > data->_size_index && i != BUFFER_SIZE_NOT_ALLOCATED);
  void *b = kmalloc(index_to_buffer_size(i));
  realloc_set_internal(b, index_to_buffer_size(i), i);
}

///////////////////////////////////
////   IOBufferData
///////////////////////////////////

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

///////////////////////////////////
////   IOBufferReader
///////////////////////////////////
inline IOBufferBlock::IOBufferBlock() : _start(0), _end(0), _buf_end(0)
{
}

inline void
IOBufferReader::skip_empty_blocks()
{
  while (block->next && block->next->read_avail() && start_offset >= block->size()) {
    start_offset -= block->size();
    block = block->next;
  }
}

inline char *
IOBufferReader::start()
{
  if (!block) {
    return 0;
  }

  skip_empty_blocks();
  return block->start() + start_offset;
}

inline char *
IOBufferReader::end()
{
  if (!block) {
    return 0;
  }

  skip_empty_blocks();
  return block->end();
}

inline int64_t
IOBufferReader::read_avail()
{
  int64_t t        = 0;
  IOBufferBlock *b = block.get();

  while (b) {
    t += b->read_avail();
    b = b->next.get();
  }

  t -= start_offset;
  if (size_limit != INT64_MAX && t > size_limit) {
    t = size_limit;
  }

  return t;
}

inline bool
IOBufferReader::is_read_avail_more_than(int64_t size)
{
  int64_t t        = -start_offset;
  IOBufferBlock *b = block.get();

  while (b) {
    t += b->read_avail();
    if (t > size) {
      return true;
    }
    b = b->next.get();
  }
  return false;
}

inline void
IOBufferReader::consume(int64_t n)
{
  start_offset += n;
  if (size_limit != INT64_MAX) {
    size_limit -= n;
  }

  kassert(size_limit >= 0);
  if (!block) {
    return;
  }

  int64_t r = block->read_avail();
  int64_t s = start_offset;
  while (r <= s && block->next && block->next->read_avail()) {
    s -= r;
    start_offset = s;
    block        = block->next;
    r            = block->read_avail();
  }
}

inline char &IOBufferReader::operator[](int64_t i)
{
  static char default_ret = '\0'; // This is just to avoid compiler warnings...
  IOBufferBlock *b        = block.get();

  i += start_offset;
  while (b) {
    int64_t bytes = b->read_avail();
    if (bytes > i)
      return b->start()[i];
    i -= bytes;
    b = b->next.get();
  }

  krelease_assert(!"out of range");
  // Never used, just to satisfy compilers not undersatnding the fatality of krelease_assert().
  return default_ret;
}

inline void
IOBufferReader::clear()
{
  accessor     = nullptr;
  block        = nullptr;
  mbuf         = nullptr;
  start_offset = 0;
  size_limit   = INT64_MAX;
}

inline void
IOBufferReader::reset()
{
  block        = mbuf->_writer;
  start_offset = 0;
  size_limit   = INT64_MAX;
}

///////////////////////////////////
////   MIOBuffer
///////////////////////////////////

inline MIOBuffer::MIOBuffer(void *b, int64_t bufsize, int64_t aWater_mark)
{
  set(b, bufsize);
  water_mark = aWater_mark;
  size_index = BUFFER_SIZE_NOT_ALLOCATED;
  return;
}

inline MIOBuffer::MIOBuffer()
{
  clear();
}

inline MIOBuffer::MIOBuffer(int64_t default_size_index)
{
  size_index = default_size_index;
  clear();
}

inline MIOBuffer::~MIOBuffer()
{
  _writer = nullptr;
  dealloc_all_readers();
}

inline void
MIOBuffer::append_block(int64_t asize_index)
{
	IOBufferBlock *b = new IOBufferBlock();
  b->alloc(asize_index);
  append_block_internal(b);
  return;
}

inline void
MIOBuffer::add_block()
{
  append_block(size_index);
}

inline void
MIOBuffer::append_block_internal(IOBufferBlock *b)
{
  // It would be nice to remove an empty buffer at the beginning,
  // but this breaks HTTP.
  // if (!_writer || !_writer->read_avail())
  if (!_writer) {
    _writer = std::make_shared<IOBufferBlock>(b);
    init_readers();
  } else {
    kassert(!_writer->next || !_writer->next->read_avail());
    _writer->next = std::make_shared<IOBufferBlock>(b);
    while (b->read_avail()) {
      _writer = std::make_shared<IOBufferBlock>(b);
      b       = b->next.get();
      if (!b)
        break;
    }
  }
  while (_writer->next && !_writer->write_avail() && _writer->next->read_avail())
    _writer = _writer->next;
}

inline int64_t
MIOBuffer::write_avail()
{
  check_add_block();
  return current_write_avail();
}

inline void
MIOBuffer::fill(int64_t len)
{
  int64_t f = _writer->write_avail();
  while (f < len) {
    _writer->fill(f);
    len -= f;
    if (len > 0)
      _writer = _writer->next;
    f         = _writer->write_avail();
  }
  _writer->fill(len);
}

inline int
MIOBuffer::max_block_count()
{
  int maxb = 0;
  for (int i = 0; i < MAX_MIOBUFFER_READERS; i++) {
    if (readers[i].allocated()) {
      int c = readers[i].block_count();
      if (c > maxb) {
        maxb = c;
      }
    }
  }
  return maxb;
}

inline int64_t
MIOBuffer::max_read_avail()
{
  int64_t s = 0;
  int found = 0;
  for (int i = 0; i < MAX_MIOBUFFER_READERS; i++) {
    if (readers[i].allocated()) {
      int64_t ss = readers[i].read_avail();
      if (ss > s) {
        s = ss;
      }
      found = 1;
    }
  }
  if (!found && _writer)
    return _writer->read_avail();
  return s;
}

inline void
MIOBuffer::set(void *b, int64_t len)
{
  std::shared_ptr<IOBufferBlock> tmp(new IOBufferBlock());
  _writer = tmp;
  _writer->set_internal(b, len, len + DEFAULT_BUFFER_SIZES);
  init_readers();
}

inline void
MIOBuffer::set_xmalloced(void *b, int64_t len)
{
  std::shared_ptr<IOBufferBlock> tmp(new IOBufferBlock());
  _writer = tmp;
  _writer->set_internal(b, len, len + DEFAULT_BUFFER_SIZES);
  init_readers();
}

inline void
MIOBuffer::append_xmalloced(void *b, int64_t len)
{
  IOBufferBlock *x = new IOBufferBlock();
  x->set_internal(b, len, len + DEFAULT_BUFFER_SIZES);
  append_block_internal(x);
}

inline void
MIOBuffer::append_fast_allocated(void *b, int64_t len, int64_t fast_size_index)
{
  IOBufferBlock *x = new IOBufferBlock();
  x->set_internal(b, len, len + DEFAULT_BUFFER_SIZES);
  append_block_internal(x);
}

inline void
MIOBuffer::alloc(int64_t i)
{
  std::shared_ptr<IOBufferBlock> tmp(new IOBufferBlock());
  _writer = tmp;
  _writer->alloc(i);
  size_index = i;
  init_readers();
}

inline void
MIOBuffer::alloc_xmalloc(int64_t buf_size)
{
  char *b = (char *)kmalloc(buf_size);
  set_xmalloced(b, buf_size);
}

inline void
MIOBuffer::dealloc_reader(IOBufferReader *e)
{
  if (e->accessor) {
    kassert(e->accessor->writer() == this);
    kassert(e->accessor->reader() == e);
    e->accessor->clear();
  }
  e->clear();
}

inline IOBufferReader *
IOBufferReader::clone()
{
  return mbuf->clone_reader(this);
}

inline void
IOBufferReader::dealloc()
{
  mbuf->dealloc_reader(this);
}

inline void
MIOBuffer::dealloc_all_readers()
{
  for (int i = 0; i < MAX_MIOBUFFER_READERS; i++)
    if (readers[i].allocated())
      dealloc_reader(&readers[i]);
}

inline void
MIOBuffer::set_size_index(int64_t size)
{
  // size_index = iobuffer_size_to_index(size);
}

///////////////////////////////////
////   MIOBufferAccessor
///////////////////////////////////

inline void
MIOBufferAccessor::reader_for(MIOBuffer *abuf)
{
  mbuf = abuf;
  if (abuf)
    entry = mbuf->alloc_accessor(this);
  else
    entry = nullptr;
}

inline void
MIOBufferAccessor::reader_for(IOBufferReader *areader)
{
  if (entry == areader)
    return;
  mbuf  = areader->mbuf;
  entry = areader;
  kassert(mbuf);
}

inline void
MIOBufferAccessor::writer_for(MIOBuffer *abuf)
{
  mbuf  = abuf;
  entry = nullptr;
}

inline MIOBufferAccessor::~MIOBufferAccessor()
{
}

#endif // TEST_LOCK_P_IOBUFFER_H
