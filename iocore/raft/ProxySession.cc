

#include "ProxyRaft.h"

int
ProxyClientSession::accept(int event, void *edata)
{
  VIO *vio;
  NetVConnection *netvc;

  vio   = static_cast<VIO *>(edata);
  netvc = static_cast<NetVConnection *>(vio->vc_server);

  switch (event) {
  case VC_EVENT_EOS:
  case VC_EVENT_ERROR:
  case VC_EVENT_ACTIVE_TIMEOUT:
  case VC_EVENT_INACTIVITY_TIMEOUT:
    // Error ....
    goto done;
  case VC_EVENT_READ_READY:
  case VC_EVENT_READ_COMPLETE:
    break;
  default:
    ink_abort("unkonwn event abort!");
    return EVENT_ERROR;
  }

done:
  netvc->do_io_close();
  free_MIOBuffer(this->iobuf);
  this->iobuf = nullptr;
  delete this;
  return EVENT_CONT;
}
