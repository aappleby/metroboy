#include "Types.h"

#include <string>

//-----------------------------------------------------------------------------

/*
template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}
*/

//-----------------------------------------------------------------------------

void dump_req(Dumper& d, const Req& req) {
  if (req.write)  {
    d("%04x:%04x %s%s\n", req.addr, req.data2,
      req.read  ? "\003R \001" : "- ",
      req.write ? "\002W \001" : "- ");
  }
  else {
    d("%04x:---- %s%s\n", req.addr,
      req.read  ? "\003R \001" : "- ",
      req.write ? "\002W \001" : "- ");
  }
}

void dump_ack(Dumper& d, const Ack& ack) {
  d("%04x:%04x %s%s\n", ack.addr, ack.data2,
    ack.read  ? "\003R \001" : "- ",
    "- ");
}

