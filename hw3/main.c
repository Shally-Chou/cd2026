/*
==========================================
HW#3: Recursive-Descent Parser
Description: 使用 HW#2 Scanner 實作 RD Parser
Grammar:
  S  -> E S'
  S' -> ε | + S
  E  -> num | ( S )
Only <stdio.h> & <string.h> allowed!
==========================================
*/
#include <stdio.h>
#include <string.h>

/* ---- Character Check Functions ---- */
int is_space(int c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

/* ---- Token Types ---- */
#define TOKEN_NUM     1
#define TOKEN_PLUS    2
#define TOKEN_LPAREN  3
#define TOKEN_RPAREN  4
#define TOKEN_EOF     5

/* ---- Global Variables ---- */
char input[1000];
int pos = 0;
int token;
char lexeme[256];
int indent_level = 0;  /* 用於輸出縮排 */

/* ---- Scanner (from HW2) ---- */
void nextToken() {
    int len = 0;

    /* 跳過空白 */
    while (is_space(input[pos]))
        pos++;

    /* EOF */
    if (input[pos] == '\0') {
        token = TOKEN_EOF;
        lexeme[0] = '\0';
        return;
    }

    /* 數字 */
    if (is_digit(input[pos])) {
        while (is_digit(input[pos])) {
            lexeme[len++] = input[pos++];
        }
        lexeme[len] = '\0';
        token = TOKEN_NUM;
        return;
    }

    /* 加號 */
    if (input[pos] == '+') {
        pos++;
        token = TOKEN_PLUS;
        return;
    }

    /* 左括號 */
    if (input[pos] == '(') {
        pos++;
        token = TOKEN_LPAREN;
        return;
    }

    /* 右括號 */
    if (input[pos] == ')') {
        pos++;
        token = TOKEN_RPAREN;
        return;
    }
}

/* ---- 輸出輔助函式 ---- */
void print_production(const char* production) {
    int i;
    for (i = 0; i < indent_level; i++) {
        printf(" ");
    }
    printf("%s\n", production);
}

/* ---- Parser Functions ---- */
void parse_S();
void parse_S_prime();
void parse_E();

/* S -> E S' */
void parse_S() {
    print_production("S -> E S'");
    indent_level += 2;
    parse_E();
    parse_S_prime();
    indent_level -= 2;
}

/* S' -> + S | ε */
void parse_S_prime() {
    if (token == TOKEN_PLUS) {
        print_production("S' -> + S");
        nextToken();
        indent_level += 2;
        parse_S();
        indent_level -= 2;
    }
    else {
        /* ε (epsilon) - 必須輸出！ */
        print_production("S' -> epsilon");
    }
}

/* E -> num | ( S ) */
void parse_E() {
    if (token == TOKEN_NUM) {
        print_production(lexeme);
        nextToken();
    }
    else if (token == TOKEN_LPAREN) {
        print_production("E -> ( S )");
        nextToken();
        indent_level += 2;
        parse_S();
        indent_level -= 2;
        if (token == TOKEN_RPAREN) {
            nextToken();
        }
    }
}

/* ---- Main ---- */
int main() {
    /* 讀取輸入 */
    if (fgets(input, sizeof(input), stdin) != NULL) {
        /* 移除換行符 */
        int len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
    }

    /* 初始化並開始解析 */
    pos = 0;
    indent_level = 0;
    nextToken();
    parse_S();

    return 0;
}
