/*
 * ==========================================
 * HW#1: The Crap-C Scanner
 * Time Complexity: O(n)
 * Description: Lexical Analyzer (Scanner) based on DFA.
 * Modified from HW#0 (Bonus version).
 * Note: Only allowed headers are used (<stdio.h>, <stdlib.h>, <string.h>).
 * No <ctype.h> — custom char-check functions are used instead.
 * Reads input explicitly from "scan.c".
 * ==========================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- 因規定不能 include <ctype.h>，自行實作字元判斷函式 (DFA 輔助) ---- */

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
    /* 1. 打開測資檔案 scan.c */
    FILE *file = fopen("scan.c", "r");
    if (file == NULL) {
        printf("Error: Cannot open file 'scan.c'. Please make sure it exists.\n");
        return 1;
    }

    /*
     * 延續 HW#0 lookup table 架構：
     * 使用 char_map[256] 作為 O(1) 查找表，用來追蹤已見過的字元。
     */
    int char_map[256];
    memset(char_map, 0, sizeof(char_map)); 

    int in_char, c;
    char buffer[256];
    int len;

    /* ---- DFA 掃描主迴圈：改由 fgetc(file) 讀取字元直到 EOF ---- */
    while ((in_char = fgetc(file)) != EOF) {

        /* 記錄此字元已出現 */
        char_map[(unsigned char)in_char]++;

        /* --------------------------------------------------
         * State 0: 起始狀態
         * -------------------------------------------------- */

        /* 1. 忽略空白字元 */
        if (is_space(in_char)) {
            continue;
        }

        /* --------------------------------------------------
         * State 1: ID / Keyword 狀態
         * -------------------------------------------------- */
        else if (is_alpha(in_char) || in_char == '_') {
            len = 0;
            buffer[len++] = (char)in_char;

            /* 繼續吃字元，直到不是合法 ID 字元 */
            for (c = fgetc(file); is_alnum(c) || c == '_'; c = fgetc(file)) {
                if (len < 255) buffer[len++] = (char)c;
                char_map[(unsigned char)c]++;
            }
            buffer[len] = '\0';

            /* 多讀了一個不合法字元，退回 file */
            if (c != EOF) ungetc(c, file);

            /* 判定 Keyword 或 Identifier 並輸出 */
            print_id_or_keyword(buffer);
        }

        /* --------------------------------------------------
         * State 2: Integer Literal 狀態
         * -------------------------------------------------- */
        else if (is_digit(in_char)) {
            len = 0;
            buffer[len++] = (char)in_char;

            /* 繼續吃數字 */
            while (is_digit((c = fgetc(file)))) {
                if (len < 255) buffer[len++] = (char)c;
                char_map[(unsigned char)c]++;
            }
            buffer[len] = '\0';

            /* 多讀了非數字字元，退回 file */
            if (c != EOF) ungetc(c, file);

            printf("%s: LITERAL_TOKEN\n", buffer);
        }

        /* --------------------------------------------------
         * State 3: Operators / Symbols 狀態
         * -------------------------------------------------- */
        else {
            switch (in_char) {
                case '(':
                    printf("(: LEFTPAREN_TOKEN\n");
                    break;
                case ')':
                    printf("): REFTPAREN_TOKEN\n");
                    break;
                case '{':
                    printf("{: LEFTBRACE_TOKEN\n");
                    break;
                case '}':
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
                    c = fgetc(file);
                    if (c == '=') {
                        printf("==: EQUAL_TOKEN\n");
                    } else {
                        if (c != EOF) ungetc(c, file);
                        printf("=: ASSIGN_TOKEN\n");
                    }
                    break;

                /* >= 或 > */
                case '>':
                    c = fgetc(file);
                    if (c == '=') {
                        printf(">=: GREATEREQUAL_TOKEN\n");
                    } else {
                        if (c != EOF) ungetc(c, file);
                        printf(">: GREATER_TOKEN\n");
                    }
                    break;

                /* <= 或 < */
                case '<':
                    c = fgetc(file);
                    if (c == '=') {
                        printf("<=: LESSEQUAL_TOKEN\n");
                    } else {
                        if (c != EOF) ungetc(c, file);
                        printf("<: LESS_TOKEN\n");
                    }
                    break;

                default:
                    break;
            }
        }
    }

    /* 關閉檔案 */
    fclose(file);
    return 0;
}
