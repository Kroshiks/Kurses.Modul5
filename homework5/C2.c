#include <stdio.h>

int stack[255], pst = 0;

void push(int s)
{
    stack[pst++] = s;
}

int pop()
{
    return stack[--pst];
}

int main(int argc, char **argv)
{
    char S = 0;
    int num = 0;
    int is_reading_number = 0;

    while(1)
    {
        scanf("%c", &S);
        if(S == '.') break;
        
        if(S >= '0' && S <= '9')
        {
            if(is_reading_number)
            {
                num = num * 10 + (S - '0');
            }
            else
            {
                num = S - '0';
                is_reading_number = 1;
            }
        }
        else if(S == ' ')
        {
            if(is_reading_number)
            {
                push(num);
                num = 0;
                is_reading_number = 0;
            }
        }
        else if(S == '+' || S == '-' || S == '*' || S == '/')
        {
            if(is_reading_number)
            {
                push(num);
                num = 0;
                is_reading_number = 0;
            }
            
            int b = pop();  
            int a = pop(); 
            int result = 0;
            
            switch(S)
            {
                case '+':
                    result = a + b;
                    break;
                case '-':
                    result = a - b;
                    break;
                case '*':
                    result = a * b;
                    break;
                case '/':
                    if(b == 0) 
                    {
                        printf("Ошибка: деление на ноль\n");
                        return 1;
                    }
                    result = a / b;
                    break;
            }
            
            push(result);
        }
    }

    if(is_reading_number)
    {
        push(num);
    }

    int itog = pop();
    printf("%d", itog);

    return 0;
}
