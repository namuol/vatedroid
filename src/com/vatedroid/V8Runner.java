package com.vatedroid;

public class V8Runner {
  static {
    System.loadLibrary("gnustl_shared");
    System.loadLibrary("vatewrap");
  }
  
  private native static long createRunner();
  private native static void destroyRunner(long index);
  private native static double runJS_number(long index, String src);
  private native static void runJS_void(long index, String src);
  
  private long index = -1;

  public V8Runner() {
    index = createRunner();
  }
  
  public double runJS_number(String js) {
    return runJS_number(index, js);
  }
  
  public void runJS_void(String js) {
    runJS_void(index, js);
  }
  
  public void dispose() {
    if (index >= 0) {
      destroyRunner(index);
      index = -1;
    }
  }
}
