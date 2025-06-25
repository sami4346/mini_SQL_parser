#include "tokenizer.h"
#include <cctype>
#include <sstream>

std::string toLower(const std::string& s) {
    std::string out = s;
    for (char& c : out) c = std::tolower(c);
    return out;
}

TokenType keywordToToken(const std::string& word) {
    std::string lw = toLower(word);
    if (lw == "select") return TokenType::SELECT;
    if (lw == "from") return TokenType::FROM;
    if (lw == "where") return TokenType::WHERE;
    if (lw == "insert") return TokenType::INSERT;
    if (lw == "into") return TokenType::INTO;
    if (lw == "values") return TokenType::VALUES;
    if (lw == "update") return TokenType::UPDATE;
    if (lw == "set") return TokenType::SET;
    if (lw == "delete") return TokenType::DELETE;
    if (lw == "or") return TokenType::OR;
    if (lw == "and") return TokenType::AND;
    if (lw == "true" || lw == "false") return TokenType::BOOLEAN;
    return TokenType::IDENTIFIER;
}

#include <iostream>

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::SELECT: return "SELECT";
        case TokenType::FROM: return "FROM";
        case TokenType::WHERE: return "WHERE";
        case TokenType::INSERT: return "INSERT";
        case TokenType::INTO: return "INTO";
        case TokenType::VALUES: return "VALUES";
        case TokenType::UPDATE: return "UPDATE";
        case TokenType::SET: return "SET";
        case TokenType::DELETE: return "DELETE";
        case TokenType::OR: return "OR";
        case TokenType::AND: return "AND";
        case TokenType::BOOLEAN: return "BOOLEAN";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::COMMA: return "COMMA";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::OPERATOR: return "OPERATOR";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";
        case TokenType::STAR: return "STAR";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::UNKNOWN: return "UNKNOWN";
        case TokenType::END: return "END";
        default: return "UNKNOWN";
    }
}

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t i = 0;
    while (i < input.length()) {
        char c = input[i];
        if (isspace(c)) {
            ++i;
            continue;
        }

        if (std::isalpha(c)) {
            std::string word;
            while (i < input.length() && (std::isalnum(input[i]) || input[i] == '_'))
                word += input[i++];
            TokenType type = keywordToToken(word);
            tokens.push_back({ type, word });
        } else if (std::isdigit(c)) {
            std::string number;
            bool dotEncountered = false;
            while (i < input.length() && (std::isdigit(input[i]) || (input[i] == '.' && !dotEncountered))) {
                if (input[i] == '.') {
                    dotEncountered = true;
                }
                number += input[i++];
            }
            tokens.push_back({ TokenType::NUMBER, number });
        } else if (c == '\'') {
            std::string str;
            ++i; // skip opening quote
            while (i < input.length() && input[i] != '\'') {
                str += input[i++];
            }
            ++i; // skip closing quote if present
            tokens.push_back({ TokenType::STRING, str });
        } else if (c == ',') {
            tokens.push_back({ TokenType::COMMA, "," }); ++i;
        } else if (c == '*') {
            tokens.push_back({ TokenType::STAR, "*" }); ++i;
        } else if (c == ';') {
            tokens.push_back({ TokenType::SEMICOLON, ";" }); ++i;
        } else if (c == '>') {
            if (i + 1 < input.length() && input[i + 1] == '=') {
                tokens.push_back({ TokenType::GREATER_EQUAL, ">=" });
                i += 2;
            } else {
                tokens.push_back({ TokenType::GREATER, ">" });
                ++i;
            }
        } else if (c == '<') {
            if (i + 1 < input.length() && input[i + 1] == '=') {
                tokens.push_back({ TokenType::LESS_EQUAL, "<=" });
                i += 2;
            } else {
                tokens.push_back({ TokenType::LESS, "<" });
                ++i;
            }
        } else if (c == '!') {
            if (i + 1 < input.length() && input[i + 1] == '=') {
                tokens.push_back({ TokenType::NOT_EQUAL, "!=" });
                i += 2;
            } else {
                tokens.push_back({ TokenType::UNKNOWN, std::string(1, c) });
                ++i;
            }
        } else if (c == '=') {
            tokens.push_back({ TokenType::EQUAL, "=" }); ++i;
        } else if (c == '(') {
            tokens.push_back({ TokenType::LPAREN, "(" }); ++i;
        } else if (c == ')') {
            tokens.push_back({ TokenType::RPAREN, ")" }); ++i;
        } else {
            tokens.push_back({ TokenType::UNKNOWN, std::string(1, c) }); ++i;
        }
    }
    tokens.push_back({ TokenType::END, "" });
    return tokens;
}
