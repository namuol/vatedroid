package com.vatedroid;

public class V8Runner {
  static {
    System.loadLibrary("gnustl_shared");
    System.loadLibrary("vatewrap");
  }
  
  private native static long createRunner();
  public native void destroyRunner();
  public native double runJS_number(String src);
  public native void runJS_void(String src);
  
  private long handle;

  public V8Runner() {
    handle = createRunner();
  }
  
  public void dispose() {  
    destroyRunner();
  }
}
