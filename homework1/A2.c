#include <stdio.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
	uint32_t N = 0;
	uint8_t K = 0;
	scanf("%" SCNu32, &N);
	scanf("%" SCNu8, &K);

	N = (N >> K) | (N << (32 - K));
		
	printf("%" PRIu32, N);
	return 0;
}

