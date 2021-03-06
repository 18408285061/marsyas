/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.33
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package edu.uvic.marsyas;

public class MarControlPtr {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected MarControlPtr(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(MarControlPtr obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      marsyasJNI.delete_MarControlPtr(swigCPtr);
    }
    swigCPtr = 0;
  }

  public MarControlPtr() {
    this(marsyasJNI.new_MarControlPtr__SWIG_0(), true);
  }

  public MarControlPtr(MarControlPtr a) {
    this(marsyasJNI.new_MarControlPtr__SWIG_1(MarControlPtr.getCPtr(a), a), true);
  }

  public boolean isInvalid() {
    return marsyasJNI.MarControlPtr_isInvalid(swigCPtr, this);
  }

  public static MarControlPtr from_natural(int x) {
    return new MarControlPtr(marsyasJNI.MarControlPtr_from_natural(x), true);
  }

  public static MarControlPtr from_real(double x) {
    return new MarControlPtr(marsyasJNI.MarControlPtr_from_real(x), true);
  }

  public static MarControlPtr from_bool(boolean x) {
    return new MarControlPtr(marsyasJNI.MarControlPtr_from_bool(x), true);
  }

  public static MarControlPtr from_string(String x) {
    return new MarControlPtr(marsyasJNI.MarControlPtr_from_string(x), true);
  }

  public String getType() {
    return marsyasJNI.MarControlPtr_getType(swigCPtr, this);
  }

  public String getName() {
    return marsyasJNI.MarControlPtr_getName(swigCPtr, this);
  }

  public boolean setValue_natural(int x) {
    return marsyasJNI.MarControlPtr_setValue_natural(swigCPtr, this, x);
  }

  public boolean setValue_real(double x) {
    return marsyasJNI.MarControlPtr_setValue_real(swigCPtr, this, x);
  }

  public boolean setValue_bool(boolean x) {
    return marsyasJNI.MarControlPtr_setValue_bool(swigCPtr, this, x);
  }

  public boolean setValue_string(String x) {
    return marsyasJNI.MarControlPtr_setValue_string(swigCPtr, this, x);
  }

  public int to_natural() {
    return marsyasJNI.MarControlPtr_to_natural(swigCPtr, this);
  }

  public double to_real() {
    return marsyasJNI.MarControlPtr_to_real(swigCPtr, this);
  }

  public String to_string() {
    return marsyasJNI.MarControlPtr_to_string(swigCPtr, this);
  }

  public boolean to_bool() {
    return marsyasJNI.MarControlPtr_to_bool(swigCPtr, this);
  }

}
