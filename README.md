# 🧠 Mini SQL Parser and Validator

A lightweight educational project that demonstrates how basic SQL statements are parsed and semantically validated using compiler design principles. It simulates the front-end of an SQL engine, supporting tokenization, AST generation, and symbol table management.

---

## 🚀 Features

- **Tokenization**:  
  Recognizes SQL keywords, identifiers, literals (numbers, strings, booleans), operators, and punctuation.

- **Parsing**:  
  Builds Abstract Syntax Trees (ASTs) for:
  - `SELECT`
  - `INSERT INTO`
  - `UPDATE`
  - `DELETE`

- **Condition Parsing**:  
  Supports logical expressions using `AND`, `OR`.

- **Symbol Table**:  
  Maintains information about tables and their columns for semantic validation.

- **Validation**:  
  Ensures referenced tables and columns exist.

- **Interactive Mode**:  
  Accepts multiple SQL queries from the command line until a blank line is entered.

- **Output**:
  - Token list with types
  - AST structure
  - Symbol table
  - Validation results or error messages

---

## 🧾 Sample Usage

```sql
CREATE TABLE students (id, name, department);
INSERT INTO students VALUES (1, 'Alice', 'CSE');
SELECT name FROM students WHERE department = 'CSE';
