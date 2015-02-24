// sync.cc
#include <node.h>
#include <nan.h>
#include "sync.h"
#include "pinger.h"

using v8::Number;

NAN_METHOD(PingSync) {
  NanScope();

  // Check if number of arguments
  if(args.Length() < 1){
    NanThrowError("Url expected as first argument");
    return;
  }

  // Check if first argument is a string
  if(!args[0]->IsString()){
    NanThrowError("Url string expected as first argument");
    return;
  }

  // expect a string as the first argument
  std::string domain = std::string(*(v8::String::Utf8Value(args[0]->ToString())));
  Pinger* pinger = new Pinger();
  int ping_time = pinger->ping(domain);

  NanReturnValue(NanNew<Number>(ping_time));

  delete(pinger);
}