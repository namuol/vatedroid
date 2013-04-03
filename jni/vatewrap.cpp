#include <string>
#include <sstream>
#include <vector>

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

  Handle<Value> runJS(std::string js) {
    Locker l(isolate);
    Isolate::Scope isolateScope(isolate);

    HandleScope handle_scope;

    Context::Scope context_scope(context);

    // Create a string containing the JavaScript source code.
    Handle<String> source = String::New(js.c_str());

    // Compile the source code.
    Handle<Script> script = Script::Compile(source);
    
    // Run the script to get the result.
    Handle<Value> result = script->Run();
    
    return result;
  }
};

static void runJS_void (
  JNIEnv *env,
  jclass klass,
  jlong handle,
  jstring jstr
) {
  const char* cstr = env->GetStringUTFChars(jstr, NULL);
  std::string js(cstr);
  V8Runner* r = (V8Runner*) handle;
  r->runJS(js);
  env->ReleaseStringUTFChars(jstr, cstr);
}

static jdouble runJS_number (
  JNIEnv *env,
  jclass klass,
  jlong handle,
  jstring jstr
) {
  const char* cstr = env->GetStringUTFChars(jstr, NULL);
  std::string js(cstr);
  V8Runner* r = (V8Runner*) handle;
  jdouble result = r->runJS(js)->NumberValue();
  env->ReleaseStringUTFChars(jstr, cstr);
  return result;
}

static jlong createRunner (
  JNIEnv *env,
  jclass klass
) {
  return (jlong) new V8Runner();
}

static void destroyRunner (
  JNIEnv *env,
  jclass klass,
  jlong handle
) {
  V8Runner* r = (V8Runner*) handle;
  delete r;
}

static JNINativeMethod method_table[] = {
  {"createRunner", "()J", (void *) createRunner},
  {"destroyRunner", "(J)V", (void *) destroyRunner},
  {"runJS_void", "(JLjava/lang/String;)V", (void *) runJS_void},
  {"runJS_number", "(JLjava/lang/String;)D", (void *) runJS_number}
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

  jclass klass = env->FindClass("com/vatedroid/V8Runner");
  
  if (!klass) {
    return -1;
  }

  env->RegisterNatives(
    klass,
    method_table,
    sizeof(method_table) / sizeof(method_table[0])
  );

  env->DeleteLocalRef(klass);
  return JNI_VERSION_1_6;
}
