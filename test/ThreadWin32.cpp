/****************************************************
 * File: ThreadWin32.cpp
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * An implementation of the Thread class using
 * the Win32 API.
 */
#include "Thread.hpp"
#include <windows.h>

namespace synch {
  /* A utility struct containing the data necessary to launch a thread
   * with user code.  It stores either a nullary function pointer or
   * a unary function and its argument.
   */
  struct FunctionData {
    void (*mNullaryFunction)();
    void (*mUnaryFunction)(void *);
    void* mUserData;
  
    /* Ctor sets up all the fields. */
    FunctionData(void (*nullaryFunction)(), 
                 void (*unaryFunction)(void*), void* userData)
      : mNullaryFunction(nullaryFunction), mUnaryFunction(unaryFunction), mUserData(userData) {
      // Handled in initializer list
    }
  };
  
  /* Thread execution functions which, given access to a FunctionData object,
   * runs either the unary or nullary function it contains.
   */
  static DWORD WINAPI NullaryExecutionFunction(LPVOID data);
  static DWORD WINAPI UnaryExecutionFunction(LPVOID data);
  
  /* The Thread::Impl is a wrapper around a HANDLE object and the specified
   * callback.
   */
  struct Thread::Impl {
    HANDLE mHandle;
    FunctionData mData;
  
    /* Constructors store the function and run the main thread. */
    Impl(void (*function)()) : mData(function, NULL, NULL) {
      mHandle = CreateThread(NULL, 0, NullaryExecutionFunction, &mData, 0, NULL);
    }
    Impl(void (*function)(void*), void* data) : mData(NULL, function, data) {
      mHandle = CreateThread(NULL, 0, UnaryExecutionFunction, &mData, 0, NULL);
    }
  
    /* Destructor closes the thread handle. */
    ~Impl() {
      CloseHandle(mHandle);
    }
  };
  
  /* The functions that actually run the threads. */
  static DWORD WINAPI UnaryExecutionFunction(LPVOID arg) {
    /* Convert the argument from void* to a pointer to a FunctionData object. */
    FunctionData* data = static_cast<FunctionData*>(arg);
    data->mUnaryFunction(data->mUserData);
    return 0;
  }
  static DWORD WINAPI NullaryExecutionFunction(LPVOID arg) {
    /* Convert the argument from void* to a pointer to a FunctionData object. */
    static_cast<FunctionData*>(arg)->mNullaryFunction();
    return 0;
  }
  
  /* Thread ctors just push everything to Impl. */
  Thread::Thread(void function(void)) : mImpl(new Impl(function)) {
    // Handled in initializer list
  }
  Thread::Thread(void function(void*), void* data) : mImpl(new Impl(function, data)) {
    // Handled in initializer list
  }
  
  /* Thread dtor cleans up impl, implicitly cleaning up state. */
  Thread::~Thread() {
    delete mImpl;
  }
  
  /* join just joins the underlying thread. */
  void Thread::join() {
    WaitForSingleObject(mImpl->mHandle, INFINITE);
  }
}
