#include "tokenizer.h"
#include "parser.h"
#include "ast.h"
#include "symbol_table.h"
#include<bits/stdc++.h>
using namespace std;

// Forward declaration to fix compilation error
void printConditionExpressionTree(const ConditionExpression* expr, int indent = 0);
#include "tokenizer.h"
#include "parser.h"
#include "ast.h"
#include "symbol_table.h"


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

// Enhanced symbol table printing with more details
void printSymbolTable(const SymbolTable& symTable, std::ostream& out = std::cout) {
    out << "Symbol Table (Detailed):\n";
    out << "--------------------------------------------\n";
    symTable.print(out);
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
        printConditionExpression(expr->next.get());
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
    }    if (s.whereClause) {
        std::cout << "WHERE (Tree):\n";
        printConditionExpressionTree(s.whereClause, 4);
        std::cout << "\nWHERE (Flat): ";
        printConditionExpression(s.whereClause);
        std::cout << "\n";
    }
    std::cout << "--------------------------------------------\n\n";
}
void printConditionExpressionTree(const ConditionExpression* expr, int indent) {
    if (!expr) return;
    std::string ind(indent, ' ');
    std::cout << ind << "Condition: " << expr->left.value << " " << expr->op << " ";
    if (expr->right.type == TokenType::STRING) {
        std::cout << "'" << expr->right.value << "'";
    } else {
        std::cout << expr->right.value;
    }
    std::cout << "\n";
    if (!expr->logicalOp.empty() && expr->next) {
        std::cout << ind << "LogicalOp: " << expr->logicalOp << "\n";
        printConditionExpressionTree(expr->next.get(), indent + 4);
    }
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

// Helper function to trim whitespace from a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return str.substr(first, last - first + 1);
}

