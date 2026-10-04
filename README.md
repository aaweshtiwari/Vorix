# VORIX

VORIX is a lightweight file-based database engine written in C.

It is a personal systems-programming project built to explore how basic
database functionality can be implemented from scratch using C, file
handling, structures, data validation, schema management, record
management, and a command-line interface.

---

## Features

- Create databases using `.vrx` files
- List available databases
- Access an active database
- Create tables with custom columns
- Define column data types
- Store table schemas separately
- Insert records into tables
- INTEGER input validation
- Duplicate ID detection
- Retrieve stored records
- Display records in formatted CLI tables
- Persistent file-based storage
- Interactive command-line interface
- Case-insensitive commands

---

## Commands

| Command | Description |
|---|---|
| `BUILD` | Create a new database |
| `INSPECT` | List available databases |
| `ACCESS` | Select an existing database |
| `TABLE` | Create a table and define its columns |
| `DESCRIBE` | Display a table's schema |
| `APPEND` | Add a new record |
| `RETRIEVE` | Display stored records |
| `TERMINATE` | Shut down VORIX |

Commands can be entered in uppercase or lowercase.

---

## How VORIX Works

VORIX currently uses the local file system for persistent storage.

Instead of using an external database server, the current implementation
stores database information, table schemas, and table records in separate
files.

This approach makes VORIX a simple file-based database engine and allows
the project to demonstrate the basic concepts involved in database
storage and management.

---

## Database Storage

When a database is created, VORIX creates a `.vrx` file.

Example:

```text
school.vrx
```

The `.vrx` file represents the database and is used by VORIX to identify
and access the database.

---

## Table Schema Storage

Each table has a separate schema file.

The schema stores information about:

- Table name
- Number of columns
- Column names
- Column data types

Example:

```text
school.vrx_students.tbl
```

The schema file contains information in a structured text format.

Example:

```text
TABLE|students
COLUMNS|3
COLUMN|id|INTEGER
COLUMN|name|TEXT
COLUMN|age|INTEGER
```

This allows VORIX to load the structure of a table before performing
operations such as inserting or retrieving records.

---

## Record Storage

Table records are stored separately from the table schema.

Example:

```text
school.vrx_students.data
```

Records are stored using the `|` character as a field separator.

Example:

```text
1|Aawesh|20
2|Rahul|21
3|Aman|19
```

This separation allows VORIX to keep table structure and table data
independent from each other.

---

## Example Workflow

### 1. Start VORIX

```text
VORIX Database Engine
If you want to exit, type TERMINATE.

VORIX>
```

---

### 2. Create a Database

Use the `BUILD` command:

```text
VORIX> BUILD
Enter database name: school
Database 'school' created successfully.
```

This creates:

```text
school.vrx
```

---

### 3. Inspect Available Databases

Use the `INSPECT` command:

```text
VORIX> INSPECT

AVAILABLE DATABASES
-------------------
school.vrx
```

This command searches for available `.vrx` database files.

---

### 4. Access a Database

Use the `ACCESS` command:

```text
VORIX> ACCESS
Enter database name: school
Database 'school' is now active.
```

Once the database is active, table-related commands can operate on it.

---

### 5. Create a Table

Use the `TABLE` command:

```text
VORIX> TABLE
Enter table name: students
Enter number of columns: 3

Column 1 name: id
Column 1 type: INTEGER

Column 2 name: name
Column 2 type: TEXT

Column 3 name: age
Column 3 type: INTEGER

Table 'students' created successfully.
```

VORIX then creates a table schema file:

```text
school.vrx_students.tbl
```

---

### 6. Describe a Table

Use the `DESCRIBE` command:

```text
VORIX> DESCRIBE
Enter table name: students

TABLE: students
-----------------------------
COLUMNS: 3

1. id              INTEGER
2. name            TEXT
3. age             INTEGER
```

The `DESCRIBE` command loads the stored schema and displays the table's
columns and their data types.

---

### 7. Insert a Record

Use the `APPEND` command:

```text
VORIX> APPEND
Enter table name : students
Enter id: 1
Enter name: Aawesh
Enter age: 20

Record successfully addes.
```

The record is then stored in:

```text
school.vrx_students.data
```

---

### 8. Retrieve Records

Use the `RETRIEVE` command:

```text
VORIX> RETRIEVE
Enter table name: students

+----+--------+-----+
| id | name   | age |
+----+--------+-----+
| 1  | Aawesh | 20  |
+----+--------+-----+
```

VORIX automatically calculates column widths based on the table headers
and stored records before displaying the formatted table.

---

## Data Validation

VORIX performs basic validation when records are inserted.

### INTEGER Validation

When a column is defined as `INTEGER`, VORIX checks whether the entered
value is a valid integer.

Example:

```text
Enter age: abc
Invalid value. age must be INTEGER.
```

Valid values such as:

```text
20
-10
+15
```

are accepted by the integer validation logic.

