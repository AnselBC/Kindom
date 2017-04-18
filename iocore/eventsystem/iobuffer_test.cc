#include "P_EventSystem.h"
#include <string.h>

int
main()
{
  IOBufferData *data = new IOBufferData(MAX_BUFFER_SIZE_INDEX);
  data->block_size();
  data->dealloc();

  const char *buf         = "scw! I love you!!!";
  const char output[1024] = {0};
  MIOBuffer *mbuf         = new MIOBuffer(MAX_BUFFER_SIZE_INDEX);
  auto w                  = mbuf->_writer.get();
  mbuf->add_block();
  mbuf->add_block();
  mbuf->add_block();
  IOBufferReader *reader = mbuf->alloc_reader();
  mbuf->write(buf, strlen(buf));
  reader->read((void *)output, 7);
  printf("%s\n", output);
  memset((void *)output, 0, 1024);
  reader->read((void *)output, 7);
  printf("%s\n", output);

  w->clear();
}
