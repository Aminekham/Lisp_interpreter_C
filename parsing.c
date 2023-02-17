#include "lisp.h"
/**
 * the whole parsing process
 */
Token peek(Parser *parser)
{
    // Return the token at the current index if it exists
    if (parser->current_index < parser->ts->size)
    {
        return parser->ts->tokens[parser->current_index];
    }
    // Otherwise, return an EOF token
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    return eof_token;
}
Token next_token(Parser *parser)
{
    parser->current_index += 1;
    return parser->ts->tokens[parser->current_index];
}
lisp *parse_symbol(Parser *parser)
{
    // Get the next token from the parser
    Token token = next_token(parser);

    // Check if the token type is TOKEN_SYMBOL
    // Create a new lisp atom using the symbol value from the token
    lisp *atom = lisp_atom((const atomtype)token.value);

    // Return the atom
    return atom;
}

lisp *parse_list(Parser *parser)
{
    // Consume the opening parenthesis
    next_token(parser);

    lisp *head = NULL;
    lisp *tail = NULL;
    bool done = false;

    while (!done)
    {
        TokenType next_type = peek(parser).type;
        if (next_type == TOKEN_RPAREN)
        {
            done = true;
        }
        else if (next_type == TOKEN_DOT)
        {
            // Consume the dot
            next_token(parser);

            // Parse the cdr expression
            lisp *cdr = parse_expression(parser);
            tail->data.list.cdr = cdr;

            // Consume the closing parenthesis
            next_token(parser);
            done = true;
        }
        else
        {
            // Parse the next expression
            lisp *expression = parse_expression(parser);

            // Add the expression to the list
            if (head == NULL)
            {
                head = lisp_cons(expression, NULL);
                tail = head;
            }
            else
            {
                tail->data.list.cdr = lisp_cons(expression, NULL);
                tail = tail->data.list.cdr;
            }
        }
    }

    // Consume the closing parenthesis
    next_token(parser);

    return head;
}
lisp *parse_integer(Parser *parser)
{
    // Get the current token
    Token token = peek(parser);

    // Make sure the token is an integer
    if (token.type != TOKEN_INTEGER)
    {
        printf("Expected integer, got %d\n", token.type);
        exit(1);
    }

    // Consume the token and move to the next one
    next_token(parser);

    // Return an integer atom with the value of the token
    return lisp_atom(token.value);
}
lisp *parse_quote(Parser *parser)
{
    // Consume the quote token
    next_token(parser);

    // Parse the expression being quoted
    lisp *quoted_expression = parse_expression(parser);

    // Return a list with the quote symbol and the quoted expression
    return lisp_cons(lisp_atom(quoted_expression->data.value), lisp_cons(quoted_expression, NULL));
}

lisp *parse_expression(Parser *parser)
{
        // Peek at the next token
        Token token = peek(parser);
        // Check the type of the token
        switch (token.type)
        {
        // If the token is an integer, parse it as an atomic expression
        case TOKEN_INTEGER:
            return parse_integer(parser);

        // If the token is a symbol, parse it as an atomic expression
        case TOKEN_SYMBOL:
            return parse_symbol(parser);

        // If the token is an open parenthesis, parse it as a list
        case TOKEN_LPAREN:
        {
            return parse_list(parser);
        }

        // If the token is a quote, parse it as a quote expression
        case TOKEN_QUOTE:
            return parse_quote(parser);

        // If none of the above cases match, print an error message and exit
        default:
            exit(1);
        }
}
