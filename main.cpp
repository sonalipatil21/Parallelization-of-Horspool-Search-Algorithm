#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <vector>

#define NO_OF_CHARS 256
int bad_char[NO_OF_CHARS];



int* last_bad_char(char str[])
{
	memset(bad_char, -1, NO_OF_CHARS);

	int len = strlen(str);
	for (int i = 0; i < len - 1; i++)
	{
		char a = str[i];
		bad_char[a] = i;
	}
	return bad_char;
}


void bmh(char *text, int start, int end, char *pat, std::vector<int>* vec)
{
	int n = end - start + 1;
	int m = strlen(pat);
	int *bad_char = last_bad_char(pat);
	int i = start;

	while (i <= (end - m + 1))
	{
		int j = m - 1;
		while (j >= 0 && pat[j] == text[i+j])
			j--;

		if (j < 0){
			//vec.resize(vec.size() + 1);
			vec->push_back(i);
			//printf("Pattern found at %d %d\n", i, vec.size());
		}
		i += m - 1;
		i -= bad_char[text[i]];
	}
}

void search(char* text, char* pat, int n_threads)
{
	double start_time, elapsed_time;

	int chunk_size = strlen(text) / n_threads;
	int rem_size   = strlen(text) % n_threads;

	int pat_len = strlen(pat);
	printf("Length of pattern: %d\n", pat_len);

	int tid, start, end;
	std::vector<int> vec;
	std::vector<int> vec_local;

	start_time = omp_get_wtime();

	#pragma omp parallel num_threads(n_threads) private(tid,start,end,vec_local) shared(text,pat,rem_size,chunk_size,vec)
	{
		tid = omp_get_thread_num();
		//printf("tid  %d\n", tid);
		if (tid == 0)
		{
			#pragma omp critical (part1)
			{
				start = tid;
				end   = chunk_size - 1;
				//printf("start: %d end: %d\n", start, end);
				//printf("tid= %d  text block : %d ... %d\n", tid, start, end);
				bmh(text, start, end, pat, &vec_local);
			}
		}
		else
		{
			#pragma omp critical (part2)
			{
				start = (tid * chunk_size) - pat_len;
				end   = (tid * chunk_size) + chunk_size - 1;
				//printf("start: %d end: %d\n", start, end);
				//printf("tid= %d  text block : %d ... %d\n", tid, start, end);
				bmh(text, start, end, pat, &vec_local);
			}
		}
		vec.insert(vec.end(), vec_local.begin(), vec_local.end());
	}

	if (rem_size != 0)
		bmh(text, (n_threads + 1) * chunk_size, strlen(text), pat, &vec);

	elapsed_time = (omp_get_wtime() - start_time) * 1000;

	printf("Total found: %d\n", vec.size());
	for (std::vector<int>::iterator i = vec.begin(); i != vec.end(); ++i){
		printf("Pattern found at %d\n", *i);
	}
	printf("\nElapsed time: %f milliseconds\n", elapsed_time);
}

int main(int argc, char* argv[])
{
	char pat[32];
	int n_threads = 1;


	if (argc < 2)
		printf("\n Usage: HorspoolSearch test.txt [number of threads]");

	if (argc > 2)
	{
		n_threads = atoi(argv[2]);
		printf("\n Usage: HorspoolSearch test.txt [number of threads]");
		printf("\n      : quit\n");
	}


	/* Open your_file in read-only mode */
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		printf("\n Can't open file");
		return 0;
	}
	/* Get the buffer size */
	fseek(fp, 0, SEEK_END); /* Go to end of file */
	size_t size = ftell(fp); /* How many bytes did we pass ? */


	/* Allocate the buffer (no need to initialize it with calloc) */
	char* text = (char*)malloc((size + 1) * sizeof(*text)); /* size + 1 byte for the \0 */

	/* Read the file into the buffer */
	rewind(fp);	/* Set position of stream to the beginning */
	fread(text, size, 1, fp); /* Read 1 chunk of size bytes from fp into buffer */
	text[size] = '\0';	/* NULL-terminate the buffer */
	fclose(fp);

	printf("Chunk size: %d Rem size: %d\n", strlen(text) / n_threads, strlen(text) % n_threads);
	printf("num of threads %d\n", n_threads);
	printf("Length of text: %d\n", strlen(text));

	while (1)
	{
		printf("\nPlease enter search text: ");
		scanf("%s", pat);

		if (strcmp(pat, "quit") == 0)
			break;

		search(text, pat, n_threads);
	}
	return 0;
}