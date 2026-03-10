/*
 * ==========================================
 * HW#0: Bonus Requirement
 * Time Complexity: O(1)
 * Description: Using pointer array as a lookup table.
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
    // read code open "main1.c"
    FILE *file = fopen("main1.c", "r");
    if (file == NULL) {
        printf("Error: Cannot open file.\n");
        return 1;
    }

    Node *head = NULL;
    Node *tail = NULL;

    // O(n) decrease to O(1)
    Node *char_map[256] = {NULL};

    int ch;

    while ((ch = fgetc(file)) != EOF) {
        // read a new char
        if (char_map[ch] == NULL) {
            // add a node
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->character = (char)ch;
            newNode->count = 1;
            newNode->next = NULL;

            // new node put into link-list 
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }

            char_map[ch] = newNode;
        } else {
            // char exist num+1
            char_map[ch]->count++;
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

    // release linked-list mem
    current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
