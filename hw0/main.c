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

// 定義 linked-list 的節點結構
typedef struct Node {
    char character;
    int count;
    struct Node *next;
} Node;

int main() {
    // 實作讀取自己本身的程式
    FILE *file = fopen("main.c", "r");
    if (file == NULL) {
        printf("Error: Cannot open file.\n");
        return 1;
    }

    Node *head = NULL;
    Node *tail = NULL;
    int ch;

    // 逐字元讀取檔案
    while ((ch = fgetc(file)) != EOF) {
        Node *current = head;
        int found = 0;

        // 逐次找法：遍歷整個 linked-list 尋找字元，時間複雜度為 O(n)
        while (current != NULL) {
            if (current->character == (char)ch) {
                // 若已存在的字元，則其個數加一
                current->count++;
                found = 1;
                break;
            }
            current = current->next;
        }

        // 若讀到一個新字元（在 list 中沒找到），就新增一個 node 進 link-list 裡
        if (!found) {
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->character = (char)ch;
            newNode->count = 1;
            newNode->next = NULL;

            // 將新 node 串連進 link-list 的尾端
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

    // 巡覽 linked-list 並輸出結果
    Node *current = head;
    while (current != NULL) {
        // 針對特殊字元做排版處理
        if (current->character == '\n') {
            printf("\\n:%d\n", current->count);
        } else if (current->character == ' ') {
            printf("space:%d\n", current->count);
        } else if (current->character == '\r') {
            printf("\\r:%d\n", current->count);
        } else if (current->character == '\t') {
            printf("\\t:%d\n", current->count);
        } else {
            // 正常輸出格式
            printf("%c:%d\n", current->character, current->count);
        }
        current = current->next;
    }

    // 釋放 linked-list 記憶體
    current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
