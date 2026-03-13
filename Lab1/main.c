#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reuse HW0 structure, keep count to match slide P.49 requirement
typedef struct Node {
    char character;
    int count;
    struct Node *next;
} Node;

int main() {
    // Read its own source code
    FILE *file = fopen("main.c", "r");
    if (file == NULL) {
        printf("Error: Cannot open file.\n");
        return 1;
    }

    Node *head = NULL;
    Node *tail = NULL;
    int ch;

    // Reuse HW0 logic: read characters and filter duplicates
    while ((ch = fgetc(file)) != EOF) {
        Node *current = head;
        int found = 0;

        // Check if the character already exists in the linked-list
        while (current != NULL) {
            if (current->character == (char)ch) {
                current->count++; // If it exists, increment the count
                found = 1;
                break;
            }
            current = current->next;
        }

        // If it is a new character, add a new node to the linked-list
        if (!found) {
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->character = (char)ch;
            newNode->count = 1;
            newNode->next = NULL;

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

    // Output result: print characters from the linked-list based on Lab#1
    Node *current = head;
    while (current != NULL) {
        // Handle special characters for formatting
        if (current->character == '\n') {
            printf("\\n, ");
        } else if (current->character == '\r') {
            printf("\\r, ");
        } else if (current->character == '\t') {
            printf("\\t, ");
        } else if (current->character == ' ') {
            printf("space, ");
        } else {
            // Normal output format, e.g., #, i, n, c, u, 
            printf("%c, ", current->character);
        }
        current = current->next;
    }

    // Free linked-list memory
    current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
