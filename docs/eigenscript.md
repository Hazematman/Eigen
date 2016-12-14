Eigen Script
============

This documents out lines the Eigen Script(ES) scripting language. It covers
the tokens in the language as well as the grammar for it as well.

# Tokens

ES has the following tokens:

 - VAR : the keyword "var"
 - FUNC : the keyword "func" used to define function
 - IF : the keyword "if"
 - ELSE : the keyword "else"
 - WHILE : the keyword "while"
 - FOR : the keyword "for"
 - RETURN : the keyword "return"
 - NULL : the keyword "null"
 - TRUE : the keyword "true"
 - FALSE : the keyword "false"
 - BECOMES : the operator "="
 - EQUAL : the operator "=="
 - NEQUAL : the operator "!="
 - LTHAN : the operator "<"
 - LETHAN : the operator "<="
 - GTHAN : the operator ">"
 - GETHAN : the operator ">="
 - PLUS : the operator "+"
 - MINUS : the operator "-"
 - STAR : the operator "*"
 - SLASH : the opeartor "/"
 - PERCENT : the operator "%"
 - AND : the operator "&&"
 - OR : the operator "||"
 - COMMA : the character ","
 - SEMICOLON : the character ";"
 - LPAREN : the character "("
 - RPAREN : the character ")"
 - LBRACE : the character "{"
 - RBRACE : the character "}"
 - LBRACK : the character "["
 - RBRACK : the character "]"
 - ID : any text id that is not a keyword
 - NUMBER : any number that is either an integer or floating point
 - STRING : any text literal inside quotes

# Grammar

The production rules for ES are provided as such. They are in an SLR(1)
parseable format (e represents epsilon):

    S -> BOF lines EOF
    lines -> e
    lines -> function lines
    lines -> statement lines
    function -> FUNC ID LPAREN params RPAREN LBRACE statements RBRACE
    params -> e
    params -> paramlist
    paramlist -> ID
    paramlist -> ID COMMA paramlist
    statements -> e
    statements -> statement statements
    statement -> lvalue BECOMES expr SEMI
    statement -> functioncall SEMI
    statement -> IF LPAREN test RPAREN LBRACE statements RBRACE elseclause
    statement -> WHILE LPAREN tests RPAREN LBRACE statements RBRACE
    statement -> FOR LPAREN lvalue BECOMES expr SEMI tests SEMI lvalue BECOMES expr RPAREN LBRACE statements RBRACE
    elseclause -> e
    elseclause -> ELSE IF LPAREN test RPAREN LBRACE statements RBRACE elseclause
    elseclause -> ELSE LBRACE statements RBRACE
    tests -> test
    tests -> tests AND test
    tests -> tests OR test
    test -> expr EQUAL expr
    test -> expr NEQUAL expr
    test -> expr LTHAN expr
    test -> expr LETHAN expr
    test -> expr GTHAN expr
    test -> expr GETHAN expr
    expr -> term
    expr -> expr PLUS term
    expr -> expr MINUS term
    term -> factor
    term -> term STAR factor
    term -> term SLASH factor
    term -> term PERCENT factor
    factor -> ID
    factor -> NUMBER
    factor -> STIRNG
    factor -> TRUE
    factor -> FALSE
    factor -> LPAREN expr RPAREN
    factor -> ID LBRACK expr RBRACK
    factor -> functioncall
    functioncall -> ID LPAREN RPAREN
    functioncall -> ID LPAREN arglist RPAREN
    arglist -> expr
    arglist -> expr COMMA arglist
    lvalue -> ID
    lvalue -> ID LBRACK expr RBRACK

# Examples

Some examples of ES scripts would be a basic example like so:

    print("Hello World!\n");

to more complex examples like this:

    func add(a,b) {
        return a + b;
    }

    print("1 + 2 = "+add(1,2));

You can also create variables like this:

    var a = 10
    var b = 3

    print("a is "+a+" b is "+b)

You can also use arrays like so:

    var array1 = [] //This is an empty array
    var array2 = [10] //This is an array with 10 elements

    array[0] = 10 // Arrays are zero indexed
    array[1] = "hello" // Arrays can contain any type
