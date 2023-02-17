#include "lisp.h"
/**
 * A lisp language interpreter using c
 */
// Returns element 'a' - this is not a list, and
// by itself would be printed as e.g. "3", and not "(3)"
lisp *lisp_atom(const atomtype a)
{
    lisp *atom = ncalloc(1, sizeof(lisp));
    atom->is_atomic = true;
    atom->data.value = a;
    return (atom);
}
// Returns a list containing the car as 'l1'
// and the cdr as 'l2'- data in 'l1' and 'l2' are reused,
// and not copied. Either 'l1' and/or 'l2' can be NULL
lisp *lisp_cons(const lisp *l1, const lisp *l2)
{
    lisp *cons = ncalloc(1, sizeof(lisp));
    cons->is_atomic = false;
    cons->data.list.car = (lisp *)l1;
    cons->data.list.cdr = (lisp *)l2;
    return (cons);
}

// Returns the car (1st) component of the list 'l'.
// Does not copy any data.
lisp *lisp_car(const lisp *l)
{
    return (l->data.list.car);
}
// Returns the cdr (all but the 1st) component of the list '
// Does not copy any data.
lisp* lisp_cdr(const lisp* l)
{
    if (l == NULL) {
        return NULL;
    }
    else {
        return l->data.list.cdr;
    }
}


// Returns the data/value stored in the cons 'l'
atomtype lisp_getval(const lisp *l)
{
    assert(lisp_isatomic(l));
    return l->data.value;
}

// Returns a boolean depending up whether l points to an atom (not a list)
bool lisp_isatomic(const lisp *l)
{
    if (l == NULL)
        return false;
    return (l->is_atomic);
}

// Returns a deep copy of the list 'l'
lisp *lisp_copy(const lisp *l)
{
    lisp *copy = ncalloc(1, sizeof(lisp));
    copy->is_atomic = l->is_atomic;
    if (lisp_isatomic(l))
    {
        copy->data.value = l->data.value;
    }
    else
    {
        copy->data.list.car = lisp_copy(l->data.list.car);
        copy->data.list.cdr = lisp_copy(l->data.list.cdr);
    }
    return copy;
}
int lisp_length(const lisp *l)
{
    int length = 0;
    while (!lisp_isatomic(l))
    {
        length++;
        l = lisp_cdr(l);
    }
    return (length);
}
void lisp_tostring(const lisp *l, char *str)
{
    if (lisp_isatomic(l))
    {
        sprintf(str, "%d", lisp_getval(l));
    }
    else
    {
        char temp_str[32];
        str[0] = '(';
        int str_index = 1;
        const lisp *current_element = l;
        while (current_element != NULL)
        {
            lisp_tostring(lisp_car(current_element), temp_str);
            strcat(str + str_index, temp_str);
            str_index += strlen(temp_str);
            if (lisp_cdr(current_element) != NULL)
            {
                str[str_index++] = ' ';
            }
            current_element = lisp_cdr(current_element);
        }
        str[str_index++] = ')';
        str[str_index] = '\0';
    }
}
void lisp_free(lisp **l)
{
    if (!lisp_isatomic(*l))
    {
        lisp_free(&((*l)->data.list.car));
        lisp_free(&((*l)->data.list.cdr));
    }
    free(*l);
    *l = NULL;
}
lisp *lisp_list(const int n, ...)
{
    va_list args;
    va_start(args, n);

    lisp *result = NULL;
    for (int i = n - 1; i >= 0; i--)
    {
        result = lisp_cons(va_arg(args, lisp *), result);
    }

    va_end(args);
    return (result);
}
void lisp_reduce(void (*func)(lisp *l, atomtype *n), lisp *l, atomtype *acc)
{
    if (l == NULL)
    {
        return;
    }
    if (lisp_isatomic(l))
    {
        func(l, acc);
    }
    else
    {
        lisp_reduce(func, lisp_car(l), acc);
        lisp_reduce(func, lisp_cdr(l), acc);
    }
}
int main() {
  while (true) {
    // Read input from the user
    char input[1024];
    fgets(input, 1024, stdin);

    // Tokenize the input
    TokenStream* ts = tokenize(input);
    // Create a parser and parse the token stream
    Parser* parser = malloc(sizeof(Parser));
    parser->ts = ts;
    parser->current_index = 0;
    lisp* expression = parse_expression(parser);
    // Create an interpreter and interpret the parsed expression
    Interpreter* interpreter = malloc(sizeof(Interpreter));
    interpreter->symbol_table = NULL;
    interpreter->expression_stack = NULL;
    lisp* result = interpret(interpreter, expression);
    // Print the result
    char str[64];
    lisp_tostring(result, str);
    printf("%s\n", str);

    // Clean up
    lisp_free(&expression);
    lisp_free(&result);
    free(interpreter);
    free(parser);
    free(ts->tokens);
    free(ts);
  }

  return 0;
}

