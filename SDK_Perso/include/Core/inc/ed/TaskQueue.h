// asynchronous task queue interface
// written by dsb@eagle.ru
#ifndef _ed_TaskQueue_h_
#define _ed_TaskQueue_h_

#include <memory>
#include "../_config.h"

namespace ed {

class TaskQueue {
public:
    /**
     * Executes all pending tasks and destructs the queue.
     */
    virtual ~TaskQueue() = 0 {};

    /**
     * Pushes task to the queue.
     * If queue is full, waits until there's some space.
     *
     * Thread-safe.
     */
    virtual void push_task(void (*func)(void*), void* arg) = 0;

    /**
     * Pushes task to the queue, returns true on success.
     * if queue is full, returns false.
     *
     * Thread-safe.
     */
    virtual bool try_push_task(void (*func)(void*), void* arg) = 0;

    /**
     * Waits until all the tasks which were pushed before the barrier are completed.
     *
     * Thread-safe.
     */
    virtual void barrier() = 0;


    /**
     * Specifies queue attributes, such as task allocator.
     * By default, all queues are unbounded and share the same global allocator.
     */
    struct Attributes;

    /**
     * Create new task queue.
     * Name is used for informational purposes only.
     */
    static ED_CORE_EXTERN std::unique_ptr<TaskQueue> create(const char* name, Attributes* attr = nullptr);

};

typedef std::unique_ptr<TaskQueue> TaskQueuePtr;

} // namespace ed

#endif /* _ed_TaskQueue_h_ */
