#include <stdio.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
	uint32_t N = 0, mask = 1;
	uint8_t K = 0;
	
	scanf("%" SCNu32, &N);

	for(uint8_t i = 0; i < 32; i++)
	{
		uint32_t temp  = N & mask;

		if(temp) K++;

		mask <<= 1;
	}
		
	printf("%" PRIu8, K);
	return 0;
}
