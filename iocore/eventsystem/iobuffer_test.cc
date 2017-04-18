#include "P_EventSystem.h"

int
main()
{
  IOBufferData *data = new IOBufferData(MAX_BUFFER_SIZE_INDEX);
  data->block_size();
  data->dealloc();

  MIOBuffer *mbuf = new MIOBuffer(MAX_BUFFER_SIZE_INDEX);
  mbuf->add_block();
  IOBufferReader *reader = mbuf->alloc_reader();
}
