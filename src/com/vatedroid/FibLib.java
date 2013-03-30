package com.vatedroid;

public class FibLib {
  static {
    System.loadLibrary("gnustl_shared");
    System.loadLibrary("fiblib");
  }

  public native static long fibNR(long n);
  public native static long fibNI(long n);

  public static long fibJSR(long n) { return (long) v8.runJS_number("fibJSR(" + n + ");"); }
  public static long fibJSI(long n) { return (long) v8.runJS_number("fibJSI(" + n + ");"); }
  
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
  
  public static void init () {
    v8 = new V8Runner();
    v8.runJS_void(INITIAL_SOURCE);
  }
  
  public static void deinit() {
    if (v8 != null) {
      v8.dispose();
    }
  }
}
