#include <stdio.h>
#include <string.h>

/* -------- Token types -------- */
#define TOKEN_NUM    0
#define TOKEN_PLUS   1
#define TOKEN_LPAREN 2
#define TOKEN_RPAREN 3
#define TOKEN_EOF    4

static int token;    /* current lookahead token */
static int num_val;  /* value when token == TOKEN_NUM */
static int err = 0;  /* error flag */

/* -------- Scanner (HW#2) -------- */
static int next_token(void) {
    int c;
    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') continue;
        if (c >= '0' && c <= '9') {
            num_val = c - '0';
            while ((c = getchar()) != EOF && c >= '0' && c <= '9')
                num_val = num_val * 10 + (c - '0');
            if (c != EOF) ungetc(c, stdin);
            return TOKEN_NUM;
        }
        if (c == '+') return TOKEN_PLUS;
        if (c == '(') return TOKEN_LPAREN;
        if (c == ')') return TOKEN_RPAREN;
    }
    return TOKEN_EOF;
}

/* -------- Helpers -------- */
static void indent(int d) {
    int i;
    for (i = 0; i < d * 4; i++) putchar(' ');
}

/* -------- Forward declarations -------- */
void parse_S(int d);
void parse_Sp(int d);
void parse_E(int d);

/* S -> E S' */
void parse_S(int d) {
    if (err) return;
    if (token == TOKEN_NUM || token == TOKEN_LPAREN) {
        indent(d); printf("S -> E S'\n");
        parse_E(d + 1);
        parse_Sp(d);
    } else {
        printf("parse error\n");
        err = 1;
    }
}

/* S' -> e | +S */
void parse_Sp(int d) {
    if (err) return;
    if (token == TOKEN_PLUS) {
        indent(d); printf("S' -> + S\n");
        token = next_token();
        parse_S(d + 1);
    } else if (token == TOKEN_RPAREN || token == TOKEN_EOF) {
        indent(d); printf("S' -> e\n");
    } else {
        printf("parse error\n");
        err = 1;
    }
}

/* E -> num | (S) */
void parse_E(int d) {
    if (err) return;
    if (token == TOKEN_NUM) {
        indent(d); printf("%d\n", num_val);
        token = next_token();
    } else if (token == TOKEN_LPAREN) {
        indent(d); printf("E -> ( S )\n");
        token = next_token();
        parse_S(d + 1);
        if (err) return;
        if (token != TOKEN_RPAREN) {
            printf("parse error\n");
            err = 1;
            return;
        }
        token = next_token();
    } else {
        printf("parse error\n");
        err = 1;
    }
}

int main(void) {
    token = next_token();
    parse_S(0);
    return 0;
}
