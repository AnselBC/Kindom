/** @file

  A brief file description

  @section license License

  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

/**************************************************************************
  UIOBuffer.cc

**************************************************************************/
#include "P_EventSystem.h"

int64_t max_iobuffer_size = DEFAULT_BUFFER_SIZES - 1;

int64_t
MIOBuffer::remove_append(IOBufferReader *r)
{
  int64_t l = 0;
  while (r->block) {
    std::shared_ptr<IOBufferBlock> b = r->block;
    r->block                         = r->block->next;
    b->_start += r->start_offset;
    if (b->start() >= b->end()) {
      r->start_offset = -r->start_offset;
      continue;
    }
    r->start_offset = 0;
    l += b->read_avail();
    append_block(b.get());
  }
  r->mbuf->_writer = nullptr;
  return l;
}

int64_t
MIOBuffer::write(const void *abuf, int64_t alen)
{
  const char *buf = (const char *)abuf;
  int64_t len     = alen;
  while (len) {
    if (!_writer)
      add_block();
    int64_t f = _writer->write_avail();
    f         = f < len ? f : len;
    if (f > 0) {
      ::memcpy(_writer->end(), buf, f);
      _writer->fill(f);
      buf += f;
      len -= f;
    }
    if (len) {
      if (!_writer->next)
        add_block();
      else
        _writer = _writer->next;
    }
  }
  return alen;
}

int64_t
MIOBuffer::write(IOBufferReader *r, int64_t alen, int64_t offset)
{
  int64_t len      = alen;
  IOBufferBlock *b = r->block.get();
  offset += r->start_offset;

  while (b && len > 0) {
    int64_t max_bytes = b->read_avail();
    max_bytes -= offset;
    if (max_bytes <= 0) {
      offset = -max_bytes;
      b      = b->next.get();
      continue;
    }
    int64_t bytes;
    if (len < 0 || len >= max_bytes) {
      bytes = max_bytes;
    } else {
      bytes = len;
    }
    IOBufferBlock *bb = b->clone();
    bb->_start += offset;
    bb->_buf_end = bb->_end = bb->_start + bytes;
    append_block(bb);
    offset = 0;
    len -= bytes;
    b = b->next.get();
  }

  return alen - len;
}

int64_t
MIOBuffer::puts(char *s, int64_t len)
{
  char *pc = end();
  char *pb = s;
  while (pc < buf_end()) {
    if (len-- <= 0)
      return -1;
    if (!*pb || *pb == '\n') {
      int64_t n = (int64_t)(pb - s);
      memcpy(end(), s, n + 1); // Upto and including '\n'
      end()[n + 1] = 0;
      fill(n + 1);
      return n + 1;
    }
    pc++;
    pb++;
  }
  return 0;
}

int64_t
IOBufferReader::read(void *ab, int64_t len)
{
  char *b       = (char *)ab;
  int64_t n     = len;
  int64_t l     = block_read_avail();
  int64_t bytes = 0;

  while (n && l) {
    if (n < l)
      l = n;
    ::memcpy(b, start(), l);
    consume(l);
    b += l;
    n -= l;
    bytes += l;
    l = block_read_avail();
  }
  return bytes;
}

// TODO: I don't think this method is used anywhere, so perhaps get rid of it ?
int64_t
IOBufferReader::memchr(char c, int64_t len, int64_t offset)
{
  IOBufferBlock *b = block.get();
  offset += start_offset;
  int64_t o = offset;

  while (b && len) {
    int64_t max_bytes = b->read_avail();
    max_bytes -= offset;
    if (max_bytes <= 0) {
      offset = -max_bytes;
      b      = b->next.get();
      continue;
    }
    int64_t bytes;
    if (len < 0 || len >= max_bytes)
      bytes = max_bytes;
    else
      bytes = len;
    char *s = b->start() + offset;
    char *p = (char *)::memchr(s, c, bytes);
    if (p)
      return (int64_t)(o - start_offset + p - s);
    o += bytes;
    len -= bytes;
    b      = b->next.get();
    offset = 0;
  }

  return -1;
}

char *
IOBufferReader::memcpy(const void *ap, int64_t len, int64_t offset)
{
  char *p          = (char *)ap;
  IOBufferBlock *b = block.get();
  offset += start_offset;

  while (b && len) {
    int64_t max_bytes = b->read_avail();
    max_bytes -= offset;
    if (max_bytes <= 0) {
      offset = -max_bytes;
      b      = b->next.get();
      continue;
    }
    int64_t bytes;
    if (len < 0 || len >= max_bytes)
      bytes = max_bytes;
    else
      bytes = len;
    ::memcpy(p, b->start() + offset, bytes);
    p += bytes;
    len -= bytes;
    b      = b->next.get();
    offset = 0;
  }

  return p;
}
