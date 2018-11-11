/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * pthread_wrappers.h
 *
 * wrappers that simplify error handling in calls to pthread
 *
 * all functions have the corresponding pthread call, 
 * and their signature as directly copied from the pthread.h file 
 * (an action code was added as the first argument)
 * (an error string was added as the second argument)
 *
 * not all functions are contemplated
 * =============================================================================
 */

#include "pthread_wrappers.h"
#include <stdlib.h>

static void handle_error(wrapper_action action, int error_code, const char * const error_string)
{
  switch (action) {
    case abort_exec:
      errno = error_code;
      perror(error_string);
      fprintf(stderr, "pthread_wrappers: aborting execution\n");
      exit(-3);
      break;
    case print_error:
      errno = error_code;
      fprintf(stderr, "pthread_wrappers: ");
      perror(error_string);
    case ignore:
      break; // BAD
    default:
      fprintf(stderr, "pthread_wrappers: invalid action: aborting\n");
      exit(-4);
  }
}


int Pthread_create (wrapper_action action, const char * const error_string, 
    		   pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg)
{
  int ret = pthread_create(__newthread, __attr, __start_routine, __arg);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}


void Pthread_exit (wrapper_action action, const char * const error_string, void *__retval) 
{
  /* as per the man page:
   * "This function always succeeds."
   *
   * here represented to maintain consistency
   */
  pthread_exit(__retval);
}

int Pthread_join (wrapper_action action, const char * const error_string, pthread_t __th, void **__thread_return)
{
  int ret = pthread_join(__th, __thread_return);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}


int Pthread_detach (wrapper_action action, const char * const error_string, pthread_t __th) 
{
  int ret = pthread_detach (__th);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}

int Pthread_attr_init (wrapper_action action, const char * const error_string,pthread_attr_t *__attr) 
{
  int ret = pthread_attr_init(__attr);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}

int Pthread_attr_destroy (wrapper_action action, const char * const error_string,pthread_attr_t *__attr)
{
  int ret = pthread_attr_destroy(__attr);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}

int Pthread_mutex_init (wrapper_action action, const char * const error_string, pthread_mutex_t *__mutex,
			       const pthread_mutexattr_t *__mutexattr)
{
  int ret = pthread_mutex_init(__mutex, __mutexattr);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}


int Pthread_mutex_destroy (wrapper_action action, const char * const error_string, pthread_mutex_t *__mutex)
{
  int ret = pthread_mutex_destroy(__mutex);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}


int Pthread_mutex_trylock (wrapper_action action, const char * const error_string, pthread_mutex_t *__mutex)
{
  int ret = pthread_mutex_trylock(__mutex);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}


int Pthread_mutex_lock (wrapper_action action, const char * const error_string, pthread_mutex_t *__mutex)
{
  int ret = pthread_mutex_trylock(__mutex);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}


int Pthread_mutex_unlock (wrapper_action action, const char * const error_string, pthread_mutex_t *__mutex)
{
  int ret = pthread_mutex_unlock(__mutex);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}

int Pthread_mutexattr_init (wrapper_action action, const char * const error_string, pthread_mutexattr_t *__attr)
{
  int ret = pthread_mutexattr_init (__attr);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}


int Pthread_mutexattr_destroy (wrapper_action action, const char * const error_string, pthread_mutexattr_t *__attr)
{
  int ret = pthread_mutexattr_init (__attr);
  if (ret)
    handle_error(action, ret, error_string);

  return ret;
}