---

## Duplicate ID Detection

If a table contains a column named `id`, VORIX checks whether the
entered ID already exists in the table.

Example:

```text
Enter id: 1
Error: ID 1 already exists.
```

This prevents duplicate IDs from being inserted into the same table.

---

## Persistent Storage

VORIX uses file-based persistence.

The current implementation separates storage into three types of files:

```text
Database
   |
   +-- school.vrx
   |
   +-- school.vrx_students.tbl
   |
   +-- school.vrx_students.data
```

### `.vrx`

Represents the database.

### `.tbl`

Stores the table schema.

### `.data`

Stores the records belonging to a table.

This design allows data to remain available after the VORIX program is
closed and started again.

---

## Project Structure

The current project is intentionally kept simple.

```text
VORIX/
│
├── main.c
├── README.md
└── .gitignore
```

The current implementation is contained in `main.c`.

As the project grows, the codebase can be separated into multiple modules
for storage, schema management, records, commands, and utilities.

---

## Technical Concepts

VORIX currently demonstrates practical implementation of:

- C structures
- Functions
- Function declarations
- File handling
- File-based persistence
- String manipulation
- Command-line interfaces
- Schema management
- Record management
- Input validation
- Duplicate record checking
- Formatted table output
- Basic data storage design
- Text-based data serialization

---

## Current Data Model

### Column

Each column currently contains:

```text
name
type
```

Example:

```text
id     INTEGER
name   TEXT
age    INTEGER
```

### Table

A table contains:

```text
name
column count
columns
```

VORIX currently supports up to 10 columns per table.

### Record

A record stores values corresponding to the table's columns.

Example:

```text
1
Aawesh
20
```

Multiple records can be stored in the table's `.data` file.

---

## Current Limitations

VORIX is currently an experimental and educational database-engine
project. It is not intended to replace production database systems.

Current limitations include:

- Maximum of 10 columns per table
- Maximum of 100 records loaded during retrieval
- Basic text-based storage format
- Limited data-type validation
- No SQL parser
- No `UPDATE` operation
- No `DELETE` operation
- No filtering or `WHERE` functionality
- No indexing system
- No transaction system
- No concurrent access handling
- Windows-specific filesystem functionality is currently used
- Input currently uses simple token-based scanning, so values containing
  spaces are not supported

---

## Current Implementation Notes

VORIX contains an initial helper function for calculating the next ID,
but the current record insertion flow does not automatically use this
function.

Therefore, IDs are currently entered manually by the user.

Automatic ID generation is planned as a future improvement rather than
being presented as a completed feature.

---

## Roadmap

The project will continue to evolve as additional database concepts are
implemented.

Planned improvements include:

### Record Operations

- `UPDATE` records
- `DELETE` records
- Better record editing
- Better record validation

### Querying

- Record filtering
- `WHERE`-style conditions
- Basic query language
- Searching records by column values

### Storage

- Improved storage format
- Better metadata management
- More efficient record storage
- Indexing
- Improved data integrity

### Data Types

- Better data-type support
- More robust type validation
- Additional numeric and text types

### Reliability

- Improved error handling
- Better input handling
- Corruption detection
- Safer file operations

### Portability

- Cross-platform filesystem support
- Linux support
- macOS support

### Architecture

As VORIX grows, the current single-file implementation can be separated
into multiple source and header files for better maintainability.

---

## Why VORIX?

The purpose of VORIX is not simply to create another CRUD application.

The project is being developed to understand the lower-level concepts
behind database systems.

Through VORIX, the project explores:

- How data can be persisted without an external database server
- How table schemas can be represented and stored
- How records can be written to and read from files
- How basic data validation works
- How duplicate records can be detected
- How a command-line database interface can be designed
- How database-like functionality can be built from the ground up

The project is intentionally being developed incrementally, starting
with basic file-based storage and gradually moving toward more advanced
database functionality.

---

## Technology

**Language**

- C

**Core Concepts**

- File I/O
- Structures
- Strings
- Functions
- Input validation
- Persistent storage
- CLI application design

**Storage**

- Local file system
- Custom `.vrx`, `.tbl`, and `.data` file formats

---

## Platform

The current implementation is designed for Windows because it uses
Windows-specific filesystem functions such as `_findfirst`, `_findnext`,
and `_findclose`.

Cross-platform support is planned for a future version.

---

## Status

**Development Status: Active / Experimental**

VORIX is an evolving personal project.

The current version focuses on establishing the core foundation of a
file-based database engine. More advanced database capabilities will be
implemented in future versions.

---

## Author

**Aawesh Tiwari**

VORIX is a personal C project focused on learning and implementing
database-system concepts from scratch.

---

## License

Copyright © 2026 Aawesh Tiwari.

All rights reserved.

This repository is publicly available for viewing and evaluation purposes.
The source code may not be copied, modified, distributed, or used in other
projects without explicit permission from the author.
