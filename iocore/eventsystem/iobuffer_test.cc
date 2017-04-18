#include "P_EventSystem.h"

int
main()
{
  IOBufferData *data = new IOBufferData();
  data->alloc(MAX_BUFFER_SIZE_INDEX);
  data->block_size();
  data->dealloc();

  MIOBuffer *mbuf = new MIOBuffer();
}
