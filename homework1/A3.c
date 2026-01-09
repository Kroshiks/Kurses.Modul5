#include <stdio.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
	uint32_t N = 0, mask = 0;
	uint8_t K = 0;
	scanf("%" SCNu32, &N);
	scanf("%" SCNu8, &K);

	for(uint8_t i = 0; i < K; i++)
	{
		mask <<= 1;
		mask++;
	}
	
	N = N & mask;
		
	printf("%" PRIu32, N);
	return 0;
}
