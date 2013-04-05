package com.vatedroid;

public class V8Value {
  private long handle;
  
  private native void init();
  private native void init(String str);
  private native void init(double num);
  
  private native boolean isUndefined();
  private native boolean isString();
  private native boolean isNumber();
  
  public native String asString();
  public native double asNumber();
  
  public V8Value() { init(); }
  public V8Value(String str) { init(str); }
  public V8Value(double num) { init(num); }
}
