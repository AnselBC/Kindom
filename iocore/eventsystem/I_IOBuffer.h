//
// Created by 宋辰伟 on 2017/4/18.
//

#ifndef TEST_LOCK_I_IOBUFFER_H
#define TEST_LOCK_I_IOBUFFER_H
#include <memory>

using namespace std;

struct MIOBufferAccessor;

class MIOBuffer;
class IOBufferReader;

// extern int64_t max_iobuffer_size;
// extern int64_t default_small_iobuffer_size;
// extern int64_t default_large_iobuffer_size; // matched to size of OS buffers

enum AllocType {
  NO_ALLOC,
  FAST_ALLOCATED,
  XMALLOCED,
  MEMALIGNED,
  DEFAULT_ALLOC,
  CONSTANT,
};

#define DEFAULT_BUFFER_NUMBER 128
#define DEFAULT_HUGE_BUFFER_NUMBER 32
#define MAX_MIOBUFFER_READERS 5
#define DEFAULT_BUFFER_ALIGNMENT 8192 // should be disk/page size
#define DEFAULT_BUFFER_BASE_SIZE 128

#define BUFFER_SIZE_INDEX_128 0
#define BUFFER_SIZE_INDEX_256 1
#define BUFFER_SIZE_INDEX_512 2
#define BUFFER_SIZE_INDEX_1K 3
#define BUFFER_SIZE_INDEX_2K 4
#define BUFFER_SIZE_INDEX_4K 5
#define BUFFER_SIZE_INDEX_8K 6
#define BUFFER_SIZE_INDEX_16K 7
#define BUFFER_SIZE_INDEX_32K 8
#define BUFFER_SIZE_INDEX_64K 9
#define BUFFER_SIZE_INDEX_128K 10
#define BUFFER_SIZE_INDEX_256K 11
#define BUFFER_SIZE_INDEX_512K 12
#define BUFFER_SIZE_INDEX_1M 13
#define BUFFER_SIZE_INDEX_2M 14
#define MAX_BUFFER_SIZE_INDEX 14
#define DEFAULT_BUFFER_SIZES (MAX_BUFFER_SIZE_INDEX + 1)

#define BUFFER_SIZE_FOR_XMALLOC(_size) (-(_size))

#define BUFFER_SIZE_NOT_ALLOCATED DEFAULT_BUFFER_SIZES

class IOBufferData
{
public:
  /**
    The size of the memory allocated by this IOBufferData. Calculates
    the amount of memory allocated by this IOBufferData.

    @return number of bytes allocated for the '_data' member.

  */
  int64_t block_size();
  /**
Frees the memory managed by this IOBufferData.  Deallocates the
memory previously allocated by this IOBufferData object. It frees
the memory pointed to by '_data' according to the '_mem_type' and
'_size_index' members.

*/
  void dealloc();

  /**
Allocates memory and sets this IOBufferData to point to it.
Allocates memory according to the size_index and type
parameters. Any previously allocated memory pointed to by
this IOBufferData is deallocated.

@param size_index
@param type of allocation to use; see remarks section.
*/
  void alloc(int64_t size_index, AllocType type = DEFAULT_ALLOC);

  /**
Provides access to the allocated memory. Returns the address of the
allocated memory handled by this IOBufferData.

@return address of the memory handled by this IOBufferData.

*/
  char *
  data()
  {
    return _data;
  }

  /**
 Cast operator. Provided as a convenience, the cast to a char* applied
 to the IOBufferData returns the address of the memory handled by the
 IOBuffer data. In this manner, objects of this class can be used as
 parameter to functions requiring a char*.

*/

  operator char *() { return _data; }
  /**
    Frees the IOBufferData object and its underlying memory. Deallocates
    the memory managed by this IOBufferData and then frees itself. You
    should not use this object or reference after this call.

  */

  /**
Frees the IOBufferData object and its underlying memory. Deallocates
the memory managed by this IOBufferData and then frees itself. You
should not use this object or reference after this call.

*/
  virtual void free();

  int64_t _size_index;

  /**
Type of allocation used for the managed memory. Stores the type of
allocation used for the memory currently managed by the IOBufferData
object. Do not set or modify this value directly. Instead use the
alloc or dealloc methods.

*/
  AllocType _mem_type;

  /**
Points to the allocated memory. This member stores the address of
the allocated memory. You should not modify its value directly,
instead use the alloc or dealloc methods.

*/
  char *_data;

