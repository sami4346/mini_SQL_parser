#include "parser.h"
#include<bits/stdc++.h>
using namespace std;
size_t index = 0;

Token expect(const std::vector<Token>& tokens, TokenType type) {
    if (tokens[index].type != type)
        throw std::runtime_error("Unexpected token: " + tokens[index].value);
    return tokens[index++];
}

// Use smart pointers for ConditionExpression to avoid memory leaks
#include <memory>
std::unique_ptr<ConditionExpression> parseConditionExpression(const std::vector<Token>& tokens, size_t& idx) {
    auto expr = std::make_unique<ConditionExpression>();

    expr->left.value = expect(tokens, TokenType::IDENTIFIER).value;
    expr->left.type = TokenType::IDENTIFIER;

    if (tokens[idx].type == TokenType::EQUAL || tokens[idx].type == TokenType::LESS || tokens[idx].type == TokenType::GREATER) {
        expr->op = expect(tokens, tokens[idx].type).value;
    } else {
        throw std::runtime_error("Expected comparison operator (=, <, >), got: " + tokens[idx].value);
    }

    Token valToken = tokens[idx++];
    expr->right = {valToken.value, valToken.type};

    expr->logicalOp = "";
    expr->next = nullptr;

    if (idx < tokens.size()) {
        if (tokens[idx].type == TokenType::AND || tokens[idx].type == TokenType::OR) {
            expr->logicalOp = tokens[idx].value;
            idx++;
            expr->next = parseConditionExpression(tokens, idx);
        }
    }

    return expr;
}

SelectStatement parseSelect(const std::vector<Token>& tokens, SymbolTable& symTable) {
    index = 0;
    SelectStatement stmt;

    expect(tokens, TokenType::SELECT);

    while (tokens[index].type == TokenType::IDENTIFIER || tokens[index].type == TokenType::STAR) {
        stmt.columns.push_back(tokens[index++].value);
        if (tokens[index].type == TokenType::COMMA) index++;
        else break;
    }

    expect(tokens, TokenType::FROM);
    stmt.table = expect(tokens, TokenType::IDENTIFIER).value;

    // Add table and columns to symbol table
    symTable.addTable(stmt.table);
    for (const auto& col : stmt.columns) {
        symTable.addColumn(stmt.table, col);
    }

    if (tokens[index].type == TokenType::WHERE) {
        ++index;
        stmt.whereClause = parseConditionExpression(tokens, index).release();
    } else {
        stmt.whereClause = nullptr;
    }

    if (tokens[index].type == TokenType::SEMICOLON)
        ++index;

    return stmt;
}

InsertStatement parseInsert(const std::vector<Token>& tokens, SymbolTable& symTable) {
    index = 0;
    InsertStatement stmt;

    expect(tokens, TokenType::INSERT);
    expect(tokens, TokenType::INTO);
    stmt.table = expect(tokens, TokenType::IDENTIFIER).value;

    expect(tokens, TokenType::LPAREN);
    while (tokens[index].type == TokenType::IDENTIFIER) {
        stmt.columns.push_back(tokens[index++].value);
        if (tokens[index].type == TokenType::COMMA) index++;
        else break;
    }
    expect(tokens, TokenType::RPAREN);

    expect(tokens, TokenType::VALUES);

    expect(tokens, TokenType::LPAREN);
    while (tokens[index].type == TokenType::NUMBER || tokens[index].type == TokenType::STRING || tokens[index].type == TokenType::BOOLEAN) {
        stmt.values.push_back({tokens[index].value, tokens[index].type});
        index++;
        if (tokens[index].type == TokenType::COMMA) index++;
        else break;
    }
    expect(tokens, TokenType::RPAREN);

    // Add table and columns to symbol table
    symTable.addTable(stmt.table);
    for (const auto& col : stmt.columns) {
        symTable.addColumn(stmt.table, col);
    }

    if (tokens[index].type == TokenType::SEMICOLON)
        ++index;

    return stmt;
}

UpdateStatement parseUpdate(const std::vector<Token>& tokens, SymbolTable& symTable) {
    index = 0;
    UpdateStatement stmt;

    expect(tokens, TokenType::UPDATE);
    stmt.table = expect(tokens, TokenType::IDENTIFIER).value;

    // Add table to symbol table
    symTable.addTable(stmt.table);

    expect(tokens, TokenType::SET);

    while (tokens[index].type == TokenType::IDENTIFIER) {
        std::string column = tokens[index++].value;
        expect(tokens, TokenType::EQUAL);
        Token valToken = tokens[index++];
        stmt.setClauses.push_back(std::make_pair(column, ValueWithType{valToken.value, valToken.type}));
        // Add column to symbol table
        symTable.addColumn(stmt.table, column);
        if (tokens[index].type == TokenType::COMMA) index++;
        else break;
    }

    if (tokens[index].type == TokenType::WHERE) {
        ++index;
        stmt.whereClause = parseConditionExpression(tokens, index).release();
    } else {
        stmt.whereClause = nullptr;
    }

    if (tokens[index].type == TokenType::SEMICOLON)
        ++index;

    return stmt;
}

DeleteStatement parseDelete(const std::vector<Token>& tokens, SymbolTable& symTable) {
    index = 0;
    DeleteStatement stmt;

    expect(tokens, TokenType::DELETE);
    expect(tokens, TokenType::FROM);
    stmt.table = expect(tokens, TokenType::IDENTIFIER).value;

    // Add table to symbol table
    symTable.addTable(stmt.table);

    if (tokens[index].type == TokenType::WHERE) {
        ++index;
        stmt.whereClause = parseConditionExpression(tokens, index).release();
    } else {
        stmt.whereClause = nullptr;
    }

    if (tokens[index].type == TokenType::SEMICOLON)
        ++index;

    return stmt;
}
