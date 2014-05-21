/*
 * par_mem.c - determine the memory hierarchy parallelism
 *
 * usage: par_mem [-L <line size>] [-M len[K|M]] [-W <warmup>] [-N <repetitions>]
 *
 * Copyright (c) 2000 Carl Staelin.
 * Copyright (c) 1994 Larry McVoy.  Distributed under the FSF GPL with
 * additional restriction that results may published only if
 * (1) the benchmark is unmodified, and
 * (2) the version in the sccsid below is included in the report.
 * Support for this development by Sun Microsystems is gratefully acknowledged.
 */
#include "bench.h"

void compute_times(struct mem_state* state, double* tlb_time, double* cache_time);


/*
 * Assumptions:
 *
 * 1) Cache lines are a multiple of pointer-size words
 * 2) Cache lines are no larger than 1/8 of a page (typically 512 bytes)
 * 3) Pages are an even multiple of cache lines
 */
int
main(int ac, char **av)
{
	int	i;
	int	c;
	int	warmup = 0;
	int	repetitions = (1000000 <= get_enough(0) ? 1 : TRIES);
	size_t	maxlen = 64 * 1024 * 1024;
	double	par;
	struct mem_state state;
	char   *usage = "[-L <line size>] [-M len[K|M]] [-W <warmup>] [-N <repetitions>]\n";

	state.line = getpagesize() / 16;
	state.pagesize = getpagesize();

	while (( c = getopt(ac, av, "L:M:W:N:")) != EOF) {
		switch(c) {
		case 'L':
			state.line = atoi(optarg);
			if (state.line < sizeof(char*))
				state.line = sizeof(char*);
			break;
		case 'M':
			maxlen = bytes(optarg);
			break;
		case 'W':
			warmup = atoi(optarg);
			break;
		case 'N':
			repetitions = atoi(optarg);
			break;
		default:
			lmbench_usage(ac, av, usage);
			break;
		}
	}

	for (i = MAX_MEM_PARALLELISM * state.line; i <= maxlen; i<<=1) { 
		par = par_mem(i, warmup, repetitions, &state);

		if (par > 0.) {
			fprintf(stderr, "%.6f %.2f\n", 
				i / (1000. * 1000.), par);
		}
	}

	exit(0);
}

