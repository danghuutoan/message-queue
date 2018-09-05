# message-queue
a simple Implementation of a message queue
# Requirements
## The problem is to design and implement a priority queue in C, and provide a test application which demonstrates that the queue works.

### Here are a few requirements:

- Priority values are integers. Consumer should always receive a message with highest priority value available in the queue at the time.

- Priority queue should be thread-safe. Multiple threads should be able to write to the queue and read from the queue concurrently.

- Priority queue should generate “high water mark”/“low water mark” events. You may implement these events as callback functions.

 - High water mark event should be generated when queue is almost full (say, 90% full — this parameter should be configurable). 
    Writer threads can use this event to suspend writing to the queue. 

 - Low water mark event should be generated when the queue is almost empty (say, 10% — this parameter should be configurable as well). 
    Writer threads can use this event to resume writing to the queue.

  - Writer threads in the test application should use these events to suspend and resume writing.

- Priority queue should have a well-defined interface declared in the header file.

- Priority queue should be written in C or C++ (you may also use GNU extensions).

- Test application should create at least two threads writing into the queue and at least two threads reading from the queue. Message size should be configurable at compile time.

- Test application should compile and run on Linux x86, or Linux x64, or Windows, or macOS, at your choice. Feel free to use pthreads or WinAPI or C++ threads library.

- Test application should come with some instructions for compiling it — in the form of a Makefile, CMake script, or a project file for Visual Studio or Xcode


## What is priority queue ?

according to wikipedia:

a priority queue is an abstract data type which is like a regular queue or stack data structure, but where additionally each element has a "priority" associated with it. In a priority queue, an element with high priority is served before an element with low priority
