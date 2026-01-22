#include <stdio.h>
#include <string.h>

int main() {
	int possible_len1[10] = {0}; 
    int possible_len2[100] = {0}; 
    int possible_len3[1000] = {0}; 

    char digits[1000];
    scanf("%s", digits);

    int len = strlen(digits);
    
    for (int i = 0; i < len; i++)
    {
        int c = digits[i] - '0';
        
        for (int num = 0; num < 100; num++)
        {
            if (possible_len2[num])
            {
                int three_digit = num * 10 + c;
                if (three_digit >= 100 && three_digit <= 999)
                    possible_len3[three_digit] = 1;
            }
        }
        
        for (int num = 0; num < 10; num++)
        {
            if (possible_len1[num])
            {
                int two_digit = num * 10 + c;
                if (two_digit >= 10 && two_digit <= 99)
                    possible_len2[two_digit] = 1;
            }
        }
        
        if (c != 0)
            possible_len1[c] = 1;
    }
    
    int count = 0;
    for (int num = 100; num <= 999; num++)
    {
        if (possible_len3[num])
            count++;
    }
    printf("%d\n", count);
    return 0;
}
