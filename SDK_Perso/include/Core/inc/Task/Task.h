#ifndef EDSIM_CORE_TASK_H
#define EDSIM_CORE_TASK_H

#include "_config.h"

#include <stdint.h>

#ifndef ED_CORE_EXPORT
	#define ED_CORE_EXPORT EDSIM_DLL_IMPORT
#endif // ED_CORE_EXPORT

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ED_task_job_t)(void *);

typedef struct ED_serial_queue_t_struct* ED_serial_queue_t;

static const ED_serial_queue_t INVALID_SERIAL_QUEUE = (ED_serial_queue_t)~0;

#define IO_QUEUE "IOQueue"

ED_CORE_EXPORT void ED_task_setAffinityMask(uint32_t mask);

/// Adds task to serial queue.
/// \param q serial queue handle to add task to. 
/// \param arg argument of task.
/// \param job  task to run.
ED_CORE_EXPORT void ED_add_serial_queue_task(ED_serial_queue_t q, void* arg, ED_task_job_t job);

/// Adds task which'll run in main thread.
/// \param arg argument of task.
/// \param job task to run.
ED_CORE_EXPORT void ED_add_main_thread_task(void* arg, ED_task_job_t job);

/// Creates new serial queue with given name and returns its handle. If queue with given name exists retruns this queue handle.
ED_CORE_EXPORT ED_serial_queue_t ED_create_serial_queue(const char* name);

/// Returns serial queue with given name. If there is no such queue returns INVALID_SERIAL_QUEUE.
ED_CORE_EXPORT ED_serial_queue_t ED_get_serial_queue(const char* name);

/// Waits while serial queue finish all its jobs.
ED_CORE_EXPORT void ED_wait_serial_queue(ED_serial_queue_t q);

/// Removes all tasks except running one from given serial queue.
ED_CORE_EXPORT void ED_clear_serial_queue(ED_serial_queue_t q);

/// Waits while all serial queues finish their jobs. Don't call it outside of main thread.
/// This fuction also run tasks scheduled for main thread.
ED_CORE_EXPORT void ED_wait_all_serial_queues();

/// This function must be run from main thread regulary to make tasks scheduled to main thread.
/// \param secondsMax holds time allowed for this function.
/// \param nJobs holds maximum allowed jobs.
ED_CORE_EXPORT void ED_make_main_thread_task(double secondsMax = 1e10, unsigned int nJobs = 1000000);

/// Waits while io queue finishes its jobs. . Don't call it outside of main thread.
/// This fuction also run tasks scheduled for main thread.
inline void ED_wait_io_queue()
{
	ED_make_main_thread_task();
	auto ioQueue = ED_get_serial_queue(IO_QUEUE);
	ED_wait_serial_queue(ioQueue);
	ED_make_main_thread_task();
}

#ifdef __cplusplus
}
#endif

#endif // EDSIM_CORE_TASK_H
