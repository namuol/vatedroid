#ifndef V8RUNNER_H_
#define V8RUNNER_H_

#include <v8.h>
using namespace v8;

class V8Value;

class V8Runner {
  Isolate* isolate;
  Handle<Context> context;
  
public:
  V8Runner();

  Handle<Value> runJS(const char* str);
  friend class V8Value;
};

#endif // V8RUNNER_H_