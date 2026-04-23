#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* 處理並辨識 Token 的主函數 */
void scan_tokens(FILE *file)
{
    int c;

    /* 每次讀取一個字元，直到檔案結束 */
    while ((c = fgetc(file)) != EOF)
    {

        /* 1. 略過所有空白字元 (包含空白、換行、Tab) */
        if (isspace(c))
        {
            continue;
        }

        /* 2. 判斷是否為「識別字 (Identifier)」或「關鍵字 (Keyword)」 */
        if (isalpha(c) || c == '_')
        {
            char buffer[256];
            int len = 0;
            buffer[len++] = (char)c;

            while ((c = fgetc(file)) != EOF && (isalnum(c) || c == '_'))
            {
                buffer[len++] = (char)c;
            }
            buffer[len] = '\0';

            /* 將多讀的那個字元退回給檔案串流 */
            ungetc(c, file);

            /* 比對關鍵字 */
            if (strcmp(buffer, "int") == 0)
            {
                printf("%s: TYPE_TOKEN\n", buffer);
            }
            else if (strcmp(buffer, "main") == 0)
            {
                printf("%s: MAIN_TOKEN\n", buffer);
            }
            else if (strcmp(buffer, "if") == 0)
            {
                printf("%s: IF_TOKEN\n", buffer);
            }
            else if (strcmp(buffer, "else") == 0)
            {
                printf("%s: ELSE_TOKEN\n", buffer);
            }
            else if (strcmp(buffer, "while") == 0)
            {
                printf("%s: WHILE_TOKEN\n", buffer);
            }
            else
            {
                printf("%s: ID_TOKEN\n", buffer);
            }
            continue;
        }

        /* 3. 判斷是否為「數字 (Number)」 */
        if (isdigit(c))
        {
            char buffer[256];
            int len = 0;
            buffer[len++] = (char)c;

            while ((c = fgetc(file)) != EOF && isdigit(c))
            {
                buffer[len++] = (char)c;
            }
            buffer[len] = '\0';

            ungetc(c, file);
            printf("%s: NUM_TOKEN\n", buffer);
            continue;
        }

        /* 4. 判斷符號與運算子 */
        if (c == '=')
        {
            int next_c = fgetc(file);
            if (next_c == '=')
            {
                printf("==: EQUAL_TOKEN\n");
            }
            else
            {
                ungetc(next_c, file);
                printf("=: ASSIGN_TOKEN\n");
            }
        }
        else if (c == '>')
        {
            int next_c = fgetc(file);
            if (next_c == '=')
            {
                printf(">=: GREATEREQUAL_TOKEN\n");
            }
            else
            {
                ungetc(next_c, file);
                printf(">: GREATER_TOKEN\n");
            }
        }
        else if (c == '(')
        {
            printf("(: LEFTPAREN_TOKEN\n");
        }
        else if (c == ')')
        {
            printf("): RIGHTPAREN_TOKEN\n");
        }
        else if (c == '{')
        {
            printf("{: LEFTBRACE_TOKEN\n");
        }
        else if (c == '}')
        {
            printf("}: RIGHTBRACE_TOKEN\n");
        }
        else if (c == '+')
        {
            printf("+: PLUS_TOKEN\n");
        }
        else if (c == ';')
        {
            printf(";: SEMICOLON_TOKEN\n");
        }
    }
}

int main()
{
    FILE *file;

    /* 打開你要測試的程式碼檔案 */
    file = fopen("test.txt", "r");
    if (file == NULL)
    {
        printf("Cannot open file.\n");
        return 1;
    }

    scan_tokens(file);

    fclose(file);
    return 0;
}