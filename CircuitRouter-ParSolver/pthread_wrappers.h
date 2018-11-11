/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * pthread_wrappers.h
 *
 * wrappers that simplify error handling in calls to pthread
 *
 * all functions have the corresponding pthread call, 
 * and their signature as directly copied from the pthread.h file 
 * (an action code was added as the first argument)
 *
 * not all functions are contemplated
 *
 * this code hinges on fact that pthread functions return valid error codes
 * for perror
 * =============================================================================
 */

#ifndef _PTHREAD_WRAPPERS_H
#define _PTHREAD_WRAPPERS_H
typedef enum {
  abort_exec, 	// print error message and abort
  print_error, 	// print error message
  ignore, 		// ignore :: bad!
} wrapper_action;

#include <pthread.h>
#include <errno.h>
#include <stdio.h>

extern int Pthread_create (wrapper_action action, 
    		   pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg) ;

extern void Pthread_exit (wrapper_action action, void *__retval) __attribute__ ((__noreturn__));

extern int Pthread_join (wrapper_action action, pthread_t __th, void **__thread_return);
extern int Pthread_tryjoin_np (wrapper_action action, pthread_t __th, void **__thread_return) ;
extern int Pthread_timedjoin_np (wrapper_action action, pthread_t __th, void **__thread_return,
				 const struct timespec *__abstime);

extern int Pthread_detach (wrapper_action action, pthread_t __th) ;
extern int Pthread_attr_init (wrapper_action action,pthread_attr_t *__attr) ;
extern int Pthread_attr_destroy (wrapper_action action,pthread_attr_t *__attr) ;

extern int Pthread_mutex_init (wrapper_action action, pthread_mutex_t *__mutex,
			       const pthread_mutexattr_t *__mutexattr);

extern int Pthread_mutex_destroy (wrapper_action action, pthread_mutex_t *__mutex);

extern int Pthread_mutex_trylock (wrapper_action action, pthread_mutex_t *__mutex);

extern int Pthread_mutex_lock (wrapper_action action, pthread_mutex_t *__mutex);

extern int Pthread_mutex_unlock (wrapper_action action, pthread_mutex_t *__mutex);

extern int Pthread_mutexattr_init (wrapper_action action, pthread_mutexattr_t *__attr);
 
extern int Pthread_mutexattr_destroy (wrapper_action action, pthread_mutexattr_t *__attr);

#endif	/* pthread_wrappers.h */
