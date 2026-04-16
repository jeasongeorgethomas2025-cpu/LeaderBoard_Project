#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int id;
    int score;
    int height;
    int size;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct HNode {
    int id;
    Node* ptr;
    struct HNode* next;
} HNode;

#define HSIZE 1009

HNode* table[HSIZE];
Node* root = NULL;

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int getHeight(Node* node) {
    if (node == NULL) {
        return 0;
    } else {
        return node->height;
    }
}

int getSize(Node* node) {
    if (node == NULL) {
        return 0;
    } else {
        return node->size;
    }
}

Node* createNode(int id, int score) {
    Node* newnode = (Node*)malloc(sizeof(Node));

    newnode->id = id;
    newnode->score = score;

    newnode->height = 1;
    newnode->size = 1;

    newnode->left = NULL;
    newnode->right = NULL;

    return newnode;
}

int getBalanceFactor(Node* node) {
    if (node == NULL) {
        return 0;
    } else {
        return getHeight(node->left) - getHeight(node->right);
    }
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = getHeight(y->left);
    if (getHeight(y->right) > y->height) {
        y->height = getHeight(y->right);
    }
    y->height = y->height + 1;

    x->height = getHeight(x->left);
    if (getHeight(x->right) > x->height) {
        x->height = getHeight(x->right);
    }
    x->height = x->height + 1;

    y->size = getSize(y->left) + getSize(y->right) + 1;
    x->size = getSize(x->left) + getSize(x->right) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = getHeight(x->left);
    if (getHeight(x->right) > x->height) {
        x->height = getHeight(x->right);
    }
    x->height = x->height + 1;

    y->height = getHeight(y->left);
    if (getHeight(y->right) > y->height) {
        y->height = getHeight(y->right);
    }
    y->height = y->height + 1;

    x->size = getSize(x->left) + getSize(x->right) + 1;
    y->size = getSize(y->left) + getSize(y->right) + 1;

    return y;
}

Node* insertAVL(Node* node, int id, int score) {
    if (node == NULL) {
        return createNode(id, score);
    }

    if (score < node->score) {
        node->left = insertAVL(node->left, id, score);
    } else {
        node->right = insertAVL(node->right, id, score);
    }

    node->height = getHeight(node->left);
    if (getHeight(node->right) > node->height) {
        node->height = getHeight(node->right);
    }
    node->height = node->height + 1;

    node->size = getSize(node->left) + getSize(node->right) + 1;

    int balance = getBalanceFactor(node);

    if (balance > 1) {
        if (score < node->left->score) {
            return rightRotate(node);
        } else {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }

    if (balance < -1) {
        if (score >= node->right->score) {
            return leftRotate(node);
        } else {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }

    return node;
}

int hashFunction(int id) {
    return id % HSIZE;
}

void insertHash(int id, Node* ptr) {
    int index = hashFunction(id);

    HNode* newnode = (HNode*)malloc(sizeof(HNode));

    newnode->id = id;
    newnode->ptr = ptr;

    newnode->next = table[index];

    table[index] = newnode;
}

Node* getFromHash(int id) {
    int index = hashFunction(id);

    HNode* temp = table[index];

    while (temp != NULL) {
        if (temp->id == id) {
            return temp->ptr;
        }
        temp = temp->next;
    }

    return NULL;
}

void addPlayer(int id, int score) {
    root = insertAVL(root, id, score);
    insertHash(id, createNode(id, score));
}

void updateScore(int id, int newScore) {
    Node* node = getFromHash(id);

    if (node == NULL) {
        printf("Player not found\n");
        return;
    }

    node->score = newScore;

    root = insertAVL(root, id, newScore);

    printf("Score updated\n");
}

void inorder(Node* node) {
    if (node == NULL) {
        return;
    }

    inorder(node->right);
    printf("ID: %d Score: %d\n", node->id, node->score);
    inorder(node->left);
}

void topK(Node* node, int* k) {
    if (node == NULL) {
        return;
    }

    if (*k <= 0) {
        return;
    }

    topK(node->right, k);

    if (*k > 0) {
        printf("ID: %d Score: %d\n", node->id, node->score);
        (*k)--;
    }

    topK(node->left, k);
}

int main() {
    int op;
    int id;
    int score;
    int k;

    while (1) {
        printf("\n1. Add Player\n");
        printf("2. Update Score\n");
        printf("3. Show Leaderboard\n");
        printf("4. Top K Players\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &op);

        if (op == 1) {
            printf("Enter id: ");
            scanf("%d", &id);
            printf("Enter score: ");
            scanf("%d", &score);

            addPlayer(id, score);

            printf("Player added\n");
        }

        else if (op == 2) {
            printf("Enter id: ");
            scanf("%d", &id);
            printf("Enter new score: ");
            scanf("%d", &score);

            updateScore(id, score);
        }

        else if (op == 3) {
            printf("Leaderboard:\n");
            inorder(root);
        }

        else if (op == 4) {
            printf("Enter K: ");
            scanf("%d", &k);

            printf("Top %d players:\n", k);
            topK(root, &k);
        }

        else if (op == 5) {
            break;
        }

        else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}