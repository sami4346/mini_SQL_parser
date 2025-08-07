
#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <utility>
#include "tokenizer.h"
#include <memory>

/**
 * @brief Represents a value with its token type.
 */
struct ValueWithType {
    std::string value;
    TokenType type;
};

/**
 * @brief Represents a condition expression (used in WHERE clauses).
 */
struct ConditionExpression {
    ValueWithType left;
    std::string op;
    ValueWithType right;
    std::string logicalOp; // "AND", "OR", or empty
    std::unique_ptr<ConditionExpression> next;
};

/**
 * @brief Represents a parsed SELECT statement.
 */
struct SelectStatement {
    std::vector<std::string> columns;
    std::string table;
    ConditionExpression* whereClause; // pointer to compound where clause expression
};

/**
 * @brief Represents a parsed INSERT statement.
 */
struct InsertStatement {
    std::string table;
    std::vector<std::string> columns;
    std::vector<ValueWithType> values;
};

/**
 * @brief Represents a parsed UPDATE statement.
 */
struct UpdateStatement {
    std::string table;
    std::vector<std::pair<std::string, ValueWithType>> setClauses; // column, value with type
    ConditionExpression* whereClause; // pointer to compound where clause expression
};

/**
 * @brief Represents a parsed DELETE statement.
 */
struct DeleteStatement {
    std::string table;
    ConditionExpression* whereClause; // pointer to compound where clause expression
};

#endif