  IOBufferData() : _size_index(BUFFER_SIZE_NOT_ALLOCATED), _mem_type(NO_ALLOC), _data(nullptr) {}
  IOBufferData(int64_t i) { alloc(i); }
  IOBufferData(const IOBufferData *d) : _size_index(BUFFER_SIZE_NOT_ALLOCATED), _mem_type(NO_ALLOC), _data(nullptr) {}
  ~IOBufferData()
  {
    if (_data)
      dealloc();
  }

private:
  // declaration only
  IOBufferData(const IOBufferData &);
  // IOBufferData &operator=(const IOBufferData &);
};

class IOBufferBlock
{
public:
  char *
  buf()
  {
    return data->_data;
  }

  char *
  start()
  {
    return _start;
  }

  char *
  end()
  {
    return _end;
  }

  char *
  buf_end()
  {
    return _buf_end;
  }

  int64_t
  size()
  {
    return (int64_t)(_end - _start);
  }

  int64_t
  read_avail()
  {
    return (int64_t)(_end - _start);
  }

  int64_t
  write_avail()
  {
    return (int64_t)(_buf_end - _end);
  }

  int64_t
  block_size()
  {
    return data->block_size();
  }

  void consume(int64_t len);

  void fill(int64_t len);

  void reset();

  IOBufferBlock *clone();

  void clear();

  void alloc(int64_t i = DEFAULT_BUFFER_SIZES);

  void dealloc();

  void set(IOBufferData *d, int64_t len = 0, int64_t offset = 0);
  void set_internal(void *b, int64_t len, int64_t asize_index);
  void realloc_set_internal(void *b, int64_t buf_size, int64_t asize_index);
  void realloc(void *b, int64_t buf_size);
  void realloc(int64_t i);
  void realloc_xmalloc(void *b, int64_t buf_size);
  void realloc_xmalloc(int64_t buf_size);

  virtual void free();

  char *_start;
  char *_end;
  char *_buf_end;

  std::shared_ptr<IOBufferData> data;
  std::shared_ptr<IOBufferBlock> next;
  IOBufferBlock();

  ~IOBufferBlock()
  {
    if (data != nullptr)
      dealloc();
    data = nullptr;
    next = nullptr;
  }
  IOBufferBlock(const IOBufferBlock *) : _start(0), _end(0), _buf_end(0) {}
private:
  IOBufferBlock(const IOBufferBlock &);
  IOBufferBlock &operator=(const IOBufferBlock &);
};

class IOBufferReader
{
public:
  char *start();
  char *end();
  int64_t read_avail();
  bool is_read_avail_more_than(int64_t size);
  int block_count();
  int64_t block_read_avail();
  void skip_empty_blocks();
  void clear();
  void reset();
  void consume(int64_t n);
  IOBufferReader *clone();
  void dealloc();
  IOBufferBlock *get_current_block();
  bool current_low_water();
  bool low_water();
  bool high_water();
  int64_t memchr(char c, int64_t len = INT64_MAX, int64_t offset = 0);
  int64_t read(void *buf, int64_t len);
  char *memcpy(const void *buf, int64_t len = INT64_MAX, int64_t offset = 0);

  char &operator[](int64_t i);

  MIOBuffer *
  writer() const
  {
    return mbuf;
  }
  MIOBuffer *
  allocated() const
  {
    return mbuf;
  }

  MIOBufferAccessor *accessor; // pointer back to the accessor

  MIOBuffer *mbuf;
  std::shared_ptr<IOBufferBlock> block;

  int64_t start_offset;
  int64_t size_limit;

  IOBufferReader() : accessor(nullptr), mbuf(nullptr), start_offset(0), size_limit(INT64_MAX) {}
};

class MIOBuffer
{
public:
  void fill(int64_t len);
  void append_block(IOBufferBlock *b);
  void append_block(int64_t asize_index);
  void add_block();
  void append_xmalloced(void *b, int64_t len);
  void append_fast_allocated(void *b, int64_t len, int64_t fast_size_index);
  int64_t write(const void *rbuf, int64_t nbytes);
  int64_t write(IOBufferReader *r, int64_t len = INT64_MAX, int64_t offset = 0);
  int64_t remove_append(IOBufferReader *);

