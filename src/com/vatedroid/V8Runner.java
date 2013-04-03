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
  
  private long handle = -1;

  public V8Runner() {
    handle = createRunner();
  }
  
  public double runJS_number(String js) {
    return runJS_number(handle, js);
  }
  
  public void runJS_void(String js) {
    runJS_void(handle, js);
  }
  
  public void dispose() {
    if (handle >= 0) {
      destroyRunner(handle);
      handle = -1;
    }
  }
}
