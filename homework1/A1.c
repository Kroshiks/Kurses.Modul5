#include <stdio.h>

int main(int argc, char **argv)
{
	int K = 0;
	char S;

	scanf("%d", &K);

	while(1)
	{
		scanf("%c", &S);
		if(S == '.')
		{
			printf("%c", S);
			break;
		}

		if(K > 26) K = K % 26;

		if(S >= 'A' && S <= 'Z')
        {
            printf("%c", 'A' + (S - 'A' + K) % 26);
        }
        else if(S >= 'a' && S <= 'z')
        {
			printf("%c", 'a' + (S - 'a' + K) % 26);
        }
        else
        {
            printf("%c", S);
        }
	}
	

	return 0;
}
