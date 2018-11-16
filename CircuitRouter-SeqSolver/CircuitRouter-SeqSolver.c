/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This code is an adaptation of the Lee algorithm's implementation originally included in the STAMP Benchmark
 * by Stanford University.
 *
 * The original copyright notice is included below.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) Stanford University, 2006. All Rights Reserved.
 * Author: Chi Cao Minh
 *
 * =============================================================================
 *
 * Unless otherwise noted, the following license applies to STAMP files:
 *
 * Copyright (c) 2007, Stanford University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 *   * Neither the name of Stanford University nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * =============================================================================
 *
 * CircuitRouter-SeqSolver.c
 *
 * =============================================================================
 */


#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/list.h"
#include "maze.h"
#include "router.h"
#include "lib/timer.h"
#include "lib/types.h"

enum param_types {
  PARAM_BENDCOST = (unsigned char)'b',
  PARAM_XCOST  = (unsigned char)'x',
  PARAM_YCOST  = (unsigned char)'y',
  PARAM_ZCOST  = (unsigned char)'z',
};

enum param_defaults {
  PARAM_DEFAULT_BENDCOST = 1,
  PARAM_DEFAULT_XCOST  = 1,
  PARAM_DEFAULT_YCOST  = 1,
  PARAM_DEFAULT_ZCOST  = 2,
};

bool_t global_doPrint = TRUE;
char* global_inputFile = NULL;
long global_params[256]; /* 256 = ascii limit */


/* =============================================================================
 * displayUsage
 * =============================================================================
 */
static void displayUsage (const char* appName){
  fprintf(stderr, "Usage: %s [options] <filename>\n", appName);
  fputs("\nOptions:                    (defaults)\n", stderr);
  fprintf(stderr, "  b       <INT>  [b]end cost     (%i)\n", PARAM_DEFAULT_BENDCOST);
  fprintf(stderr, "  x       <UINT>  [x] movement cost  (%i)\n", PARAM_DEFAULT_XCOST);
  fprintf(stderr, "  y       <UINT>  [y] movement cost  (%i)\n", PARAM_DEFAULT_YCOST);
  fprintf(stderr, "  z       <UINT>  [z] movement cost  (%i)\n", PARAM_DEFAULT_ZCOST);
  fprintf(stderr, "  h           [h]elp message    (false)\n");
  exit(1);
}


/* =============================================================================
 * setDefaultParams
 * =============================================================================
 */
static void setDefaultParams (){
  global_params[PARAM_BENDCOST] = PARAM_DEFAULT_BENDCOST;
  global_params[PARAM_XCOST]  = PARAM_DEFAULT_XCOST;
  global_params[PARAM_YCOST]  = PARAM_DEFAULT_YCOST;
  global_params[PARAM_ZCOST]  = PARAM_DEFAULT_ZCOST;
}


/* =============================================================================
 * parseArgs
 * =============================================================================
 */
static void parseArgs (long argc, char* const argv[]){
  long i;
  long opt;
  opterr = 0;

  setDefaultParams();

  while ((opt = getopt(argc, argv, "hb:x:y:z:")) != -1) {
    switch (opt) {
      case 'b':
      case 'x':
      case 'y':
      case 'z':
        global_params[(unsigned char)opt] = atol(optarg);
        break;
      case '?':
      case 'h':
      default:
        opterr++;
        break;
    }
  }

  for (i = optind; i < argc; i++) {
    if (!global_inputFile) {
      if (access(argv[i], R_OK) == -1) {
        fprintf(stderr, "Non-existing file: %s\n", argv[i]);
        opterr++;
        i = argc; // break
      }
      else {
        global_inputFile = argv[i];
      }
    }
    else {
     fprintf(stderr, "Extra argument: %s\n", argv[i]);
     opterr++;
    }
  }

  if (opterr || !global_inputFile) {
    displayUsage(argv[0]);
  }
}

/* =============================================================================
 * open_out_stream
 * =============================================================================
 */
FILE * open_out_stream(const char * const input_filename) 
{
  FILE *fp;
  size_t input_len = strlen(input_filename);
  char out_filename[(input_len + 4 + 1)];
  
  strncpy(out_filename, input_filename, input_len + 1);
  strcat(out_filename, ".res");

  if (access(out_filename, R_OK) == 0) {
    // renaming .res to .res.old
    char old_filename[(input_len + 8 + 1)];

    strncpy(old_filename, out_filename, input_len + 4 + 1);
    strcat(old_filename, ".old");
    if (rename(out_filename, old_filename) == -1) {
      perror("open_out_stream: rename");
      return NULL;
    }
  }

  fp = fopen(out_filename, "w");
  if (fp == NULL) {
    perror("open_out_stream: fopen");
    exit(1);
  }
  return fp;
}


/* =============================================================================
 * main
 * =============================================================================
 */
int main(int argc, char** argv){
  /*
   * Initialization
   */
  parseArgs(argc, (char** const)argv);
  maze_t* mazePtr = maze_alloc();
  assert(mazePtr);

  FILE * out_stream = open_out_stream(global_inputFile);
  assert(out_stream);
  
  long numPathToRoute = maze_read(mazePtr, global_inputFile, out_stream);
  router_t* routerPtr = router_alloc(global_params[PARAM_XCOST],
                    global_params[PARAM_YCOST],
                    global_params[PARAM_ZCOST],
                    global_params[PARAM_BENDCOST]);
  assert(routerPtr);
  list_t* pathVectorListPtr = list_alloc(NULL);
  assert(pathVectorListPtr);

  router_solve_arg_t routerArg = {routerPtr, mazePtr, pathVectorListPtr};
  TIMER_T startTime;
  TIMER_READ(startTime);

  router_solve((void *)&routerArg);

  TIMER_T stopTime;
  TIMER_READ(stopTime);

  long numPathRouted = 0;
  list_iter_t it;
  list_iter_reset(&it, pathVectorListPtr);
  while (list_iter_hasNext(&it, pathVectorListPtr)) {
    vector_t* pathVectorPtr = (vector_t*)list_iter_next(&it, pathVectorListPtr);
    numPathRouted += vector_getSize(pathVectorPtr);
  }
  fprintf(out_stream, "Paths routed  = %li\n", numPathRouted);
  fprintf(out_stream, "Elapsed time  = %f seconds\n", TIMER_DIFF_SECONDS(startTime, stopTime));


  /*
   * Check solution and clean up
   */
  assert(numPathRouted <= numPathToRoute);
  bool_t status = maze_checkPaths(mazePtr, pathVectorListPtr, global_doPrint, out_stream);
  assert(status == TRUE);
  fputs("Verification passed.", out_stream);
  fclose(out_stream);

  maze_free(mazePtr);
  router_free(routerPtr);

  list_iter_reset(&it, pathVectorListPtr);
  while (list_iter_hasNext(&it, pathVectorListPtr)) {
    vector_t* pathVectorPtr = (vector_t*)list_iter_next(&it, pathVectorListPtr);
    vector_t* v;
    while((v = vector_popBack(pathVectorPtr))) {
      // v stores pointers to longs stored elsewhere; no need to free them here
      vector_free(v);
    }
    vector_free(pathVectorPtr);
  }
  list_free(pathVectorListPtr);


  return 0;
}


/* =============================================================================
 *
 * End of CircuitRouter-SeqSolver.c
 *
 * =============================================================================
 */
