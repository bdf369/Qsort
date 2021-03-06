/*
 * qsort -- test and measure timing for the qsort algorithm
 * Copyright 2011 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <config.h>
#include <stdio.h>
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#include <getopt.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef HAVE_CLOCK_GETTIME
#define HAVE_CLOCK_GETTIME 0
#endif /* HAVE_CLOCK_GETTIME */

/*
 * Swap contents of arguments a and b
 */
static inline void
swap (int *a, int *b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

static void
setpivot (int *array, int len)
{
  if (MEDIAN_PIVOT)
    {
      int mid = len/2;
      if (array[0] > array[mid]) swap(&array[0], &array[mid]);
      if (array[mid] > array[len-1]) swap(&array[mid], &array[len-1]);
      if (array[0] > array[mid]) swap(&array[0], &array[mid]);
    }
  else
    {
      int pidx = rand () % len;
      swap (&array[0], &array[pidx]);
    }
}

/*
 * Sort integer array of length len using Quicksort algorithm
 */
static void
quicksort (int *array, int len)
{
  if (len <= 1)
    return;
  setpivot(array, len);
  int i,last=0;
  for (i=1; i<len; ++i)
    {
      if (array[i] < array[0])
        swap (&array[i], &array[++last]);
    }
  swap (&array[0], &array[last]);
  quicksort (&array[0], last);
  quicksort (&array[(last+1)], len-(last+1));
}

/*
 * Display version information
 */
static void
version (void)
{
  printf ("%s\n"
          "Copyright 2011 Google Inc.\n"
          "License: Apache License, Version 2.0, available at\n"
          "  http://www.apache.org/licenses/LICENSE-2.0\n"
          "This program comes with ABSOLUTELY NO WARRANTY.\n",
          PACKAGE_STRING);
  exit(0);
}

/*
 * Display usage information
 */
static void
usage (int errcode)
{
  fprintf (stderr,
           "Usage: qsort --count=<count> [optargs]\n"
           "       <count> is the number of random integers to sort\n"
           "       optargs:\n"
           "       --print     print out sorted array\n"
           "       --timing    print sort time in nanoseconds\n"
           "       --help      print usage information\n"
           "       --version   output version information and exit\n"
           "Report bugs to: %s\n"
           "%s home page: %s\n",
           PACKAGE_BUGREPORT, PACKAGE_NAME, PACKAGE_URL);
  exit(errcode);
}

/*
 * Program options array
 */
static struct option long_options[] = {
  {"count",    required_argument,  0,   'c'},
  {"print",    no_argument,        0,   'p'},
  {"timing",   no_argument,        0,   't'},
  {"help",     no_argument,        0,   'h'},
  {"version",  no_argument,        0,   'v'},
  {0, 0, 0, 0}
};

/*
 * Program main
 *
 * Parse arguments, generate an array of random integers to sort,
 * then sort array while measuring timing. Optionally display
 * sort time and/or sorted array.
 */
int
main (int argc, char **argv)
{
  int vsize, print, timing;

  print = timing = 0;
  vsize = 0;

  while (1)
    {
      int c, option_index = 0;
      c = getopt_long (argc, argv, "c:hpt", long_options, &option_index);
      if (c == -1)
        break;
      switch (c)
        {
        case 'c':
          vsize = atoi (optarg);
          break;
        case 'p':
          print = 1;
          break;
        case 't':
          timing = 1;
          break;
        case 'h':
          usage(0);
          break;
        case 'v':
          version();
          break;
        case '?':
          break;
        default:
          abort();
        }
    }
  
  if (!vsize)
    {
      fprintf (stderr, "required count not specified!\n");
      usage (1);
    }

  int *testvec = malloc (sizeof(int) * vsize);
  if (!testvec)
    {
      fprintf (stderr, "failed to allocate memory for %d integers\n", vsize);
      exit (1);
    }

  if (timing && !HAVE_CLOCK_GETTIME)
    {
      fprintf (stderr, "Warning: No clock_gettime(), timing not supported\n");
    }

  int i;
  srand(getpid());
  for (i=0; i<vsize; i++)
    testvec[i] = rand () % 1000;

  struct timespec before, after;

  if (HAVE_CLOCK_GETTIME)
    {
      clock_gettime (CLOCK_MONOTONIC, &before);
    }
  
  quicksort (testvec, vsize);

  if (HAVE_CLOCK_GETTIME)
    {
      clock_gettime (CLOCK_MONOTONIC, &after);
    }

  if (print)
    {
      printf ("{");
      for (i = 0; i<vsize; i++)
        printf ("%d%s", testvec[i], i==(vsize-1) ? "}\n" : ", ");
    }

  if (HAVE_CLOCK_GETTIME && timing)
    {
      unsigned long diffsec = after.tv_sec - before.tv_sec;
      unsigned long diffnsec = after.tv_nsec - before.tv_nsec;
      unsigned long nsec = diffsec * 1000000000;
      nsec += diffnsec;
      printf ("%lu\n", nsec);
    }

  free (testvec);
  exit (0);
}
