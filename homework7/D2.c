#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
    int key;
    struct tree *left, *right;
    struct tree *parent; 
} tree;

// Вспомогательная функция для создания нового узла
tree* createNode(int key, tree* parent) {
    tree* newNode = (tree*)malloc(sizeof(tree));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = parent;
    return newNode;
}

// Функция поиска брата
tree* findBrother(tree *root, int key) {
    if (root == NULL) {
        return NULL;
    }
    
    tree *target = NULL;
    
    tree *stack[100];
    int top = 0;
    
    stack[top++] = root;
    
    while (top > 0) {
        tree *node = stack[--top];
        
        if (node->key == key) {
            target = node;
            break;
        }
        
        if (node->right != NULL) {
            stack[top++] = node->right;
        }
        
        if (node->left != NULL) {
            stack[top++] = node->left;
        }
    }
    
    if (target == NULL) {
        return NULL;
    }
    
    if (target->parent == NULL) {
        return NULL;
    }
    
    tree *parent = target->parent;
    
    if (parent->left == target) 
        return parent->right; 
     else 
        return parent->left; 
    
}

int main() {  
    tree *root = createNode(10, NULL);
    
    root->left = createNode(5, root);
    root->right = createNode(15, root);
    
    root->left->left = createNode(3, root->left);
    root->left->right = createNode(7, root->left);
    root->right->right = createNode(20, root->right);
    
    root->left->left->right = createNode(4, root->left->left);
    root->right->right->left = createNode(18, root->right->right);
    
    // Тестируем функцию
    int test_keys[] = {3, 4, 5, 7, 10, 15, 18, 20, 25};
    int num_tests = sizeof(test_keys) / sizeof(test_keys[0]);
    
    for (int i = 0; i < num_tests; i++)
    {
        int key = test_keys[i];
        tree *brother = findBrother(root, key);
        
        printf("Ключ: %d -> ", key);

        printf("Брат: %d\n", brother->key);
        
    }
    
    free(root->left->left->right);
    free(root->left->left);
    free(root->left->right);
    free(root->left);
    free(root->right->right->left);
    free(root->right->right);
    free(root->right);
    free(root);
    
    return 0;
}
