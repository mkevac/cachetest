#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

# define timespec_timersub(a, b, result)				      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;			      \
    (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec;			      \
    if ((result)->tv_nsec < 0) {					      \
      --(result)->tv_sec;						      \
      (result)->tv_nsec += 1000000000;					      \
    }									      \
  } while (0)

int main()
{
	const size_t min_size = (1 * 1024) / sizeof(int); // 1 KiB
	const size_t max_size = (1 * 1024 * 1024 * 1024) / sizeof(int); // 1 GiB
	const int repeats = 100;
	
	for (size_t asize = min_size; asize <= max_size; asize *= 2) {
		int *a = calloc(asize, sizeof(*a));
		if (!a) {
			fprintf(stderr, "no mem\n");
			exit(EXIT_FAILURE);
		}

		memset(a, 1, asize*sizeof(int));

		struct timespec begin;
		struct timespec end;
		struct timespec res;

		if (-1 == clock_gettime(CLOCK_MONOTONIC, &begin)) {
			perror("clock_gettime()");
			exit(EXIT_FAILURE);
		}

		for (int r = 0; r < repeats; r++) {
			for (int i = 0; i < asize; i += 16) {
				a[i] *= 3;
			}
		}

		if (-1 == clock_gettime(CLOCK_MONOTONIC, &end)) {
			perror("clock_gettime()");
			exit(EXIT_FAILURE);
		}

		timespec_timersub(&end, &begin, &res);

		double dres = (double)res.tv_sec + (double)res.tv_nsec / 1000000000;

		if (asize*sizeof(int) >= 1*1024*1024) {
			printf("%zu MiB, [%d.%09d] %.9f sec\n", asize*sizeof(int)/1024/1024, res.tv_sec, res.tv_nsec, dres / asize);
		} else {
			printf("%zu KiB, [%d.%09d] %.9f sec\n", asize*sizeof(int)/1024, res.tv_sec, res.tv_nsec, dres / asize);
		}

		free(a);
	}

	return 0;
}

