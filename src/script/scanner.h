#ifndef SCANNER_H
#define SCANNER_H

enum TokenType {
    TOKEN_VAR,
    TOKEN_FUNC,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_RETURN,
    TOKEN_NULL,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_BECOMES,
    TOKEN_EQUAL,
    TOKEN_NEQUAL,
    TOKEN_LTHAN,
    TOKEN_LETHAN,
    TOKEN_GTHAN,
    TOKEN_GETHAN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACK,
    TOKEN_RBRACK,
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_STRING,
};

struct Token {
    enum TokenType type;
    char *str;
};

struct Token tokenCreate(enum TokenType type, char *str);

void scannerInit();

#endif