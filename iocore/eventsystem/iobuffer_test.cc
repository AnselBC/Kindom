#include "P_EventSystem.h"

int
main()
{
  IOBufferData *data = new IOBufferData(MAX_BUFFER_SIZE_INDEX);
  data->block_size();
  data->dealloc();

  const char *buf         = "scw! I love you!!!";
  const char output[1024] = {0};
  MIOBuffer *mbuf         = new MIOBuffer(MAX_BUFFER_SIZE_INDEX);
  mbuf->add_block();
  IOBufferReader *reader = mbuf->alloc_reader();
  mbuf->write(buf, strlen(buf));
  reader->read((void *)output, 7);
  printf("%s\n", output);
}
