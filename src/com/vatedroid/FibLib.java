package com.vatedroid;

public class FibLib {
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
  
  public native static long fibNR(long n);
  
  public native static long fibNI(long n);

  public native static long fibJSR(long n);
  
  public native static long fibJSI(long n);
  
  static {
    System.loadLibrary("vatewrap");
  }
}
