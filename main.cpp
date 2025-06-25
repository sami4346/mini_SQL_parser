#include "tokenizer.h"
#include "parser.h"
#include "ast.h"
#include "symbol_table.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <regex>
#include <set>
#include <cctype>

std::string formatWhereClauseString(const std::string& whereClause) {
    static const std::regex wordRegex(R"(\b([A-Za-z0-9_]+)\b)");
    static const std::set<std::string> sqlKeywords = {
        "AND", "OR", "NOT", "IN", "LIKE", "BETWEEN", "IS", "NULL", "TRUE", "FALSE"
    };

    std::string result = whereClause;
    std::smatch match;
    std::string::const_iterator searchStart(result.cbegin());

    while (std::regex_search(searchStart, result.cend(), match, wordRegex)) {
        std::string word = match[1].str();
        std::string wordUpper;
        for (char c : word) wordUpper += std::toupper(static_cast<unsigned char>(c));

        if (sqlKeywords.find(wordUpper) == sqlKeywords.end()) {
            size_t pos = match.position(1) + (searchStart - result.cbegin());
            bool alreadyQuoted = false;
            if (pos > 0 && pos + word.size() < result.size()) {
                if (result[pos - 1] == '\'' && result[pos + word.size()] == '\'') {
                    alreadyQuoted = true;
                }
            }
            if (!alreadyQuoted) {
                result.insert(pos + word.size(), "'");
                result.insert(pos, "'");
                searchStart = result.cbegin() + pos + word.size() + 2;
                continue;
            }
        }
        searchStart = match.suffix().first;
    }
    return result;
}

void printTokens(const std::vector<Token>& tokens) {
    std::cout << "Tokens:\n";
    std::cout << "+----------------+--------------+\n";
    std::cout << "| " << std::left << std::setw(14) << "Token" << " | " << std::setw(12) << "Type" << " |\n";
    std::cout << "+----------------+--------------+\n";
    for (const auto& token : tokens) {
        if (token.value.empty() || token.type == TokenType::END) {
            continue;
        }
        std::cout << "| " << std::left << std::setw(14) << token.value << " | " << std::setw(12) << tokenTypeToString(token.type) << " |\n";
    }
    std::cout << "+----------------+--------------+\n\n";
}

void printSymbolTable(const SymbolTable& symTable) {
    symTable.print();
}

void printConditionExpression(const ConditionExpression* expr) {
    if (!expr) return;
    std::cout << expr->left.value;
    std::cout << " " << expr->op << " ";
    if (expr->right.type == TokenType::STRING) {
        std::cout << "'" << expr->right.value << "'";
    } else {
        std::cout << expr->right.value;
    }
    if (!expr->logicalOp.empty() && expr->next) {
        std::cout << " " << expr->logicalOp << " ";
        printConditionExpression(expr->next);
    }
}

void printParsedUpdateStatement(const UpdateStatement& s) {
    std::cout << "Parsed Statement (AST):\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Type: UPDATE\n";
    std::cout << "Target Table: " << s.table << "\n";
    std::cout << "Set Clauses:\n";
    for (const auto& pair : s.setClauses) {
        std::cout << "  - " << pair.first << " = ";
        if (pair.second.type == TokenType::STRING) {
            std::cout << "'" << pair.second.value << "'";
        } else {
            std::cout << pair.second.value;
        }
        std::cout << "\n";
    }
    if (s.whereClause) {
        std::cout << "WHERE: ";
        printConditionExpression(s.whereClause);
        std::cout << "\n";
    }
    std::cout << "--------------------------------------------\n\n";
}

void printParsedSelectStatement(const SelectStatement& s) {
    std::cout << "Parsed Statement (AST):\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Type: SELECT\n";
    std::cout << "Columns:\n";
    for (const auto& col : s.columns) {
        std::cout << "  - " << col << "\n";
    }
    std::cout << "Table: " << s.table << "\n";
    if (s.whereClause) {
        std::cout << "WHERE: ";
        printConditionExpression(s.whereClause);
        std::cout << "\n";
    }
    std::cout << "--------------------------------------------\n\n";
}

void printParsedInsertStatement(const InsertStatement& s) {
    std::cout << "Parsed Statement (AST):\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Type: INSERT\n";
    std::cout << "Table: " << s.table << "\n";
    std::cout << "Columns:\n";
    for (const auto& col : s.columns) {
        std::cout << "  - " << col << "\n";
    }
    std::cout << "Values:\n";
    for (const auto& val : s.values) {
        if (val.type == TokenType::STRING) {
            std::cout << "  - '" << val.value << "'\n";
        } else {
            std::cout << "  - " << val.value << "\n";
        }
    }
    std::cout << "--------------------------------------------\n\n";
}

int main() {
    std::cout << "============================================\n";
    std::cout << "Please provide SQL Statement:\n";

    std::string line;
    SymbolTable symTable;

    while (true) {
        std::getline(std::cin, line);
        if (line.empty()) break;

        // Trim whitespace from line
        size_t first = line.find_first_not_of(" \t\n\r");
        size_t last = line.find_last_not_of(" \t\n\r");
        if (first == std::string::npos || last == std::string::npos) {
            continue; // skip empty or whitespace-only lines
        }
        std::string trimmed = line.substr(first, last - first + 1);

        std::cout << "============================================\n";
        // Removed duplicate printing of SQL statement here

        try {
            auto tokens = tokenize(trimmed);
            if (tokens.empty()) {
                throw std::runtime_error("Empty input");
            }

            printTokens(tokens);

            if (tokens[0].type == TokenType::UPDATE) {
                UpdateStatement s = parseUpdate(tokens, symTable);
                printParsedUpdateStatement(s);
            } else if (tokens[0].type == TokenType::SELECT) {
                SelectStatement s = parseSelect(tokens, symTable);
                printParsedSelectStatement(s);
            } else if (tokens[0].type == TokenType::INSERT) {
                InsertStatement s = parseInsert(tokens, symTable);
                printParsedInsertStatement(s);
            } else if (tokens[0].type == TokenType::DELETE) {
                DeleteStatement s = parseDelete(tokens, symTable);
                std::cout << "Parsed Statement (AST):\n";
                std::cout << "--------------------------------------------\n";
                std::cout << "Type: DELETE\n";
                std::cout << "Table: " << s.table << "\n";
                if (s.whereClause) {
                    std::cout << "WHERE: ";
                    printConditionExpression(s.whereClause);
                    std::cout << "\n";
                }
                std::cout << "--------------------------------------------\n\n";
            } else {
                // For other statement types, fallback to previous simple output
                std::cout << "Parsed Statement (AST):\n";
                std::cout << "--------------------------------------------\n";
                std::cout << "Type: (Other statement types not yet formatted)\n";
                std::cout << "--------------------------------------------\n\n";
            }

            printSymbolTable(symTable);

        } catch (const std::exception& ex) {
            std::cerr << "Parse error: " << ex.what() << "\n";
        }
    }

    return 0;
}
