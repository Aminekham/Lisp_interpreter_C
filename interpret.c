#include "lisp.h"
/**
 * interpreting the giving lisp code
 */
lisp *lisp_eval_arguments(Interpreter *interpreter, lisp *arguments)
{
    lisp *current_arg = arguments;
    lisp *evaluated_args = NULL;
    while (current_arg != NULL)
    {
        lisp *evaluated_arg = interpret(interpreter, lisp_car(current_arg));
        evaluated_args = lisp_cons(evaluated_arg, evaluated_args);
        current_arg = lisp_cdr(current_arg);
    }
    return evaluated_args;
}
lisp *lisp_apply(lisp *function, lisp *evaluated_arguments)
{
    if (lisp_isatomic(function))
    {
        atomtype func_val = lisp_getval(function);
        if (func_val == '+')
        {
            int result = 0;
            while (!lisp_isatomic(evaluated_arguments))
            {
                result += lisp_getval(lisp_car(evaluated_arguments));
                evaluated_arguments = lisp_cdr(evaluated_arguments);
            }
            return lisp_atom(result);
        }
        else if (func_val == '-')
        {
            int result = lisp_getval(lisp_car(evaluated_arguments));
            evaluated_arguments = lisp_cdr(evaluated_arguments);
            while (!lisp_isatomic(evaluated_arguments))
            {
                result -= lisp_getval(lisp_car(evaluated_arguments));
                evaluated_arguments = lisp_cdr(evaluated_arguments);
            }
            return lisp_atom(result);
        }
        else if (func_val == '*')
        {
            int result = 1;
            while (!lisp_isatomic(evaluated_arguments))
            {
                result *= lisp_getval(lisp_car(evaluated_arguments));
                evaluated_arguments = lisp_cdr(evaluated_arguments);
            }
            return lisp_atom(result);
        }
        else if (func_val == '/')
        {
            int result = lisp_getval(lisp_car(evaluated_arguments));
            evaluated_arguments = lisp_cdr(evaluated_arguments);
            while (!lisp_isatomic(evaluated_arguments))
            {
                result /= lisp_getval(lisp_car(evaluated_arguments));
                evaluated_arguments = lisp_cdr(evaluated_arguments);
            }
            return lisp_atom(result);
        }
    }
    // Function not found or not a built-in function
    return NULL;
}

lisp *interpret(Interpreter *interpreter, lisp *expression)
{
    if (lisp_isatomic(expression))
    {
        return expression;
    }

    lisp *function = lisp_car(expression);
    function = interpret(interpreter, function);
    lisp *arguments = lisp_cdr(expression);
    lisp *evaluated_arguments = lisp_eval_arguments(interpreter, arguments);
    return lisp_apply(function, evaluated_arguments);
}