int main() {
    std::cout << "============================================\n";
    std::cout << "How many SQL statements do you want to execute? ";
    int numStatements = 0;
    std::cin >> numStatements;
    std::cin.ignore(); // clear newline

    SymbolTable symTable;
    std::string line;
    static std::string lastOutput;

    for (int i = 0; i < numStatements; ++i) {
        std::cout << "Please provide SQL Statement (or type EXPORT to save symbol table):\n";
        std::getline(std::cin, line);
        if (line.empty()) {
            --i; // don't count empty input
            continue;
        }
        std::string trimmed = trim(line);
        if (trimmed.empty()) {
            --i;
            continue;
        }
        if (trimmed == "EXPORT") {
            std::cout << "Enter filename to export output: ";
            std::string filename;
            std::getline(std::cin, filename);
            if (filename.empty()) {
                std::cout << "No filename provided. Export cancelled.\n";
                --i;
                continue;
            }
            std::ofstream out(filename);
            if (!out.is_open()) {
                std::cout << "Failed to open file for writing.\n";
                --i;
                continue;
            }
            out << lastOutput;
            out.close();
            std::cout << "Output exported to " << filename << "\n";
            --i;
            continue;
        }
        std::stringstream outputBuffer;
        outputBuffer << "============================================\n";
        try {
            const auto tokens = tokenize(trimmed);
            if (tokens.empty()) {
                throw std::runtime_error("Empty input");
            }
            // Print tokens
            outputBuffer << "Tokens:\n";
            outputBuffer << "+----------------+--------------+\n";
            outputBuffer << "| " << std::left << std::setw(14) << "Token" << " | " << std::setw(12) << "Type" << " |\n";
            outputBuffer << "+----------------+--------------+\n";
            for (const auto& token : tokens) {
                if (token.value.empty() || token.type == TokenType::END) continue;
                outputBuffer << "| " << std::left << std::setw(14) << token.value << " | " << std::setw(12) << tokenTypeToString(token.type) << " |\n";
            }
            outputBuffer << "+----------------+--------------+\n\n";

            // Print parsed statement
            switch (tokens[0].type) {
                case TokenType::UPDATE: {
                    UpdateStatement s = parseUpdate(tokens, symTable);
                    outputBuffer << "Parsed Statement (AST):\n";
                    outputBuffer << "--------------------------------------------\n";
                    outputBuffer << "Type: UPDATE\n";
                    outputBuffer << "Target Table: " << s.table << "\n";
                    outputBuffer << "Set Clauses:\n";
                    for (const auto& pair : s.setClauses) {
                        outputBuffer << "  - " << pair.first << " = ";
                        if (pair.second.type == TokenType::STRING) {
                            outputBuffer << "'" << pair.second.value << "'";
                        } else {
                            outputBuffer << pair.second.value;
                        }
                        outputBuffer << "\n";
                    }
                    if (s.whereClause) {
                        // WHERE (Flat)
                        outputBuffer << "WHERE: ";
                        std::function<void(const ConditionExpression*)> printCond;
                        printCond = [&](const ConditionExpression* expr) {
                            if (!expr) return;
                            outputBuffer << expr->left.value << " " << expr->op << " ";
                            if (expr->right.type == TokenType::STRING) {
                                outputBuffer << "'" << expr->right.value << "'";
                            } else {
                                outputBuffer << expr->right.value;
                            }
                            if (!expr->logicalOp.empty() && expr->next) {
                                outputBuffer << " " << expr->logicalOp << " ";
                                printCond(expr->next.get());
                            }
                        };
                        printCond(s.whereClause);
                        outputBuffer << "\n";
                        // WHERE (Tree)
                        outputBuffer << "WHERE (Tree):\n";
                        std::function<void(const ConditionExpression*, int)> printTree;
                        printTree = [&](const ConditionExpression* expr, int indent) {
                            if (!expr) return;
                            std::string ind(indent, ' ');
                            outputBuffer << ind << "Condition: " << expr->left.value << " " << expr->op << " ";
                            if (expr->right.type == TokenType::STRING) {
                                outputBuffer << "'" << expr->right.value << "'";
                            } else {
                                outputBuffer << expr->right.value;
                            }
                            outputBuffer << "\n";
                            if (!expr->logicalOp.empty() && expr->next) {
                                outputBuffer << ind << "LogicalOp: " << expr->logicalOp << "\n";
                                printTree(expr->next.get(), indent + 4);
                            }
                        };
                        printTree(s.whereClause, 4);
                        // WHERE (Flat) again
                        outputBuffer << "\nWHERE (Flat): ";
                        printCond(s.whereClause);
                        outputBuffer << "\n";
                        // Add referenced columns to symbol table
                        std::function<void(const ConditionExpression*)> addColumns;
                        addColumns = [&](const ConditionExpression* expr) {
                            if (!expr) return;
                            symTable.addColumn(s.table, expr->left.value);
                            if (!expr->logicalOp.empty() && expr->next) {
                                addColumns(expr->next.get());
                            }
                        };
                        addColumns(s.whereClause);
                    }
                    outputBuffer << "--------------------------------------------\n\n";
                    break;
                }
                case TokenType::SELECT: {
                    SelectStatement s = parseSelect(tokens, symTable);
                    outputBuffer << "Parsed Statement (AST):\n";
                    outputBuffer << "--------------------------------------------\n";
                    outputBuffer << "Type: SELECT\n";
                    outputBuffer << "Columns:\n";
                    for (const auto& col : s.columns) {
                        outputBuffer << "  - " << col << "\n";
                    }
                    outputBuffer << "Table: " << s.table << "\n";
                    if (s.whereClause) {
                        // WHERE (Flat)
                        outputBuffer << "WHERE: ";
                        std::function<void(const ConditionExpression*)> printCond;
                        printCond = [&](const ConditionExpression* expr) {
                            if (!expr) return;
                            outputBuffer << expr->left.value << " " << expr->op << " ";
                            if (expr->right.type == TokenType::STRING) {
                                outputBuffer << "'" << expr->right.value << "'";
                            } else {
                                outputBuffer << expr->right.value;
                            }
                            if (!expr->logicalOp.empty() && expr->next) {
                                outputBuffer << " " << expr->logicalOp << " ";
                                printCond(expr->next.get());
                            }
                        };
                        printCond(s.whereClause);
                        outputBuffer << "\n";
                        // WHERE (Tree)
                        outputBuffer << "WHERE (Tree):\n";
                        std::function<void(const ConditionExpression*, int)> printTree;
                        printTree = [&](const ConditionExpression* expr, int indent) {
                            if (!expr) return;
                            std::string ind(indent, ' ');
                            outputBuffer << ind << "Condition: " << expr->left.value << " " << expr->op << " ";
                            if (expr->right.type == TokenType::STRING) {
                                outputBuffer << "'" << expr->right.value << "'";
                            } else {
                                outputBuffer << expr->right.value;
                            }
                            outputBuffer << "\n";
                            if (!expr->logicalOp.empty() && expr->next) {
                                outputBuffer << ind << "LogicalOp: " << expr->logicalOp << "\n";
                                printTree(expr->next.get(), indent + 4);
                            }
                        };
                        printTree(s.whereClause, 4);
                        // WHERE (Flat) again
                        outputBuffer << "\nWHERE (Flat): ";
                        printCond(s.whereClause);
                        outputBuffer << "\n";
                    }
                    outputBuffer << "--------------------------------------------\n\n";
                    break;
                }
                case TokenType::INSERT: {
                    InsertStatement s = parseInsert(tokens, symTable);
                    outputBuffer << "Parsed Statement (AST):\n";
                    outputBuffer << "--------------------------------------------\n";
                    outputBuffer << "Type: INSERT\n";
                    outputBuffer << "Table: " << s.table << "\n";
                    outputBuffer << "Columns:\n";
                    for (const auto& col : s.columns) {
                        outputBuffer << "  - " << col << "\n";
                    }
                    outputBuffer << "Values:\n";
                    for (const auto& val : s.values) {
                        if (val.type == TokenType::STRING) {
                            outputBuffer << "  - '" << val.value << "'\n";
                        } else {
                            outputBuffer << "  - " << val.value << "\n";
                        }
                    }
                    outputBuffer << "--------------------------------------------\n\n";
                    break;
                }
                case TokenType::DELETE: {
                    DeleteStatement s = parseDelete(tokens, symTable);
                    outputBuffer << "Parsed Statement (AST):\n";
                    outputBuffer << "--------------------------------------------\n";
                    outputBuffer << "Type: DELETE\n";
                    outputBuffer << "Table: " << s.table << "\n";
                    if (s.whereClause) {
                        // WHERE (Flat)
                        outputBuffer << "WHERE: ";
                        std::function<void(const ConditionExpression*)> printCond;
                        printCond = [&](const ConditionExpression* expr) {
                            if (!expr) return;
                            outputBuffer << expr->left.value << " " << expr->op << " ";
                            if (expr->right.type == TokenType::STRING) {
                                outputBuffer << "'" << expr->right.value << "'";
                            } else {
                                outputBuffer << expr->right.value;
                            }
                            if (!expr->logicalOp.empty() && expr->next) {
                                outputBuffer << " " << expr->logicalOp << " ";
                                printCond(expr->next.get());
                            }
                        };
                        printCond(s.whereClause);
                        outputBuffer << "\n";
                        // WHERE (Tree)
                        outputBuffer << "WHERE (Tree):\n";
                        std::function<void(const ConditionExpression*, int)> printTree;
                        printTree = [&](const ConditionExpression* expr, int indent) {
                            if (!expr) return;
                            std::string ind(indent, ' ');
                            outputBuffer << ind << "Condition: " << expr->left.value << " " << expr->op << " ";
                            if (expr->right.type == TokenType::STRING) {
                                outputBuffer << "'" << expr->right.value << "'";
                            } else {
                                outputBuffer << expr->right.value;
                            }
                            outputBuffer << "\n";
                            if (!expr->logicalOp.empty() && expr->next) {
                                outputBuffer << ind << "LogicalOp: " << expr->logicalOp << "\n";
                                printTree(expr->next.get(), indent + 4);
                            }
                        };
                        printTree(s.whereClause, 4);
                        // WHERE (Flat) again
                        outputBuffer << "\nWHERE (Flat): ";
                        printCond(s.whereClause);
                        outputBuffer << "\n";
                        // Add referenced columns to symbol table
                        std::function<void(const ConditionExpression*)> addColumns;
                        addColumns = [&](const ConditionExpression* expr) {
                            if (!expr) return;
                            symTable.addColumn(s.table, expr->left.value);
                            if (!expr->logicalOp.empty() && expr->next) {
                                addColumns(expr->next.get());
                            }
                        };
                        addColumns(s.whereClause);
                    }
                    outputBuffer << "--------------------------------------------\n\n";
                    break;
                }
                default:
                    outputBuffer << "Parsed Statement (AST):\n";
                    outputBuffer << "--------------------------------------------\n";
                    outputBuffer << "Type: (Other statement types not yet formatted)\n";
                    outputBuffer << "--------------------------------------------\n\n";
                    break;
            }
            // Print symbol table (single header, blank line before, no duplicates)
            outputBuffer << "\nSymbol Table:\n";
            outputBuffer << "--------------------------------------------\n";
            for (const auto& pair : symTable.getTableColumns()) {
                outputBuffer << "Table Name: " << pair.first << "\n";
                outputBuffer << "Columns:\n";
                if (pair.second.empty()) {
                    outputBuffer << "  No columns found\n";
                } else if (pair.second.size() == 1 && pair.second.count("*") == 1) {
                    outputBuffer << "  ALL (*)\n";
                } else {
                    std::set<std::string> printed;
                    for (const auto& col : pair.second) {
                        if (printed.insert(col).second) {
                            outputBuffer << "  - " << col << "\n";
                        }
                    }
                }
                outputBuffer << "--------------------------------------------\n";
            }
        } catch (const std::exception& ex) {
            outputBuffer << "Parse error: " << ex.what() << "\n";
        }
        lastOutput = outputBuffer.str();
        std::cout << lastOutput;
    }

    // After all statements, prompt for export or exit
    std::cout << "Press Enter to exit or type EXPORT to export the last output: ";
    std::string finalInput;
    std::getline(std::cin, finalInput);
    if (finalInput == "EXPORT") {
        std::cout << "Enter filename to export output: ";
        std::string filename;
        std::getline(std::cin, filename);
        if (filename.empty()) {
            std::cout << "No filename provided. Export cancelled.\n";
        } else {
            std::ofstream out(filename);
            if (!out.is_open()) {
                std::cout << "Failed to open file for writing.\n";
            } else {
                out << lastOutput;
                out.close();
                std::cout << "Output exported to " << filename << "\n";
            }
        }
    }
    return 0;
}
