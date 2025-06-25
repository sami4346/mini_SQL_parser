#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

enum class TokenType {
    SELECT, FROM, WHERE,
    INSERT, INTO, VALUES,
    UPDATE, SET,
    DELETE,
    OR,
    AND,
    IDENTIFIER, COMMA, SEMICOLON,
    LPAREN, RPAREN,
    OPERATOR, NUMBER, STRING,
    STAR, EQUAL, GREATER, LESS,
    LESS_EQUAL, GREATER_EQUAL, NOT_EQUAL,
    BOOLEAN,
    UNKNOWN, END
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input);

std::string tokenTypeToString(TokenType type);

#endif
