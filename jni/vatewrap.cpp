#include <jni.h>
#include <string.h>
#include <android/log.h>

#include <v8.h>

namespace vatewrap {

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

	static JNINativeMethod method_table[] = {
		{"fibNR", "(J)J", (void *) fibNR},
		{"fibNI", "(J)J", (void *) fibNI}
	};

}

using namespace vatewrap;

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

	env->RegisterNatives(
		klass,
		method_table,
		sizeof(method_table) / sizeof(method_table[0])
	);

	env->DeleteLocalRef(klass);
	return JNI_VERSION_1_6;
}