  IOBufferBlock *
  first_write_block()
  {
    if (_writer) {
      if (_writer->next && !_writer->write_avail()) {
        return _writer->next.get();
      }
      kassert(!_writer->next || !_writer->next->read_avail());
      return _writer.get();
    }

    return nullptr;
  }

  char *
  buf()
  {
    IOBufferBlock *b = first_write_block();
    return b ? b->buf() : 0;
  }

  char *
  buf_end()
  {
    return first_write_block()->buf_end();
  }

  char *
  start()
  {
    return first_write_block()->start();
  }

  char *
  end()
  {
    return first_write_block()->end();
  }

  int64_t block_write_avail();
  int64_t current_write_avail();
  int64_t write_avail();
  int64_t block_size();

  int64_t
  total_size()
  {
    return block_size();
  }

  bool
  high_water()
  {
    return max_read_avail() > water_mark;
  }

  bool
  low_water()
  {
    return write_avail() <= water_mark;
  }

  bool
  current_low_water()
  {
    return current_write_avail() <= water_mark;
  }

  void set_size_index(int64_t size);
  IOBufferReader *alloc_accessor(MIOBufferAccessor *anAccessor);
  IOBufferReader *alloc_reader();
  IOBufferReader *clone_reader(IOBufferReader *r);
  void dealloc_reader(IOBufferReader *e);
  void dealloc_all_readers();

  void set(void *b, int64_t len);
  void set_xmalloced(void *b, int64_t len);
  void alloc(int64_t i = DEFAULT_BUFFER_SIZES);
  void alloc_xmalloc(int64_t buf_size);
  void append_block_internal(IOBufferBlock *b);
  int64_t puts(char *buf, int64_t len);

  bool
  empty()
  {
    return !_writer;
  }

  int64_t max_read_avail();

  int max_block_count();
  void check_add_block();

  IOBufferBlock *get_current_block();

  void
  reset()
  {
    if (_writer) {
      _writer->reset();
    }
    for (int j = 0; j < MAX_MIOBUFFER_READERS; j++)
      if (readers[j].allocated()) {
        readers[j].reset();
      }
  }

  void
  init_readers()
  {
    for (int j = 0; j < MAX_MIOBUFFER_READERS; j++)
      if (readers[j].allocated() && !readers[j].block)
        readers[j].block = _writer;
  }

  void
  dealloc()
  {
    _writer = nullptr;
    dealloc_all_readers();
  }

  void
  clear()
  {
    dealloc();
    size_index = BUFFER_SIZE_NOT_ALLOCATED;
    water_mark = 0;
  }

  void
  realloc(int64_t i)
  {
    _writer->realloc(i);
  }

  void
  realloc(void *b, int64_t buf_size)
  {
    _writer->realloc(b, buf_size);
  }

  void
  realloc_xmalloc(void *b, int64_t buf_size)
  {
    _writer->realloc(b, buf_size);
  }

  void
  realloc_xmalloc(int64_t buf_size)
  {
    // _writer->realloc_xmalloc(buf_size);
  }

  int64_t size_index;

  int64_t water_mark;

  std::shared_ptr<IOBufferBlock> _writer;
  IOBufferReader readers[MAX_MIOBUFFER_READERS];

  MIOBuffer(void *b, int64_t bufsize, int64_t aWater_mark);
  MIOBuffer(int64_t default_size_index);
  MIOBuffer();
  ~MIOBuffer();
};

struct MIOBufferAccessor {
  IOBufferReader *
  reader()
  {
    return entry;
  }

  MIOBuffer *
  writer()
  {
    return mbuf;
  }

  int64_t
  block_size() const
  {
    return mbuf->block_size();
  }

  int64_t
  total_size() const
  {
    return block_size();
  }

  void reader_for(IOBufferReader *abuf);
  void reader_for(MIOBuffer *abuf);
  void writer_for(MIOBuffer *abuf);

  void
  clear()
  {
    mbuf  = nullptr;
    entry = nullptr;
  }

  MIOBufferAccessor()
    :
#ifdef DEBUG
      name(nullptr),
#endif
      mbuf(nullptr),
      entry(nullptr)
  {
  }

  ~MIOBufferAccessor();

#ifdef DEBUG
  const char *name;
#endif

private:
  MIOBufferAccessor(const MIOBufferAccessor &);
  MIOBufferAccessor &operator=(const MIOBufferAccessor &);

  MIOBuffer *mbuf;
  IOBufferReader *entry;
};

#endif // TEST_LOCK_I_IOBUFFER_H
