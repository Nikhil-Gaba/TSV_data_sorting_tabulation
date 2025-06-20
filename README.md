# TSV_data_sorting_tabulation
Description:
Developed a C-based program to read, store, sort, and display data from TSV (Tab-Separated Values) files. The system efficiently handles data parsing, memory-safe string operations, and implements custom sorting logic based on user-defined arguments.
Key Features:
•	Designed and implemented a custom parser to tokenize TSV input into a 2D array of strings.
•	Utilized robust string handling techniques using C standard libraries to manage memory and formatting.
•	Built multi-stage data output with modular formatting:
o	Stage 1: Parses and displays dataset dimensions and preview of the last row.
o	Stage 2: Sorts the dataset by one or more columns using custom comparison logic.
o	Stage 3: Concatenates sorted strings, calculates alignment for visual display, and prints data with indentation and formatting.
•	Implemented command-line argument handling for flexible sorting control.
•	Included unit-style modularity via reusable helper functions for parsing, swapping rows/fields, string comparisons, and formatting.
•	Handles various edge cases including line endings (Unix/Windows), overflow protection, and EOF behavior.
Technologies:
C, String manipulation, Command-line arguments, Custom sorting algorithms, Data structures, File parsing, Modular programming

