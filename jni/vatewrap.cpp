#include <string>
#include <sstream>

#include <jni.h>
#include <android/log.h>

#include <v8.h>
using namespace v8;

static jlong fib (jlong n) {
  return n <= 0 ? 0 : n == 1 ? 1 : fib(n - 1) + fib(n -2);
}

static jlong fibNR (
  JNIEnv *env,
  jclass klass,
  jlong n
) {
  return fib(n);
}

static jlong fibNI (
  JNIEnv *env,
  jclass klass,
  jlong n
) {
  jlong prev = -1;
  jlong result = 1;
  jlong i;
  for(i=0; i <= n; ++i) {
    jlong sum = result + prev;
    prev = result;
    result = sum;
  }

  return result;
}

class V8Runner {
  Isolate* isolate;

  Handle<Context> context;
  
  Handle<Value> runJS(std::string js) {
    Context::Scope context_scope(context);

    // Create a string containing the JavaScript source code.
    Handle<String> source = String::New(js.c_str());

    // Compile the source code.
    Handle<Script> script = Script::Compile(source);
    
    // Run the script to get the result.
    Handle<Value> result = script->Run();
    
    return result;
  }

  public:
  V8Runner() {
    isolate = Isolate::New();
    Locker l(isolate);
    Isolate::Scope isolateScope(isolate);

    HandleScope handle_scope;

    context = Persistent<Context>(Context::New());

    Context::Scope context_scope(context);

    // Define our actual JS functions.
    std::string sourceStr = std::string() +
    "function fib (n) {" +
      "return n <= 0 ? 0 : n == 1 ? 1 : fib(n - 1) + fib(n - 2);" +
    "}" +

    "function fibJSR (n) {" +
      "return fib(n);" +
    "}" +

    "function fibJSI (n) {" +
      "var prev = -1," +
          "result = 1," +
         "i, sum;" +

      "for(i=0; i <= n; ++i) {" +
        "sum = result + prev;" +
        "prev = result;" +
        "result = sum;" +
      "}" +

      "return result;" +
    "}";

    // Create a string containing the JavaScript source code.
    Handle<String> source = String::New(sourceStr.c_str());

    // Compile the source code and run it.
    Script::Compile(source)->Run();
  }

  jlong fibJSR (jlong n) {
    Locker l(isolate);
    Isolate::Scope isolateScope(isolate);
    
    HandleScope handle_scope;

    std::stringstream ss;
    ss << "fibJSR(" << n << ");";
    Handle<Value> result = runJS(ss.str());
    return result->NumberValue();
  }

  jlong fibJSI (jlong n) {
    Locker l(isolate);
    Isolate::Scope isolateScope(isolate);

    HandleScope handle_scope;

    std::stringstream ss;
    ss << "fibJSI(" << n << ");";
    Handle<Value> result = runJS(ss.str());
    return result->NumberValue();
  }
};

static V8Runner* runner;

static jlong fibJSR (
  JNIEnv *env,
  jclass klass,
  jlong n
) {
  return runner->fibJSR(n);
}

static jlong fibJSI (
  JNIEnv *env,
  jclass klass,
  jlong n
) {
  return runner->fibJSI(n);
}

static JNINativeMethod method_table[] = {
  {"fibNR", "(J)J", (void *) fibNR},
  {"fibNI", "(J)J", (void *) fibNI},
  {"fibJSR", "(J)J", (void *) fibJSR},
  {"fibJSI", "(J)J", (void *) fibJSI}
};

// We need to tell the JNI environment to find our method names and properly
//  map them to our C++ methods.
extern "C"
jint JNI_OnLoad (
  JavaVM* vm,
  void* reserved
) {
  JNIEnv* env;

  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }

  jclass klass = env->FindClass("com/vatedroid/FibLib");
  
  if (!klass) {
    return -1;
  }

  runner = new V8Runner();

  env->RegisterNatives(
    klass,
    method_table,
    sizeof(method_table) / sizeof(method_table[0])
  );

  env->DeleteLocalRef(klass);
  return JNI_VERSION_1_6;
}
