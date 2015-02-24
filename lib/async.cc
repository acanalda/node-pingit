// async.cc
#include <node.h>
#include <nan.h>
#include "async.h"
#include "pinger.h"

using v8::Function;
using v8::Local;
using v8::Null;
using v8::Number;
using v8::Value;

class PiWorker : public NanAsyncWorker {
 private:
   std::string _domain;
   int _ping_time;
   Pinger* _pinger;

 public:
   PiWorker(NanCallback *callback, std::string domain)
    : NanAsyncWorker(callback) {
    _domain = domain;
    _pinger = new Pinger();
   }

   ~PiWorker() {
     delete(_pinger);
   }

   // Executed inside the worker-thread.
   // It is not safe to access V8, or V8 data structures
   // here, so everything we need for input and output
   // should go on `this`.
   void Execute () {
     _ping_time = _pinger->ping(_domain);
   }

   // Executed when the async work is complete
   // this function will be run inside the main event loop
   // so it is safe to use V8 again
   void HandleOKCallback () {
     NanScope();

     Local<Value> argv[] = {
        NanNull()
       ,NanNew<Number>(_ping_time)
     };

     callback->Call(2, argv);
   };
};

// Asynchronous access to the `Estimate()` function
NAN_METHOD(PingAsync) {
  NanScope();

  // Check if we have 2 arguments
  if(args.Length() < 2){
    NanThrowError("Url expected as first argument");
    return;
  }

  // Check if first argument is a string
  if(!args[0]->IsString()){
    NanThrowError("Url string expected as first argument");
    return;
  }

  // Check if second argument exists and it's a function
  if(!args[1]->IsFunction()){
    NanThrowError("Second argument must be a callback");
    return;
  }

  std::string domain = std::string(*(v8::String::Utf8Value(args[0]->ToString())));
  NanCallback *callback = new NanCallback(args[1].As<Function>());

  NanAsyncQueueWorker(new PiWorker(callback, domain));
  NanReturnUndefined();
}