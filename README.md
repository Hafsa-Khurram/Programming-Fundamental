# Banking System in C

My first coding project for the **Programming Fundamentals** course: a console banking system written in C.

## Features

### 1. ATM Transactions (PIN protected)
- PIN login, with 3 attempts before the card is blocked (default PIN: `1234`, see `Key.txt`)
- Balance enquiry, with date and time
- Cash withdrawal (limit of Rs 25,000 per transaction)
- Cash deposit
- Fast cash: Rs 5,000 / 10,000 / 15,000 / 20,000 / 25,000

### 2. Customer Records (CRUD)
- Insert, delete, update, display, and search customer records
- Save records to `bank_records.txt` and load them back
- Saved records are loaded automatically when the program starts
- Input is checked, so typing letters where a number is expected won't crash the program

## Files

| File | Description |
|------|-------------|
| `Bank.c` | Source code |
| `bank_records.txt` | Saved customer records (`account,name,balance,age`) |
| `Key.txt` | ATM PIN code |
| `.vscode/tasks.json` | VS Code build task |

## How to Run

You need a C compiler: **GCC** (on Windows, install MinGW-w64 or MSYS2).

**In VS Code:** open the folder, press `Ctrl+Shift+B` to build, then run in the terminal:

```bash
./Bank        # Linux / macOS
.\Bank.exe    # Windows
```

**From the command line:**

```bash
gcc Bank.c -o Bank
./Bank
```
