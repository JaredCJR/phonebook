#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"
#define REPEAT 100

static double diff_in_second(struct timespec t1, struct timespec t2)
{
	struct timespec diff;
	if (t2.tv_nsec - t1.tv_nsec < 0) {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
	} else {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
	}
	return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
	/*
	    FILE *fp_append;
	    FILE *fp_findName;

	#if defined(ORIG)

	    fp_append = fopen("./result/ORIG_append.txt","a");
	    fp_findName = fopen("./result/ORIG_findName.txt","a");

	    if( (fp_append == NULL)&&(fp_findName == NULL) )
	    {
	        printf("cannot open the ORIG_result_file\n");
	        return -1;
	    }
	#endif

	#if defined(murmurHash)

	    fp_append = fopen("./result/HASH_append.txt","a");
	    fp_findName = fopen("./result/HASH_findName.txt","a");

	    if( (fp_append == NULL)&&(fp_findName == NULL) )
	    {
	        printf("cannot open the HASH_result_file\n");
	        return -1;
	    }
	#endif
	*/
	double total_time_1 = 0, total_time_2 = 0;
	for (int i = 0; i < REPEAT; i++) {
		FILE *fp;
		int i = 0;
		char line[MAX_LAST_NAME_SIZE];
		struct timespec start, end;
		double cpu_time1 = 0, cpu_time2 = 0;

		/* check file opening */
		fp = fopen(DICT_FILE, "r");
		if (fp == NULL) {
			printf("cannot open the file\n");
			return -1;
		}
#if defined(ORIG)
		/* build the entry */
		entry *pHead, *e;
		pHead = (entry *) malloc(sizeof(entry));
		printf("size of entry : %lu bytes\n", sizeof(entry));
		e = pHead;
		e->pNext = NULL;
#endif




#if defined(__GNUC__ORIG)
#if defined(ORIG)
		__builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#endif

		system("./drop_cache.sh");

		clock_gettime(CLOCK_REALTIME, &start);
		while (fgets(line, sizeof(line), fp)) {
			while (line[i] != '\0')
				i++;
			line[i - 1] = '\0';
			i = 0;
#if defined(ORIG)
			e = append(line, e);
#endif

#if defined(murmurHash)
			append(line);
#endif
		}
		clock_gettime(CLOCK_REALTIME, &end);
		cpu_time1 = diff_in_second(start, end);

		/* close file as soon as possible */
		fclose(fp);

		/* the givn last name to find */
		char input[MAX_LAST_NAME_SIZE] = "zyxel";

#if defined(ORIG)
		e = pHead;
		assert(findName(input, e) &&
		       "Did you implement findName() in " IMPL "?");
		assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));

#endif



#if defined(__GNUC__)
#if defined(ORIG)
		__builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#endif
		/* compute the execution time */
		clock_gettime(CLOCK_REALTIME, &start);

#if defined(ORIG)
		findName(input, e);
#endif

#if defined(murmurHash)
		if (!findName(input)) {
			printf("NOT FOUND!\n");
			exit(0);
		} else {
			printf("FOUND!\n");
		}
#endif

		clock_gettime(CLOCK_REALTIME, &end);
		cpu_time2 = diff_in_second(start, end);

		printf("execution time of append() : %lf sec\n", cpu_time1);
		printf("execution time of findName() : %lf sec\n", cpu_time2);
		total_time_1 += cpu_time1;
		total_time_2 += cpu_time2;
		/*
		    fprintf(fp_append,"%lf\n", cpu_time1);//append
		    fprintf(fp_findName,"%lf\n", cpu_time2);//findName
		*/
#if defined(ORIG)
		while (pHead->pNext != NULL) {
			entry *temp = pHead;
			pHead = pHead->pNext;
			free(temp);
		}
		free(pHead);
#endif

#if defined(murmurHash)
		free_all();
#endif

		/*
		    fclose(fp_append);
		    fclose(fp_findName);
		*/

	}// end of REPEAT

	printf("Total execution time of %d times of append() : %lf sec\n", REPEAT, total_time_1);
	printf("Total execution time of %d times of findName() : %lf sec\n", REPEAT, total_time_2);
	return 0;
}

