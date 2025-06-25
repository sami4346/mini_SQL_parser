#include "symbol_table.h"

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

void SymbolTable::print() const {
    std::cout << "Symbol Table:\n";
    std::cout << "--------------------------------------------\n";
    for (const auto& pair : tableColumns) {
        std::cout << "Table Name: " << pair.first << "\n";
        std::cout << "Columns:\n";
        if (pair.second.size() == 1 && pair.second.count("*") == 1) {
            std::cout << "  ALL (*)\n";
        } else {
            for (const auto& col : pair.second) {
                std::cout << "  - " << col << "\n";
            }
        }
        std::cout << "--------------------------------------------\n";
    }
}
