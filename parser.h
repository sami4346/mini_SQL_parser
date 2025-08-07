
#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "ast.h"
#include "symbol_table.h"

/**
 * @brief Parses a SELECT statement from tokens.
 * @param tokens Vector of tokens.
 * @param symTable Symbol table reference.
 * @return Parsed SelectStatement.
 */
SelectStatement parseSelect(const std::vector<Token>& tokens, SymbolTable& symTable);

/**
 * @brief Parses an INSERT statement from tokens.
 * @param tokens Vector of tokens.
 * @param symTable Symbol table reference.
 * @return Parsed InsertStatement.
 */
InsertStatement parseInsert(const std::vector<Token>& tokens, SymbolTable& symTable);

/**
 * @brief Parses an UPDATE statement from tokens.
 * @param tokens Vector of tokens.
 * @param symTable Symbol table reference.
 * @return Parsed UpdateStatement.
 */
UpdateStatement parseUpdate(const std::vector<Token>& tokens, SymbolTable& symTable);

/**
 * @brief Parses a DELETE statement from tokens.
 * @param tokens Vector of tokens.
 * @param symTable Symbol table reference.
 * @return Parsed DeleteStatement.
 */
DeleteStatement parseDelete(const std::vector<Token>& tokens, SymbolTable& symTable);

#endif
