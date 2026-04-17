/*
 * ==========================================
 * HW#1: The Crap-C Scanner
 * Description: Lexical Analyzer (Scanner) based on DFA.
 * Modified from HW#0 (Bonus version).
 * No <ctype.h> — custom char-check functions are used instead.
 * * [How to Compile & Run (Windows CMD)]
 * 1. Ensure the test file (e.g., 'scan.c') is in the same directory.
 * 2. Compile: gcc main.c -o scanner.exe
 * 3. Run:     scanner < scan.c
 * ==========================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- 不能 include <ctype.h>，實作字元判斷函式 (DFA 輔助) ---- */

int is_space(int c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

int is_alpha(int c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

int is_alnum(int c) {
    return (is_alpha(c) || is_digit(c));
}

/* ---- Token 辨識與輸出 ---- */

/* 判斷是否為 Keyword，是則輸出對應 Token，否則輸出 ID_TOKEN */
void print_id_or_keyword(const char *buf) {
    if      (strcmp(buf, "int")   == 0) printf("int: TYPE_TOKEN\n");
    else if (strcmp(buf, "main")  == 0) printf("main: MAIN_TOKEN\n");
    else if (strcmp(buf, "if")    == 0) printf("if: IF_TOKEN\n");
    else if (strcmp(buf, "else")  == 0) printf("else: ELSE_TOKEN\n");
    else if (strcmp(buf, "while") == 0) printf("while: WHILE_TOKEN\n");
    else                                printf("%s: ID_TOKEN\n", buf);
}

/* ---- Main (DFA Scanner Loop) ---- */

int main() {

    /*
     * 延續 HW#0 (main1.c) 的 lookup table 架構：
     * 使用 char_map[256] 作為 O(1) 查找表，用來追蹤已見過的字元。
     * HW#1 主要任務改為 Lexical Analysis，但保留此結構以符合
     * 「基於 HW#0 修改」的作業要求。
     */
    int char_map[256];
    memset(char_map, 0, sizeof(char_map));  /* 初始化 lookup table */

    int in_char, c;
    char buffer[256];
    int len;

    /* ---- DFA 掃描主迴圈：持續讀取字元直到 EOF ---- */
    while ((in_char = getchar()) != EOF) {

        /* 記錄此字元已出現（延續 HW#0 lookup table 概念） */
        char_map[(unsigned char)in_char]++;

        /* --------------------------------------------------
         * State 0: 起始狀態
         * 根據第一個字元決定進入哪個 DFA 分支
         * -------------------------------------------------- */

        /* 1. 忽略空白字元 */
        if (is_space(in_char)) {
            continue;
        }

        /* --------------------------------------------------
         * State 1: ID / Keyword 狀態
         * 進入條件：字母或底線開頭
         * ID  ::= LETTER (LETTER | DIGIT | '_')*
         * -------------------------------------------------- */
        else if (is_alpha(in_char) || in_char == '_') {
            len = 0;
            buffer[len++] = (char)in_char;

            /* 繼續吃字元，直到不是合法 ID 字元 */
            for (c = getchar(); is_alnum(c) || c == '_'; c = getchar()) {
                if (len < 255) buffer[len++] = (char)c;
                char_map[(unsigned char)c]++;   /* 同步更新 lookup table */
            }
            buffer[len] = '\0';

            /* 多讀了一個不合法字元，退回 stdin（DFA 退回動作） */
            if (c != EOF) ungetc(c, stdin);

            /* 判定 Keyword 或 Identifier 並輸出 */
            print_id_or_keyword(buffer);
        }

        /* --------------------------------------------------
         * State 2: Integer Literal 狀態
         * 進入條件：數字開頭
         * INTLITERAL ::= DIGIT {DIGIT}
         * -------------------------------------------------- */
        else if (is_digit(in_char)) {
            len = 0;
            buffer[len++] = (char)in_char;

            /* 繼續吃數字 */
            while (is_digit((c = getchar()))) {
                if (len < 255) buffer[len++] = (char)c;
                char_map[(unsigned char)c]++;   /* 同步更新 lookup table */
            }
            buffer[len] = '\0';

            /* 多讀了非數字字元，退回 stdin */
            if (c != EOF) ungetc(c, stdin);

            printf("%s: LITERAL_TOKEN\n", buffer);
        }

        /* --------------------------------------------------
         * State 3: Operators / Symbols 狀態
         * 進入條件：非空白、非字母、非數字
         * -------------------------------------------------- */
        else {
            switch (in_char) {
                case '(':
                    printf("(: LEFTPAREN_TOKEN\n");
                    break;
                case ')':
                    /* 嚴格遵守投影片拼法：REFT (非 RIGHT) */
                    printf("): REFTPAREN_TOKEN\n");
                    break;
                case '{':
                    printf("{: LEFTBRACE_TOKEN\n");
                    break;
                case '}':
                    /* 嚴格遵守投影片拼法：REFT (非 RIGHT) */
                    printf("}: REFTBRACE_TOKEN\n");
                    break;
                case ';':
                    printf(";: SEMICOLON_TOKEN\n");
                    break;
                case '+':
                    printf("+: PLUS_TOKEN\n");
                    break;
                case '-':
                    printf("-: MINUS_TOKEN\n");
                    break;

                /* == 或 = */
                case '=':
                    c = getchar();
                    if (c == '=') {
                        printf("==: EQUAL_TOKEN\n");
                    } else {
                        if (c != EOF) ungetc(c, stdin);
                        printf("=: ASSIGN_TOKEN\n");
                    }
                    break;

                /* >= 或 > */
                case '>':
                    c = getchar();
                    if (c == '=') {
                        printf(">=: GREATEREQUAL_TOKEN\n");
                    } else {
                        if (c != EOF) ungetc(c, stdin);
                        printf(">: GREATER_TOKEN\n");
                    }
                    break;

                /* <= 或 < */
                case '<':
                    c = getchar();
                    if (c == '=') {
                        printf("<=: LESSEQUAL_TOKEN\n");
                    } else {
                        if (c != EOF) ungetc(c, stdin);
                        printf("<: LESS_TOKEN\n");
                    }
                    break;

                /* 其他不在 token 列表內的字元：靜默忽略 */
                default:
                    break;
            }
        }
    }

    return 0;
}
