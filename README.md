# 📘 Final Project - Systems and Assembly Programming

This project answers 5 technical questions covering:
1. ELF binary reverse engineering  
2. Assembly file processing  
3. C extension module for Python  
4. Multithreaded producer-consumer model in C  
5. Linux socket-based chat system (client-server)

---

## ✅ Question 1: ELF Binary Reverse Engineering

**Binary:** `./question1`  
**Tools used:** `objdump`, `strace`, `gdb`

🔹 **Functions Identified**  
- `main`: Starts the program logic  
- `getStudentNames`: Reads user input  
- `writeToFile`, `readFromFile`: Handle file I/O  
- `sortNames`, `printNames`: Sort and display names  
- `.plt` entries: Calls like `fgets`, `printf`, `fopen`, `fclose`, `strcpy`, `strcmp`, etc.

🔹 **Control Flow**  
- Uses `jne` (Jump if Not Equal) for input validation and loop control  
- `test rax, rax` + `jne`: Skips logic if a null pointer or result is found  

🔹 **System Calls (via `strace`)**  
- `read`, `write`: User input/output  
- `openat`, `close`, `fstat`: File operations  
- `exit_group`: Program termination  

🔹 **Program Behavior**  
- Prompts user to input 10 student names  
- Writes them to `students.txt`  
- Sorts and writes to `sorted_students.txt`  
- Displays completion message on terminal  

---

## ✅ Question 2: Assembly Program to Count Names in File

**File:** `Q2.asm`  
**Input:** `names.txt` (Each line is a name)

🔹 **Behavior:**  
- Opens the file using syscalls  
- Reads contents into memory  
- Loops through buffer to count number of lines (based on `\n`)  
- Prints total count using `printf`

🔹 **Assembly Logic:**  
- File opened with syscall  
- Loop checks each character: if `\n`, increment counter  
- Result is printed

🔹 **To Compile and Run:**  
``bash
nasm -f elf64 Q2.asm
gcc -no-pie -o Q2 Q2.o
./Q2 

---

## ✅ Question 3: Python C Extension (`adder`)

### 🔹 Objective
Build a simple C function that adds two integers, and use it inside Python as a native module.

### 🔹 Files
- `adder.c`: C source file defining the `add` function.
- `setup.py`: Build script using `setuptools`.
- `test_adder.py`: Python script to test the compiled module.

### 🔹 Key Concepts
- `PyArg_ParseTuple`: Parses Python arguments in C.
- `PyLong_FromLong`: Converts C `int` to Python `int`.
- `Extension` and `PyModule_Create`: For creating Python modules in C.

### 🔹 Compilation Instructions
``bash
python3 setup.py build
cd build/lib*  # enter generated build folder
python3 ../../test_adder.py
# 🧵 Question 4: Producer-Consumer Using Threads

### 🔹 Objective
Implement a simple multithreaded system where:
- A **producer** thread generates characters A to Z.
- A **consumer** thread reads and prints those characters.
- They use a shared buffer of size 5 with synchronization.

### 🔹 Concepts Used
- `pthread_create`: To create threads.
- `pthread_mutex_t`: Mutex to protect shared buffer.
- `pthread_cond_t`: Condition variables to signal when buffer is full/empty.
- Shared circular buffer logic for writing/reading data.

### 🔹 Compilation
``bash
gcc producer_consumer.c -o pc -lpthread
# 💬 Question 5: TCP Chat System in Linux

## 🔹 Objective
Build a simple chat system where:
- The **server** listens for a client.
- The **client** connects and sends/receives messages.

## 🔹 Files Included
- `server.c`: Code for the server.
- `client.c`: Code for the client.

## 🔹 Key System Calls
- `socket()` – creates the socket.
- `bind()` – binds the socket to IP and port.
- `listen()` – server listens for incoming connections.
- `accept()` – server accepts the client's request.
- `connect()` – client connects to the server.
- `send()` / `recv()` – exchange messages.

## 🔹 How to Compile
Open terminal and run:
``bash
gcc server.c -o server
gcc client.c -o client
