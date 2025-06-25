#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "ast.h"

#include "symbol_table.h"

SelectStatement parseSelect(const std::vector<Token>& tokens, SymbolTable& symTable);
InsertStatement parseInsert(const std::vector<Token>& tokens, SymbolTable& symTable);
UpdateStatement parseUpdate(const std::vector<Token>& tokens, SymbolTable& symTable);
DeleteStatement parseDelete(const std::vector<Token>& tokens, SymbolTable& symTable);


#endif
