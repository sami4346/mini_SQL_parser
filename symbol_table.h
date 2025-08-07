
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

/**
 * @brief SymbolTable manages tables and columns for SQL parsing.
 */
class SymbolTable {
public:
    /**
     * @brief Add a table to the symbol table.
     * @param tableName Name of the table.
     */
    void addTable(const std::string& tableName);

    /**
     * @brief Add a column to a table in the symbol table.
     * @param tableName Name of the table.
     * @param columnName Name of the column.
     */
    void addColumn(const std::string& tableName, const std::string& columnName);

    /**
     * @brief Check if a table exists.
     * @param tableName Name of the table.
     * @return True if table exists, false otherwise.
     */
    bool hasTable(const std::string& tableName) const;

    /**
     * @brief Get columns of a table.
     * @param tableName Name of the table.
     * @return Vector of column names.
     */
    std::vector<std::string> getColumns(const std::string& tableName) const;

    /**
     * @brief Print the symbol table contents.
     */
    void print(std::ostream& out = std::cout) const;
    const std::unordered_map<std::string, std::unordered_set<std::string>>& getTableColumns() const { return tableColumns; }

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> tableColumns;
};

#endif
