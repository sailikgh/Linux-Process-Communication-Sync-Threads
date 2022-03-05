#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>
#include <errno.h>

#define NUM_THREADS 4

/* Initialize the semaphore. */
sem_t sem;

long int num_of_points_inside = 0;

/* Generate a random floating point number from min to max */
double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

/* Count points inside circle */
void* countCirclePoints(void *threadid) {
	int i;
	double radius;
	double x;
	double y;
	for(int i = 0; i < 1000000; i++) {
		x = randfrom(-1.0, 1.0);
		y = randfrom(-1.0, 1.0);
		radius = sqrt( pow( (x), 2 ) + pow( (y), 2 ) );
		if (radius <= 1) {
			if (sem_wait(&sem) == -1){
				printf("Error in semaphore wait.\n");
				exit(-1);
			}
			
			/* Point inside circle */

			num_of_points_inside++;
			
			if (sem_post(&sem) == -1){
				printf("Error in semaphore post.\n");
				exit(-1);
			}
		}
	}
}

int main () {
   if (sem_init(&sem, 0, 1) == -1){
   	printf("Error in creating semaphore.\n");
   	return -1;
   }
   
   /* Thread creation */
   pthread_t threads[NUM_THREADS];
   int rc;
   int t;
   for(t = 0; t < NUM_THREADS; t++ ){
      printf("Creating thread %d\n", t);
      rc = pthread_create(&threads[t], NULL, countCirclePoints, NULL);
      if (rc) {
	 printf("Error in creating thread %d\n", t);
	 return -1;
      }
      else{
      	 printf("Successfully created thread %d\n", t);
      }
   }
   
   /* Waiting for threads to return */
	int ret;
	for (t = 0; t < NUM_THREADS; t++){
		void *retval;
		ret = pthread_join(threads[t], &retval);
		if (ret){
		    printf("Error in thread join.\n");
		    return -1;
		}
		else{
		    printf("Returned point count for thread %d\n", t);
		    }
	}
	
    /* Compute area of the circle */
    double area_of_circle = 4 * (num_of_points_inside/4000000.0);
    printf("Area of circle is %f\n", area_of_circle);
    
   pthread_exit(NULL);
}
