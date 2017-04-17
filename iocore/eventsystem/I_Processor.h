//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_I_PROCESSOR_H
#define TEST_LOCK_I_PROCESSOR_H

class Processor
{
public:
    virtual ~Processor();

    /**
      Returns a Thread appropriate for the processor.

      Returns a new instance of a Thread or Thread derived class of
      a thread which is the thread class for the processor.

      @param thread_index reserved for future use.

    */
    virtual Thread *create_thread(int thread_index);

    /**
      Returns the number of threads required for this processor. If
      the number is not defined or not used, it is equal to 0.

    */
    virtual int get_thread_count();

    /**
      This function attemps to stop the processor. Please refer to
      the documentation on each processor to determine if it is
      supported.

    */
    virtual void
    shutdown()
    {
    }

    /**
      Starts execution of the processor.

      Attempts to start the number of threads specified for the
      processor, initializes their states and sets them running. On
      failure it returns a negative value.

      @param number_of_threads Positive value indicating the number of
          threads to spawn for the processor.
      @param stacksize The thread stack size to use for this processor.

    */
    virtual int
    start(int number_of_threads, size_t stacksize = DEFAULT_STACKSIZE)
    {
        (void)number_of_threads;
        (void)stacksize;
        return 0;
    }

protected:
    Processor();

private:
    // prevent unauthorized copies (Not implemented)
    Processor(const Processor &);
    Processor &operator=(const Processor &);
};

#endif //TEST_LOCK_I_PROCESSOR_H
