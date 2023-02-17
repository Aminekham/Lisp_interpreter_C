#include "lisp.h"
/**
 * making tokens out of the user input
 */
TokenStream *tokenize(const char *input)
{
    TokenStream *ts = malloc(sizeof(TokenStream));
    ts->size = 0;
    ts->tokens = malloc(sizeof(Token));

    while (*input)
    {
        while (isspace(*input))
        {
            input++;
        }
        if (*input == '\0')
        {
            break;
        }

        if (*input == '(')
        {
            ts->tokens[ts->size].type = TOKEN_LPAREN;
            input++;
            ts->size++;
            continue;
        }

        if (*input == ')')
        {
            ts->tokens[ts->size].type = TOKEN_RPAREN;
            input++;
            ts->size++;
            continue;
        }

        if (*input == '.')
        {
            ts->tokens[ts->size].type = TOKEN_DOT;
            input++;
            ts->size++;
            continue;
        }

        if (*input == '\'')
        {
            ts->tokens[ts->size].type = TOKEN_QUOTE;
            input++;
            ts->size++;
            continue;
        }

        if (isdigit(*input))
        {
            int value = 0;
            while (isdigit(*input))
            {
                value = (value * 10) + (*input - '0');
                input++;
            }
            ts->tokens[ts->size].type = TOKEN_INTEGER;
            ts->tokens[ts->size].value = value;
            ts->size++;
            continue;
        }

        if (isalpha(*input) || *input == '+' || *input == '-' || *input == '*' || *input == '/')
        {
            char symbol[128];
            int length = 0;
            while (isalpha(*input) || isdigit(*input) || *input == '+' || *input == '-' || *input == '*' || *input == '/')
            {
                symbol[length] = *input;
                length++;
                input++;
                symbol[length] = '\0';
                ts->tokens[ts->size].type = TOKEN_SYMBOL;
                ts->tokens[ts->size].symbol = strdup(symbol);
                ts->size++;
                continue;
            }
        }
        else
        {
            fprintf(stderr, "Error: Unknown token '%c'\n", *input);
            exit(1);
        }
    }
    ts->tokens[ts->size].type = TOKEN_EOF;
    ts->size++;
    return ts;
}
