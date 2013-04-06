#include <v8.h>
using namespace v8;

#include "V8Runner.h"
#include "V8Value.h"

V8Runner::V8Runner() {
  isolate = Isolate::New();
  Locker l(isolate);
  Isolate::Scope isolateScope(isolate);

  HandleScope handle_scope;

  context = Persistent<Context>(Context::New());
}

Handle<Value> V8Runner::runJS(const char* js) {
  Locker l(isolate);
  Isolate::Scope isolateScope(isolate);

  HandleScope handle_scope;

  Context::Scope context_scope(context);

  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New(js);

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);
  
  // Run the script to get the result.
  Handle<Value> result = Persistent<Value>::New(script->Run());
  
  return result;
}