#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

class SymbolTable {
public:
    // Add a table to the symbol table
    void addTable(const std::string& tableName);

    // Add a column to a table in the symbol table
    void addColumn(const std::string& tableName, const std::string& columnName);

    // Check if a table exists
    bool hasTable(const std::string& tableName) const;

    // Get columns of a table
    std::vector<std::string> getColumns(const std::string& tableName) const;

    // Print the symbol table contents
    void print() const;

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> tableColumns;
};

#endif
