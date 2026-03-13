#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// def linked-list struct
typedef struct Node {
    char character;
    struct Node *next;
} Node;

int main() {
    // read itself
    FILE *file = fopen("main.c", "r");
    if (file == NULL) {
        printf("Error: Cannot open file.\n");
        return 1;
    }

    Node *head = NULL;
    Node *tail = NULL;
    int ch;

    // read Character Stream
    while ((ch = fgetc(file)) != EOF) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->character = (char)ch;
        newNode->next = NULL;

        // new node put in the end of link-list
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    fclose(file);

    Node *current = head;
    while (current != NULL) {
        // special char transmit
        if (current->character == '\n') {
            printf("\\n, ");
        } else if (current->character == '\r') {
            printf("\\r, ");
        } else if (current->character == '\t') {
            printf("\\t, ");
        } else if (current->character == ' ') {
            printf("space, ");
        } else {
            printf("%c, ", current->character);
        }
        current = current->next;
    }

    // release linked-list mem
    current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
