//
// Created by 宋辰伟 on 2017/4/18.
//

#ifndef TEST_LOCK_I_IOBUFFER_H
#define TEST_LOCK_I_IOBUFFER_H



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


    IOBufferData()
            : _size_index(BUFFER_SIZE_NOT_ALLOCATED),
              _mem_type(NO_ALLOC),
              _data(nullptr)
    {
    }
private:
    // declaration only
    IOBufferData(const IOBufferData &);
    IOBufferData &operator=(const IOBufferData &);
};

#endif //TEST_LOCK_I_IOBUFFER_H
