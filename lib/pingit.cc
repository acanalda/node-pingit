// addon.cc 
#include <node.h>
#include <nan.h>
#include "sync.h"
#include "async.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
 
void InitAll(Handle<Object> exports){
  // Sync method
  exports->Set(NanNew<String>("ping"),
    NanNew<FunctionTemplate>(PingSync)->GetFunction());

  // Async method
  exports->Set(NanNew<String>("pingAsync"),
    NanNew<FunctionTemplate>(PingAsync)->GetFunction());
}
 
NODE_MODULE(addon, InitAll)