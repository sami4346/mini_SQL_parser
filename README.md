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

- **Boolean Type Support**:  
  Recognizes and parses boolean literals (`true`, `false`).

- **Symbol Table**:  
  Maintains information about tables and their columns for semantic validation. Now shows detailed output and supports export to file.

- **Validation**:  
  Ensures referenced tables and columns exist.

- **Interactive Mode**:  
  Accepts multiple SQL queries from the command line. Supports the `EXPORT` command to save the latest output to a file.

- **WHERE Clause Output**:  
  Displays both tree and flat representations of complex WHERE conditions for better understanding.

- **Output**:
  - Token list with types
  - AST structure
  - Symbol table (detailed)
  - Validation results or error messages
  - Exportable output

---

## 🧾 Sample Usage

```sql
CREATE TABLE students (id, name, department);
INSERT INTO students VALUES (1, 'Alice', 'CSE');
SELECT name FROM students WHERE department = 'CSE';
