#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct tree {
    int key;
    struct tree *left, *right;
} tree;

// Вспомогательная структура для очереди
typedef struct queue_item {
    tree *node;
    int hd;
} queue_item;

typedef struct queue {
    queue_item *items;
    int front, rear, capacity;
} queue;

queue* create_queue(int capacity) {
    queue *q = (queue*)malloc(sizeof(queue));
    q->capacity = capacity;
    q->front = q->rear = -1;
    q->items = (queue_item*)malloc(capacity * sizeof(queue_item));
    return q;
}

void enqueue(queue *q, tree *node, int hd) {
    if (q->rear == q->capacity - 1) return;
    if (q->front == -1) q->front = 0;
    q->rear++;
    q->items[q->rear].node = node;
    q->items[q->rear].hd = hd;
}

int is_empty(queue *q) {
    return q->front == -1;
}

queue_item dequeue(queue *q) {
    queue_item item = q->items[q->front];
    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front++;
    }
    return item;
}

void btUpView(tree *root) {
    if (!root) return;

    int map_capacity = 1000;
    int *pos_map = (int*)malloc(map_capacity * sizeof(int));
    for (int i = 0; i < map_capacity; i++) {
        pos_map[i] = INT_MIN;
    }

    int offset = map_capacity / 2;
    queue *q = create_queue(1000);
    enqueue(q, root, 0);

    int min_hd = 0, max_hd = 0;

    while (!is_empty(q)) {
        queue_item current = dequeue(q);
        int hd = current.hd;
        tree *node = current.node;

        if (pos_map[hd + offset] == INT_MIN) {
            pos_map[hd + offset] = node->key;
        }

        if (node->left) {
            enqueue(q, node->left, hd - 1);
            if (hd - 1 < min_hd) min_hd = hd - 1;
        }
        if (node->right) {
            enqueue(q, node->right, hd + 1);
            if (hd + 1 > max_hd) max_hd = hd + 1;
        }
    }

    // Вывод в порядке слева направо
    for (int i = min_hd + offset; i <= max_hd + offset; i++) {
        if (pos_map[i] != INT_MIN) {
            printf("%d ", pos_map[i]);
        }
    }

    free(pos_map);
    free(q->items);
    free(q);
}

// Вспомогательные функции
tree* create_node(int key) {
    tree *node = (tree*)malloc(sizeof(tree));
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

void insert(tree **root, int key) {
    if (*root == NULL) {
        *root = create_node(key);
        return;
    }
    
    if (key < (*root)->key) {
        insert(&((*root)->left), key);
    } else {
        insert(&((*root)->right), key);
    }
}

void free_tree(tree *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main() {
    tree *root = NULL;
    int value;
    
    // Чтение входных данных
    while (scanf("%d", &value) == 1 && value != 0) {
        insert(&root, value);
    }
    
    btUpView(root);
    
    free_tree(root);
    return 0;
}
