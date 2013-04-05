#include <string>

#include <jni.h>

#include <v8.h>
using namespace v8;

//////////////////////////////////////////////////////////////////////////////////////////////
// V8RUNNER CLASS
//////////////////////////////////////////////////////////////////////////////////////////////
class V8Value;

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

  friend class V8Value;
};

static jfieldID runnerHandle;

static void runJS_void (
  JNIEnv *env,
  jobject obj,
  jstring jstr
) {
  const char* cstr = env->GetStringUTFChars(jstr, NULL);
  std::string js(cstr);
  V8Runner* r = (V8Runner*) env->GetLongField(obj, runnerHandle);
  r->runJS(js);
  env->ReleaseStringUTFChars(jstr, cstr);
}

static jdouble runJS_number (
  JNIEnv *env,
  jobject obj,
  jstring jstr
) {
  const char* cstr = env->GetStringUTFChars(jstr, NULL);
  std::string js(cstr);
  V8Runner* r = (V8Runner*) env->GetLongField(obj, runnerHandle);
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
  jclass obj
) {
  V8Runner* r = (V8Runner*) env->GetLongField(obj, runnerHandle);
  delete r;
}

static JNINativeMethod V8Runner_Methods[] = {
  {"createRunner", "()J", (void *) createRunner},
  {"dispose", "()V", (void *) destroyRunner},
  {"runJS_void", "(Ljava/lang/String;)V", (void *) runJS_void},
  {"runJS_number", "(Ljava/lang/String;)D", (void *) runJS_number}
};

//////////////////////////////////////////////////////////////////////////////////////////////
// V8VALUE CLASS
//////////////////////////////////////////////////////////////////////////////////////////////
static jstring V8Value_asString (JNIEnv *env, jobject obj);

class V8Value {
  Handle<Value> val;

  Isolate* isolate;

  Handle<Context> context;
public:
  V8Value () {
    val = Persistent<Value>(Undefined());
  }

  V8Value (const char* str) {
    isolate = Isolate::New();
    Locker l(isolate);
    Isolate::Scope isolateScope(isolate);

    HandleScope handle_scope;

    context = Persistent<Context>(Context::New());
    val = Persistent<Value>::New(isolate, String::New(str));
  }

  Handle<Value> getValue() {
    Locker l(isolate);
    Isolate::Scope isolateScope(isolate);

    HandleScope handle_scope;
    return val;
  }
};

static jfieldID valueHandle;

static void V8Value_init_void (
  JNIEnv *env,
  jobject obj
) {
  env->SetLongField(obj, valueHandle, (jlong) new V8Value());
}

static void V8Value_init_str (
  JNIEnv *env,
  jobject obj,
  jstring jstr
) {
  const char* cstr = env->GetStringUTFChars(jstr, NULL);
  env->SetLongField(obj, valueHandle, (jlong) new V8Value(cstr)); 
  env->ReleaseStringUTFChars(jstr, cstr);
}

static void V8Value_init_num (
  JNIEnv *env,
  jobject obj,
  jdouble num
) {
  // TODO
}

static jstring V8Value_asString (
  JNIEnv *env,
  jobject obj
) {
  V8Value* val = (V8Value*) env->GetLongField(obj, valueHandle);
  return env->NewStringUTF(*String::Utf8Value(val->getValue()->ToString()));
}

static JNINativeMethod V8Value_Methods[] = {
  {"init", "()V", (void *) V8Value_init_void},
  {"init", "(Ljava/lang/String;)V", (void *) V8Value_init_str},
  {"init", "(D)V", (void *) V8Value_init_num},
  {"asString", "()Ljava/lang/String;", (void *) V8Value_asString},
};

//////////////////////////////////////////////////////////////////////////////////////////////
// GLUE CODE
//////////////////////////////////////////////////////////////////////////////////////////////
extern "C" {

#define SETUP_METHODS(env, className, method_table, field) {\
  jclass klass = (env)->FindClass(className);\
  if (!klass) { return -1; }\
  (env)->RegisterNatives(\
    klass,\
    (method_table),\
    sizeof((method_table)) / sizeof((method_table)[0])\
  );\
  field = (env)->GetFieldID(klass, "handle", "J");\
  if (field == NULL) return -1;\
  (env)->DeleteLocalRef(klass);\
}

// We need to tell the JNI environment to find our method names and properly
//  map them to our C++ methods.
jint JNI_OnLoad (
  JavaVM* vm,
  void* reserved
) {
  JNIEnv* env;

  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }

  SETUP_METHODS(env, "com/vatedroid/V8Runner", V8Runner_Methods, runnerHandle);
  SETUP_METHODS(env, "com/vatedroid/V8Value", V8Value_Methods, valueHandle);

  return JNI_VERSION_1_6;
}

} // extern "C"