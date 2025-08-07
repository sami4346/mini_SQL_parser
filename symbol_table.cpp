#include "symbol_table.h"
#include<bits/stdc++.h>
using namespace std;
// Implementation of SymbolTable methods
void SymbolTable::addTable(const std::string& tableName) {
    if (tableColumns.find(tableName) == tableColumns.end()) {
        tableColumns[tableName] = std::unordered_set<std::string>();
    }
}

void SymbolTable::addColumn(const std::string& tableName, const std::string& columnName) {
    addTable(tableName);
    tableColumns[tableName].insert(columnName);
}

bool SymbolTable::hasTable(const std::string& tableName) const {
    return tableColumns.find(tableName) != tableColumns.end();
}

std::vector<std::string> SymbolTable::getColumns(const std::string& tableName) const {
    std::vector<std::string> columns;
    auto it = tableColumns.find(tableName);
    if (it != tableColumns.end()) {
        for (const auto& col : it->second) {
            columns.push_back(col);
        }
    }
    return columns;
}

void SymbolTable::print(std::ostream& out) const {
    out << "Symbol Table:\n";
    out << "--------------------------------------------\n";
    for (const auto& pair : tableColumns) {
        out << "Table Name: " << pair.first << "\n";
        out << "Columns:\n";
        if (pair.second.size() == 1 && pair.second.count("*") == 1) {
            out << "  ALL (*)\n";
        } else {
            for (const auto& col : pair.second) {
                out << "  - " << col << "\n";
            }
        }
        out << "--------------------------------------------\n";
    }
}
