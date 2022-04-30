#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "square_matrix.h"
#include "unixtimer.h"


/************************************************************/
/* Do not change the code below                             */
/************************************************************/

#define DEFAULT_N           6
#define DEFAULT_NUM_THREADS 2

int main(int argc, char ** argv)
{
   size_t n = (argc < 2 ? DEFAULT_N : atol(argv[1]) );
   size_t num_threads = (argc < 3 ? DEFAULT_NUM_THREADS : atol(argv[2]) );
   if(argc < 2) {
      fprintf(stderr, "Usage: %s <matrix_order> <num_threads>\n", argv[0]);
      fprintf(stderr, "Currently using %d threads and a %dX%d matrix\n",
                      DEFAULT_NUM_THREADS, DEFAULT_N, DEFAULT_N);
   }

   square_matrix* m = new_square_matrix(n);
   assert(m != NULL);
   fill_square_matrix(m);

   start_timer();
   start_clock();
   square_matrix* m1 = transpose_square_matrix(m);
   printf("Sequential time: %lf wall clock sec, %lf CPU sec\n", clock_seconds(), cpu_seconds() );

   start_timer();
   start_clock();
   square_matrix* m2 = transpose_square_matrix_banded(m);
   printf("Sequential banded time: %lf wall clock sec, %lf CPU sec\n", clock_seconds(), cpu_seconds() );

   for(int t=1; t<=num_threads; t = (2*t>num_threads && t<num_threads ? num_threads : 2*t) ) {
      start_timer();
      start_clock();
      square_matrix* m3 = transpose_square_matrix_threads(m, t);
      printf("%d thread%s time: %lf wall clock sec, %lf CPU sec\n",
             t, (t==1 ? "" : "s"), clock_seconds(), cpu_seconds() );
      assert(0 == compare_square_matrices(m3, m1) );
      free_square_matrix(m3);
   }

   free_square_matrix(m);
   free_square_matrix(m1);
   free_square_matrix(m2);

   return 0;
}
