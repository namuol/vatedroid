#include <jni.h>

#include <v8.h>
using namespace v8;

#include "V8Runner.h"
#include "V8Value.h"

static jfieldID valueHandle;
static jmethodID value_init,
                 value_init_str,
                 value_init_num,
                 value_init_internal;

static jfieldID runnerHandle;

//////////////////////////////////////////////////////////////////////////////////////////////
// V8RUNNER CLASS
//////////////////////////////////////////////////////////////////////////////////////////////
static jobject runJS (
  JNIEnv *env,
  jobject jrunner,
  jstring jstr
) {
  const char* js = env->GetStringUTFChars(jstr, NULL);
  V8Runner* runner = (V8Runner*) env->GetLongField(jrunner, runnerHandle);
  V8Value* result = new V8Value(runner, runner->runJS(js));
  jclass V8Value_class = env->FindClass("com/vatedroid/V8Value");
  jobject wrappedResult = env->NewObject(V8Value_class, value_init_internal);
  env->SetLongField(wrappedResult, valueHandle, (jlong) result);

  env->ReleaseStringUTFChars(jstr, js);
  return wrappedResult;
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
  {"runJS", "(Ljava/lang/String;)Lcom/vatedroid/V8Value;", (void *) runJS}
};

//////////////////////////////////////////////////////////////////////////////////////////////
// V8VALUE CLASS
//////////////////////////////////////////////////////////////////////////////////////////////
static void V8Value_init_void (
  JNIEnv *env,
  jobject obj,
  jobject jrunner
) {
  V8Runner* runner = (V8Runner*) env->GetLongField(jrunner, runnerHandle);
  env->SetLongField(obj, valueHandle, (jlong) new V8Value(runner));
}

static void V8Value_init_str (
  JNIEnv *env,
  jobject obj,
  jobject jrunner,
  jstring jstr
) {
  V8Runner* runner = (V8Runner*) env->GetLongField(jrunner, runnerHandle);
  const char* cstr = env->GetStringUTFChars(jstr, NULL);
  env->SetLongField(obj, valueHandle, (jlong) new V8Value(runner, cstr)); 
  env->ReleaseStringUTFChars(jstr, cstr);
}

static void V8Value_init_num (
  JNIEnv *env,
  jobject obj,
  jobject jrunner,
  jdouble num
) {
  V8Runner* runner = (V8Runner*) env->GetLongField(jrunner, runnerHandle);
  env->SetLongField(obj, valueHandle, (jlong) new V8Value(runner, num)); 
}

static jstring V8Value_asString (
  JNIEnv *env,
  jobject obj
) {
  V8Value* val = (V8Value*) env->GetLongField(obj, valueHandle);
  return env->NewStringUTF(*String::Utf8Value(val->getValue()->ToString()));
}

static jdouble V8Value_asNumber (
  JNIEnv *env,
  jobject obj
) {
  V8Value* val = (V8Value*) env->GetLongField(obj, valueHandle);
  return val->getValue()->NumberValue();
}

static JNINativeMethod V8Value_Methods[] = {
  {"init", "(Lcom/vatedroid/V8Runner;)V", (void *) V8Value_init_void},
  {"init", "(Lcom/vatedroid/V8Runner;Ljava/lang/String;)V", (void *) V8Value_init_str},
  {"init", "(Lcom/vatedroid/V8Runner;D)V", (void *) V8Value_init_num},
  {"asString", "()Ljava/lang/String;", (void *) V8Value_asString},
  {"asNumber", "()D", (void *) V8Value_asNumber}
};

//////////////////////////////////////////////////////////////////////////////////////////////
// GLUE CODE
//////////////////////////////////////////////////////////////////////////////////////////////
// Lookup class, and Handle:
#define CLASS(env, klass, className, method_table, field) {\
  (klass) = (env)->FindClass(className);\
  if (!(klass)) { return -1; }\
  (env)->RegisterNatives(\
    (klass),\
    (method_table),\
    sizeof((method_table)) / sizeof((method_table)[0])\
  );\
  field = (env)->GetFieldID((klass), "handle", "J");\
  if (field == NULL) return -1;\
}

// Lookup everything else:
#define METHOD(env, klass, method, signature, methodID) {\
  (methodID) = (env)->GetMethodID((klass), "<init>", (signature));\
}

#define CLASS_END(env, klass) {\
  (env)->DeleteLocalRef((klass));\
}

// We need to tell the JNI environment to find our method names and properly
//  map them to our C++ methods.
extern "C" {
jint JNI_OnLoad (
  JavaVM* vm,
  void* reserved
) {
  int i=0;
  while(!i);

  JNIEnv* env;

  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }

  jclass V8Runner_class;
  CLASS(env, V8Runner_class, "com/vatedroid/V8Runner", V8Runner_Methods, runnerHandle)
  CLASS_END(env, V8Runner_class)

  jclass V8Value_class;
  CLASS(env, V8Value_class, "com/vatedroid/V8Value", V8Value_Methods, valueHandle)
  METHOD(env, V8Value_class, "<init>", "(Lcom/vatedroid/V8Runner;)V", value_init)
  METHOD(env, V8Value_class, "<init>", "(Lcom/vatedroid/V8Runner;Ljava/lang/String;)V", value_init_str)
  METHOD(env, V8Value_class, "<init>", "(Lcom/vatedroid/V8Runner;D)V", value_init_num)
  METHOD(env, V8Value_class, "<init>", "()V", value_init_internal)
  CLASS_END(env, V8Value_class)

  return JNI_VERSION_1_6;
}

} // extern "C"