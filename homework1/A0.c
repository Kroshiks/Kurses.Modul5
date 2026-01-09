#include <stdio.h>
#include <limits.h>

int main(int argc, char **argv)
{
	int K = 0, N = 0, n = 0, n_max =  INT_MIN;

	scanf("%d" SCNd16, &N);

	for(int i = 0; i<N; i++)
	{
		scanf("%d" SCNd16, &n);

		if(n > n_max)
		{
			n_max = n;
			K = 1;
		}
		else if( n == n_max)
			K++;
	}
	
	printf("%d", K);
	return 0;
}

