package com.vatedroid;

import android.util.Log;

public class FibLib {
  static {
    //System.loadLibrary("gnustl_shared");
    System.loadLibrary("fiblib");
  }

  public native static long fibNR(long n);
  public native static long fibNI(long n);

  public static long fibJSR(long n) { return (long) v8.runJS("fibJSR(" + n + ");").asNumber(); }
  public static long fibJSI(long n) { return (long) v8.runJS("fibJSI(" + n + ");").asNumber(); }
  
  private static V8Runner v8 = null;
  private static final String INITIAL_SOURCE = 
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
  
  public static long fibJR(long n) {
    return n <= 0 ? 0 : n == 1 ? 1 : fibJR(n - 1) + fibJR(n -2);
  }
  
  public static long fibJI(long n) {
    long prev = -1;
    long result = 1;
    
    for(long i=0; i <= n; ++i) {
      long sum = result + prev;
      prev = result;
      result = sum;
    }
    
    return result;
  }

  
  static class TestMappableMethod implements V8MappableMethod {
    @Override
    public V8Value methodToRun(V8Value[] args) {
      Log.e("methodToRun", "Hello from Java!");
      Log.e("methodToRun", "Arguments:");
      for (V8Value val : args) {
        Log.e("methodToRun", val.toString());
      }
      return v8.val(0);
    }

    @Override
    public V8Runner getRunner() {
      return v8;
    }
  }
  
  public static void init () {
    v8 = new V8Runner();
    v8.map(new FibLib.TestMappableMethod(), "sayHello");
    v8.runJS(INITIAL_SOURCE);
    v8.runJS("sayHello(\"Testing\", 1, 2, 3, [42,43,44]);");
  }
  
  public static void deinit() {
    if (v8 != null) {
      v8.dispose();
    }
  }
}
