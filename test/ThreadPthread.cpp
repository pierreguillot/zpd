/****************************************************
 * File: ThreadPthread.cpp
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * An implementation of the Thread class using
 * pthreads.
 */

#include "Thread.hpp"
#include <pthread.h>

namespace synch {

  /* A struct holding all of the necessary data for the thread to run.
   * This stores a pointer to a nullary function, as well as a unary
   * function with an argument.  At most one of the two are valid.
   */
  struct ExecParams {
    void (*mNullaryFunction)();
    void (*mUnaryFunction)(void *);
    void* mUnaryData;

    /* Utility constructor to set the appropriate fields. */
    ExecParams(void (*nullaryFunction)(), void (*unaryFunction)(void*),
               void* data) : mNullaryFunction(nullaryFunction),
                             mUnaryFunction(unaryFunction),
                             mUnaryData(data) {
      // Handled in initializer list. */
    }
  };

  /* Two thread execution functions, one of which runs a unary function and one
   * of which runs a nullary function.
   */
  static void* NullaryExecutionFunction(void* data);
  static void* UnaryExecutionFunction(void* data);
  
  /* The ThreadImpl is a wrapper around a pthread object and the specified
   * callback.
   */
  struct Thread::Impl {
    ExecParams mParams; // Parameters for function execution
    pthread_t mThread;
    
    /* Constructor for no-arg function stores the function, then fires off
     * the wrapper function.
     */
    Impl(void (*function)(void)) : mParams(function, NULL, NULL) {
      LaunchThread(NullaryExecutionFunction);
    }

    /* Constructor for the one-arg function just fires off a thread to 
     * perform that function.
     */
    Impl(void (*function)(void*), void* data) : mParams(NULL, function, data) {
      LaunchThread(UnaryExecutionFunction);
    }
    
    /* Launches a thread to run the specified callback using the stored parameters
     * as an argument.
     */
    void LaunchThread(void* (*function)(void *)) {
      /* Force the created thread to be joinable. */
      pthread_attr_t attributes;
      pthread_attr_init(&attributes);
      pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
      
      /* Create the new thread. */
      pthread_create(&mThread, &attributes, function, &mParams);
      
      /* Clean up the attributes. */
      pthread_attr_destroy(&attributes);
    }
  };
  
  /* Both execution functions typecast the arguments to ExecParams,
   * then fire off the proper function.
   */
  static void* UnaryExecutionFunction(void* data) {
    ExecParams* params = static_cast<ExecParams*>(data);
    params->mUnaryFunction(params->mUnaryData);
    return NULL;
  }
  static void* NullaryExecutionFunction(void* data) {
    static_cast<ExecParams*>(data)->mNullaryFunction();
    return NULL;
  }
  
  /* Thread ctors forward the function to ThreadImpl. */
  Thread::Thread(void function(void)) : mImpl(new Impl(function)) {
    // Handled by initializer list.
  }
  Thread::Thread(void function(void*), void* data) : mImpl(new Impl(function, data)) {
    // Handled by initializer list.
  }
  
  /* Thread dtor deletes the implementation. */
  Thread::~Thread() {
    delete mImpl;
  }
  
  /* join just joins the underlying thread. */
  void Thread::join() {
    pthread_join(mImpl->mThread, NULL);
  }
}
