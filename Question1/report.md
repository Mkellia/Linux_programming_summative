# 📄 Question 1: ELF Binary Analysis Report

**Binary File Used:** `./binaryfile`

---

## 🔹 Tools Used

- **objdump** – Disassembles binary to view functions and control flow.
- **strace** – Tracks system calls during program execution.
- **gdb** – Debugger to inspect function calls and memory access.

---

## 🔹 objdump Findings

- **Functions Identified:**
  - `main`, `getStudentNames`, `writeToFile`, `readFromFile`, `sortNames`, `printNames`
  - Standard library calls: `fgets`, `puts`, `strcmp`, `printf`, etc.

- **Control Flow Example:**
  - `jne 0x40114e` – *Jump if Not Equal*
    - If the condition is false, jumps to a different address.
    - Example use: like in an `if (x != y)` condition.

---

## 🔹 strace Output Summary

- **System Calls Observed:**
  - `write`, `read` – Used for terminal input/output.
  - `openat`, `fstat`, `close` – For file operations.
  - `exit_group` – Normal program termination.

- **Program Behavior:**
  1. Prompts user to enter names.
  2. Saves names in `students.txt`.
  3. Reads and sorts names.
  4. Saves sorted names in `sorted_students.txt`.

---

## 🔹 gdb Observations

- **Breakpoints** set in `main()` helped trace the flow.
- **Memory Inspection** showed stack variables and transitions between functions.
- **Watchpoints** were used to monitor variable changes during execution.

---

## 🔹 Final Summary

The binary collects 10 student names, writes them to a file, sorts them, and saves the sorted list to another file. We confirmed all of this using objdump, strace, and gdb.

---
