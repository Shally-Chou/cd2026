/*
 * ==========================================
 * HW#0: Basic Requirement
 * Time Complexity: O(n)
 * Description: Using sequential search in linked-list.
 * ==========================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// def linked-list struct
typedef struct Node {
    char character;
    int count;
    struct Node *next;
} Node;

int main() {
    FILE *file = fopen("main.c", "r");
    if (file == NULL) {
        printf("Error: Cannot open file.\n");
        return 1;
    }

    Node *head = NULL;
    Node *tail = NULL;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        Node *current = head;
        int found = 0;

        while (current != NULL) {
            if (current->character == (char)ch) {
                current->count++;
                found = 1;
                break;
            }
            current = current->next;
        }

        if (!found) {
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->character = (char)ch;
            newNode->count = 1;
            newNode->next = NULL;

            // new node put into the end of link-list 
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }

    fclose(file);

    Node *current = head;
    while (current != NULL) {
        if (current->character == '\n') {
            printf("\\n:%d\n", current->count);
        } else if (current->character == ' ') {
            printf("space:%d\n", current->count);
        } else if (current->character == '\r') {
            printf("\\r:%d\n", current->count);
        } else if (current->character == '\t') {
            printf("\\t:%d\n", current->count);
        } else {
            printf("%c:%d\n", current->character, current->count);
        }
        current = current->next;
    }

    current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
