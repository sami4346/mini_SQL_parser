
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

/**
 * @brief Token types for SQL parsing.
 */
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

/**
 * @brief Represents a token with type and value.
 */
struct Token {
    TokenType type;
    std::string value;
};

/**
 * @brief Tokenizes the input SQL string.
 * @param input SQL statement as string.
 * @return Vector of tokens.
 */
std::vector<Token> tokenize(const std::string& input);

/**
 * @brief Converts TokenType to string for display.
 * @param type TokenType enum value.
 * @return String representation of token type.
 */
std::string tokenTypeToString(TokenType type);

#endif
