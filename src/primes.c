/* Find prime numbers in a given range.
 *
 * Home: https://github.com/n3xus6
 * 
 * Copyright (C) 2022 by A. Heinemann. All rights reserved.
 * This program comes with ABSOLUTELY NO WARRANTY.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

static bool isprim(int64_t x);
static int64_t sqrt_floor(int64_t x);
static void primes(int64_t start, int64_t end);

int main() {
	/* Example
	 * ------- 
	 * There are four prime numbers in the range from 1234567890123456789 to 1234567890123456889:
	 * '[1234567890123456817, 1234567890123456857, 1234567890123456869, 1234567890123456883]'
	 */
	const int64_t start = 1234567890123456789LL;
	const int64_t end = start + 100;

	primes(start, end);

	return 0;
}

/* Prints all primes from 'start' to 'end'. */
static void primes(int64_t start, int64_t end) {
	int64_t i = 0;

	putchar('[');
	for (i = start; i <= end; i++)
		if (isprim(i)) {
			printf("%lli", (long long int) i);
			i++;
			break;
		}
	for (; i <= end; i++)
		if (isprim(i)) printf(", %lli", (long long int) i);
	puts("]");
}

/* Tests whether 'x' is a prime number or not.
 */
static bool isprim(int64_t x) {
	/* Stop at numbers which are obviously not prime. */
	if (x < 2 || (x > 2 && (1 & x) == 0))
		return false;

	/* Because x = sqrt(x) * sqrt(x), when we have reached sqrt(x),
	 * all factors have been tested and we can stop.
	 * We would only have to test with factors that are prime (fundamental theorem of
	 * arithmetics) but we don't know them so we test with all odd numbers.
	 */
	for (int64_t i = 3, sqrt_x = sqrt_floor(x); i <= sqrt_x; i += 2)
		if (x % i == 0)	return false;
	
	return true;
}

/* Calculates the square root of 'x' using Newton's method.
 * The fractional part of the result is discarded.
 * At least for all positive 'x' <= 2^32 the return value
 * matches the significand from the return value of the
 * math.h sqrt function.
 */
static int64_t sqrt_floor(int64_t x) {
	int64_t sqrt_x = 0, prev = 0;

	if (x <= 0) return 0;
	if (x == 1) return 1;

	prev = x / 2;

	do {
		sqrt_x = prev;
		prev = (sqrt_x + (x / sqrt_x)) / 2;
	} while (prev < sqrt_x);

	return sqrt_x;
}
