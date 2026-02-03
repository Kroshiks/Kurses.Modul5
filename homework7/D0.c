#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct list {
    char word[20];
    struct list *next;
};

// Функция для добавления элемента в список
struct list* add_to_list(struct list *head, const char *word) {
    struct list *new_node = (struct list*)malloc(sizeof(struct list));
        
    strncpy(new_node->word, word, 19);
    new_node->word[19] = '\0'; 
    new_node->next = NULL;
    
    if (head == NULL)
     {
        return new_node;
    }
    
    struct list *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    
    current->next = new_node;
    return head;
}

// Функция для обмена содержимым двух элементов списка
void swap_elements(struct list *a, struct list *b) {
    char temp[20];
    strcpy(temp, a->word);
    strcpy(a->word, b->word);
    strcpy(b->word, temp);
}

// Функция для сортировки списка по алфавиту (пузырьковая сортировка)
void sort_list(struct list *head) {
    int swapped;
    struct list *current;
    struct list *last = NULL;
    
    do
    {
        swapped = 0;
        current = head;
        
        while (current->next != last)
        {
            if (strcmp(current->word, current->next->word) > 0)
            {
                swap_elements(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);
}

// Функция для печати списка
void print_list(struct list *head) {
    struct list *current = head;
    int first = 1;
    
    while (current != NULL)
    {
        if (!first)
        {
            printf(" ");
        }
        printf("%s", current->word);
        first = 0;
        current = current->next;
    }
    printf("\n");
}

// Функция для удаления всего списка
void delete_list(struct list *head) {
    struct list *current = head;
    struct list *next;
    
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

// Функция для чтения строки и разбиения на слова
struct list* read_and_build_list() {
    struct list *head = NULL;
    char input[1000];
    char word[20];
    int word_index = 0;
    int i = 0;
    
    fgets(input, sizeof(input), stdin);
    
    // Обрабатываем строку посимвольно
    while (input[i] != '\0' && input[i] != '.')
    {
        while (input[i] == ' ' && word_index == 0)
        {
            i++;
        }
        
        if (input[i] != ' ' && input[i] != '.' && input[i] != '\n')
         {
            if (word_index < 19)
             {
                word[word_index++] = input[i];
            }
            i++;
        }
        // Если нашли пробел или достигли конца слова
        else if (word_index > 0)
        {
            word[word_index] = '\0';
            head = add_to_list(head, word);
            word_index = 0;
            
            if (input[i] == '.' || input[i] == '\n')
            {
                break;
            }
            i++;
        }
        else
        {
            i++;
        }
    }
    
    // Добавляем последнее слово, если строка заканчивается словом без пробела перед точкой
    if (word_index > 0)
    {
        word[word_index] = '\0';
        head = add_to_list(head, word);
    }
    
    return head;
}

int main() {
    struct list *head = read_and_build_list();
    
    sort_list(head);
    
    print_list(head);
    
    delete_list(head);
    
    return 0;
}
