package com.vatedroid;

public class V8Runner {
  static {
    System.loadLibrary("gnustl_shared");
    System.loadLibrary("vatewrap");
  }

  public static class V8ValueFactory {
    public V8ValueFactory(V8Runner context) {
    }
  }
  
  private V8ValueFactory valFac;
  
  private native long createRunner();
  public native void dispose();
  
  public native V8Value runJS(String src);
  public native void map(V8MappableMethod m, String name);
  
  private long handle;

  public V8Runner() {
    handle = createRunner();
  }
}
