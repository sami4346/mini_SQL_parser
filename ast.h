#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <utility>
#include "tokenizer.h"

struct ValueWithType {
    std::string value;
    TokenType type;
};

struct ConditionExpression {
    ValueWithType left;
    std::string op;
    ValueWithType right;
    std::string logicalOp; // "AND", "OR", or empty
    ConditionExpression* next;
};

struct SelectStatement {
    std::vector<std::string> columns;
    std::string table;
    ConditionExpression* whereClause; // pointer to compound where clause expression
};

struct InsertStatement {
    std::string table;
    std::vector<std::string> columns;
    std::vector<ValueWithType> values;
};

struct UpdateStatement {
    std::string table;
    std::vector<std::pair<std::string, ValueWithType>> setClauses; // column, value with type
    ConditionExpression* whereClause; // pointer to compound where clause expression
};

struct DeleteStatement {
    std::string table;
    ConditionExpression* whereClause; // pointer to compound where clause expression
};

#endif
