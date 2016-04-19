/******************************************************
 * File: Thread.hh
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * A thread class that represents threads independently
 * of the library that's used to implement them.  The
 * idea is to build a thread package that can work
 * with any of:
 *
 * - C++0x
 * - Boost
 * - pthread
 * - Win32
 */
#ifndef Thread_Included
#define Thread_Included

#include <map>

namespace synch {
  class Thread {
  public:
    /**
     * Constructor: Thread(void startRoutine(void));
     * ------------------------------------------------
     * Fires off a new thread to execute the specified
     * routine.
     */
    explicit Thread(void startRoutine(void));

    /**
     * Constructor: Thread(void startRoutine(void*), void* data);
     * -----------------------------------------------
     * Fires off a new thread to execute the specified
     * routine, passing in the specified parameter.
     */
    Thread(void startRoutine(void*), void* data);
    
    /**
     * Destructor: ~Thread();
     * ------------------------------------------------
     * Cleans any resources allocated by the Thread
     * object.  You must not allow running threads to
     * go out of scope, as this results in undefined
     * behavior.
     */
    ~Thread();
    
    /**
     * void join();
     * ------------------------------------------------
     * Blocks until the thread terminates.
     */
    void join();
    
  private:
    /* No copying. */
    Thread(const Thread&);
    Thread& operator= (const Thread&);
    
    /* pImpl idiom to hide implementation details. */
    struct Impl;
    Impl* mImpl;
  };
}

#endif
