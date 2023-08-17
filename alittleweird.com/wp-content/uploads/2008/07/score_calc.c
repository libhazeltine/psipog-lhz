/*
 * Written by Sean Connelly... in about an hour... so please don't hate me :-(
 * hereby released into the public domain
 * alittleweird.com
 */

/* uncomment this to test the speed of the algorithms */ 
/*#define TEST_SPEED 100*/


#include <stdio.h>
#ifdef TEST_SPEED
#include <sys/time.h>
#endif

#ifndef max
#define max(a, b)    ((b) > (a) ? (b) : (a))
#endif
#ifndef min
#define min(a, b)    ((b) < (a) ? (b) : (a))
#endif

int w1, w2, w3, w4, w5, wmb; /* globals... eeewwww */
int t1, t2, t3, t4, t5, tmb;

inline int calc_score()
{
	int d1 = w1 - t1;
	int d2 = w2 - t2;
	int d3 = w3 - t3;
	int d4 = w4 - t4;
	int d5 = w5 - t5;
	int dmb = wmb - tmb;
	return d1 * d1 + d2 * d2 + d3 * d3 + d4 * d4 + d5 * d5 + dmb * dmb;
}

unsigned int isqrt(unsigned int x)
{
	register unsigned int op, res, one;
	
	if (x == 0)
		return 0;

	op = x;
	res = 0;
	one = 1 << 30;

	while (one > op)
		one >>= 2;

	while (one != 0)
	{
		if (op >= res + one)
		{
			op = op - (res + one);
			res = res +  2 * one;
		}
		res >>= 1;
		one >>= 2;
	}

	return res;
}


int main(int argc, char *argv[])
{
	#ifdef TEST_SPEED
	struct timeval start, stop;
	int i;
	#endif

	if (argc != 13)
	{
		printf("Usage: score_calc w1 w2 w3 w4 w5 wmb p1 p2 p3 p4 p5 pmb\n");
		printf("  where w* are the winning numbers, and p* are the played numbers\n");
		printf("  numbers must be in sequential order... no error checking :-P\n");
		printf("  weeeee! alittleweird.com!!!!!!1!\n");
		return 0;
	}

	w1 = atoi(argv[1]);
	w2 = atoi(argv[2]);
	w3 = atoi(argv[3]);
	w4 = atoi(argv[4]);
	w5 = atoi(argv[5]);
	wmb = atoi(argv[6]);

	t1 = atoi(argv[7]);
	t2 = atoi(argv[8]);
	t3 = atoi(argv[9]);
	t4 = atoi(argv[10]);
	t5 = atoi(argv[11]);
	tmb = atoi(argv[12]);
	
	int score = calc_score();
	int in_range = 0;
	
	printf("score: %d\n", score);

	/*
	the original, unoptimized code
	it's goal is to count the number of tickets that have the same - or better - score as the current ticket
	this count is talleyed in the variable `in_range'
	lower score = better
	*/

	#ifdef TEST_SPEED
	gettimeofday(&start, NULL);
	for (i = 0; i < TEST_SPEED; i++)
	{
		in_range = 0;
		for (t1 = 1; t1 <= 52; t1++)
		{
			for (t2 = t1 + 1; t2 <= 53; t2++)
			{
				for (t3 = t2 + 1; t3 <= 54; t3++)
				{
					for (t4 = t3 + 1; t4 <= 55; t4++)
					{
						for (t5 = t4 + 1; t5 <= 56; t5++)
						{
							for (tmb = 1; tmb <= 46; tmb++)
							{
								if (calc_score() <= score)
									in_range++;
							}
						}
					}
				}
			}
		}
	}
	gettimeofday(&stop, NULL);
	printf("slowest result: %d in %f\n", in_range, (stop.tv_sec + stop.tv_usec / 1000000.0) - (start.tv_sec + start.tv_usec / 1000000.0));
	#endif

	/*
	fastest version, which will produce the same count `in_range' as the slow code... but faster :-)
	of course, it's a little harder to read ;-P
	*/

	#define loop_code(N, M) \
		md##N = isqrt(score); \
		bt##N = max(1, w##N - md##N ); \
		ed##N = min(M, w##N + md##N ) - w##N ; \
		for (a##N = bt##N - w##N , t##N = bt##N ; a##N <= ed##N ; a##N ++, t##N ++)
	#define loop_code_cond(N, M, P) \
		md##N = isqrt(score); \
		bt##N = max(1, w##N - md##N ); \
		if (bt##N <= t##P ) \
			bt##N = t##P + 1; \
		ed##N = min(M, w##N + md##N ) - w##N ; \
		for (a##N = bt##N - w##N , t##N = bt##N ; a##N <= ed##N ; a##N ++, t##N ++)

	int a1, a2, a3, a4, a5, amb;
	int md1, md2, md3, md4, md5, mdmb;
	int bt1, bt2, bt3, bt4, bt5, btmb;
	int ed1, ed2, ed3, ed4, ed5, edmb;
	
	#ifdef TEST_SPEED
	gettimeofday(&start, NULL);
	for (i = 0; i < TEST_SPEED; i++)
	{
		in_range = 0;
	#endif

		loop_code(1, 56)
		{
			score -= a1 * a1;
			loop_code_cond(2, 56, 1)
			{
				score -= a2 * a2;
				loop_code_cond(3, 56, 2)
				{
					score -= a3 * a3;
					loop_code_cond(4, 56, 3)
					{
						score -= a4 * a4;
						loop_code_cond(5, 56, 4)
						{
							score -= a5 * a5;
							loop_code(mb, 46)
								in_range++;
							score += a5 * a5;
						}
						score += a4 * a4;
					}					
					score += a3 * a3;
				}
				score += a2 * a2;
			}
			score += a1 * a1;
		}

	#ifdef TEST_SPEED
	}
	gettimeofday(&stop, NULL);
	printf("fastest result: %d in %f\n", in_range, (stop.tv_sec + stop.tv_usec / 1000000.0) - (start.tv_sec + start.tv_usec / 1000000.0));
	#endif

	/* output the results */
	printf("range: %d\n", in_range);
	printf("odds: 1 in %f\n", 175711536.0 / in_range);

	return 0;
}
