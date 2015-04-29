#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

void swap(int *yi, int *yj)
{  int tmp = *yi;
   *yi = *yj;
   *yj = tmp;
}

int separate(int *x, int low, int high)
{
//   printf("Thread Num %d out of %d - zstart|zend %d|%d\n", omp_get_thread_num(), omp_get_num_threads(), low, high);

   int i,pivot,last;
   pivot = x[low];  // would be better to take, e.g., median of 1st 3 elts
   swap(x+low,x+high);
   last = low;
   for (i = low; i < high; i++) {
      if (x[i] <= pivot) {
         swap(x+last,x+i);
         last += 1;
      }
   }
   swap(x+last,x+high);
   return last;
}

// quicksort of the array z, elements zstart through zend; set the
// latter to 0 and m-1 in first call, where m is the length of z;
// firstcall is 1 or 0, according to whether this is the first of the
// recursive calls
void qs(int *z, int zstart, int zend, int firstcall)
{
    if(firstcall == 1) {
        #pragma omp parallel
        {
            #pragma omp single nowait
            qs(z, zstart, zend, 0);
        }
    }
    else {
        int part;
        if (zstart < zend) {
            part = separate(z,zstart,zend);
            #pragma omp task
            qs(z,zstart,part-1,0);
            #pragma omp task
            qs(z,part+1,zend,0);
        }
    }
}

#define MINARG 2
int main(int argc, char**argv)
{  int i,n,*w;
   if(argc<MINARG) {
	   fprintf(stderr,"Usage %s n [print flag]\n",argv[0]);
	   exit(1);
   }
   n = atoi(argv[1]);
   w = malloc(n*sizeof(int));
   for (i = 0; i < n; i++) w[i] = rand();
   double startime,endtime;
   startime = omp_get_wtime();
   qs(w,0,n-1, 1);
   endtime = omp_get_wtime();
   printf("elapsed time:  %f\n",endtime-startime);
   if(argc>MINARG && atoi(argv[2])) {
      for (i = 0; i < n; i++) printf("%d\n",w[i]);
   }
}
