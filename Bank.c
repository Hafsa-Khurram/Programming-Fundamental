/*
 * Banking System - Programming Fundamentals Project
 *
 * Features:
 *   1. ATM Transactions (PIN protected)
 *        - Balance Enquiry
 *        - Withdraw Cash (limit Rs 25,000 per transaction)
 *        - Deposit Cash
 *        - Fast Cash Withdraw
 *   2. Customer Records (CRUD Operations)
 *        - Insert, Delete, Update, Display, Search
 *        - Save records to file / Load records from file
 *
 * Compile:  gcc Bank.c -o Bank
 * Run:      ./Bank        (Linux / macOS)
 *           Bank.exe      (Windows)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CUSTOMERS 20
#define NAME_LENGTH 50
#define RECORDS_FILE "bank_records.txt"

#define ATM_PIN 1234
#define MAX_PIN_ATTEMPTS 3
#define WITHDRAW_LIMIT 25000L
#define STARTING_BALANCE 100000L

struct Customer
{
    int accountNumber;
    char name[NAME_LENGTH];
    float balance;
    int age;
};

struct Customer bank[MAX_CUSTOMERS];
int numCustomers = 0;

/* ================= Input Helpers ================= */

/* Reads one line from the keyboard and removes the trailing newline. */
void readLine(char *buffer, int size)
{
    if (fgets(buffer, size, stdin) == NULL)
    {
        /* End of input (e.g. Ctrl+Z / Ctrl+D): exit cleanly */
        printf("\nInput closed. Exiting...\n");
        exit(0);
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
    else
    {
        /* Line was longer than the buffer: discard the rest of it */
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
}

/* Keeps asking until the user types a whole number. */
long readLong(const char *prompt)
{
    char line[100];
    char *end;
    long value;

    while (1)
    {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        value = strtol(line, &end, 10);

        while (*end == ' ' || *end == '\t')
            end++;

        if (end != line && *end == '\0')
            return value;

        printf("Invalid input. Please enter a number.\n");
    }
}

int readInt(const char *prompt)
{
    return (int)readLong(prompt);
}

/* Keeps asking until the user types a number (decimals allowed). */
float readFloat(const char *prompt)
{
    char line[100];
    char *end;
    float value;

    while (1)
    {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        value = strtof(line, &end);

        while (*end == ' ' || *end == '\t')
            end++;

        if (end != line && *end == '\0')
            return value;

        printf("Invalid input. Please enter a number.\n");
    }
}

/* Reads a name (spaces allowed, commas not allowed because of the file format). */
void readName(const char *prompt, char *name)
{
    while (1)
    {
        printf("%s", prompt);
        readLine(name, NAME_LENGTH);

        if (strlen(name) == 0)
            printf("Name cannot be empty.\n");
        else if (strchr(name, ',') != NULL)
            printf("Name cannot contain commas.\n");
        else
            return;
    }
}

/* Asks a yes/no question. Returns 1 for yes, 0 for no. */
int askYesNo(const char *prompt)
{
    char line[20];

    while (1)
    {
        printf("%s", prompt);
        readLine(line, sizeof(line));

        if (line[0] == 'y' || line[0] == 'Y')
            return 1;
        if (line[0] == 'n' || line[0] == 'N')
            return 0;

        printf("Please press 'y' for Yes or 'n' for No.\n");
    }
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnterToContinue()
{
    char line[10];
    printf("\nPress Enter to continue...");
    readLine(line, sizeof(line));
}

void printDateTime()
{
    time_t now = time(NULL);
    printf("\n\n\t%s", ctime(&now));
}

/* ================= CRUD Operations ================= */

int findCustomer(int accountNumber)
{
    for (int i = 0; i < numCustomers; i++)
    {
        if (bank[i].accountNumber == accountNumber)
            return i;
    }
    return -1;
}

void readAge(const char *prompt, int *age)
{
    while (1)
    {
        *age = readInt(prompt);
        if (*age > 0 && *age <= 120)
            return;
        printf("Please enter a valid age (1 - 120).\n");
    }
}

void readBalance(const char *prompt, float *balance)
{
    while (1)
    {
        *balance = readFloat(prompt);
        if (*balance >= 0)
            return;
        printf("Balance cannot be negative.\n");
    }
}

void insertRecord()
{
    if (numCustomers >= MAX_CUSTOMERS)
    {
        printf("Cannot insert record. Maximum number of customers reached.\n");
        return;
    }

    struct Customer newCustomer;

    newCustomer.accountNumber = readInt("Enter account number: ");
    if (newCustomer.accountNumber <= 0)
    {
        printf("Account number must be a positive number.\n");
        return;
    }

    /* Check if the account number already exists */
    if (findCustomer(newCustomer.accountNumber) != -1)
    {
        printf("Account number already exists. Cannot insert record.\n");
        return;
    }

    readName("Enter name: ", newCustomer.name);
    readBalance("Enter balance: ", &newCustomer.balance);
    readAge("Enter age: ", &newCustomer.age);

    bank[numCustomers] = newCustomer;
    numCustomers++;
    printf("Record inserted successfully.\n");
}

void deleteRecord()
{
    int accountNumber = readInt("Enter account number to delete: ");
    int index = findCustomer(accountNumber);

    if (index == -1)
    {
        printf("Record not found.\n");
        return;
    }

    /* Shift the remaining records one place to the left */
    for (int j = index; j < numCustomers - 1; j++)
    {
        bank[j] = bank[j + 1];
    }
    numCustomers--;
    printf("Record deleted successfully.\n");
}

void updateRecord()
{
    int accountNumber = readInt("Enter account number to update: ");
    int index = findCustomer(accountNumber);

    if (index == -1)
    {
        printf("Record not found.\n");
        return;
    }

    readName("Enter new name: ", bank[index].name);
    readBalance("Enter new balance: ", &bank[index].balance);
    readAge("Enter new age: ", &bank[index].age);
    printf("Record updated successfully.\n");
}

void printCustomer(const struct Customer *c)
{
    printf("***********************************************\n");
    printf("* Account Number : %d\n", c->accountNumber);
    printf("* Customer Name  : %s\n", c->name);
    printf("* Balance        : Rs %.2f\n", c->balance);
    printf("* Age            : %d\n", c->age);
    printf("***********************************************\n");
}

void displayRecords()
{
    printf("/////////////// Account Details ///////////////\n");

    if (numCustomers == 0)
    {
        printf("No records to display.\n");
        return;
    }

    for (int i = 0; i < numCustomers; i++)
    {
        printCustomer(&bank[i]);
    }
    printf("Total customers: %d\n", numCustomers);
}

void searchRecord()
{
    int accountNumber = readInt("Enter account number to search: ");
    int index = findCustomer(accountNumber);

    if (index == -1)
        printf("Record not found.\n");
    else
        printCustomer(&bank[index]);
}

void saveRecordsToFile()
{
    FILE *file = fopen(RECORDS_FILE, "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numCustomers; i++)
    {
        fprintf(file, "%d,%s,%.2f,%d\n", bank[i].accountNumber, bank[i].name, bank[i].balance, bank[i].age);
    }

    fclose(file);
    printf("%d record(s) saved to %s.\n", numCustomers, RECORDS_FILE);
}

/*
 * Loads records from the file into memory.
 * If showMessages is 0 (used at program start), it stays quiet when the file is missing.
 */
void loadRecordsFromFile(int showMessages)
{
    FILE *file = fopen(RECORDS_FILE, "r");
    if (file == NULL)
    {
        if (showMessages)
            printf("No saved records found (%s does not exist yet).\n", RECORDS_FILE);
        return;
    }

    char line[200];
    int skipped = 0;
    numCustomers = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        struct Customer c;

        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0')
            continue;

        if (sscanf(line, "%d,%49[^,],%f,%d", &c.accountNumber, c.name, &c.balance, &c.age) != 4 ||
            findCustomer(c.accountNumber) != -1)
        {
            skipped++;
            continue;
        }

        if (numCustomers >= MAX_CUSTOMERS)
        {
            printf("Warning: file has more than %d records. Extra records were not loaded.\n", MAX_CUSTOMERS);
            break;
        }

        bank[numCustomers] = c;
        numCustomers++;
    }

    fclose(file);

    if (showMessages)
    {
        printf("%d record(s) loaded from %s.\n", numCustomers, RECORDS_FILE);
        if (skipped > 0)
            printf("%d invalid or duplicate line(s) were skipped.\n", skipped);
    }
}

void crudMenu()
{
    int choice;

    do
    {
        printf("\n***********************************************\n");
        printf("*             //Banking System//              *\n");
        printf("*            1. Insert Record                 *\n");
        printf("*            2. Delete Record                 *\n");
        printf("*            3. Update Record                 *\n");
        printf("*            4. Display Records               *\n");
        printf("*            5. Search Record                 *\n");
        printf("*            6. Save Records to File          *\n");
        printf("*            7. Load Records from File        *\n");
        printf("*            8. Back to Main Menu             *\n");
        printf("***********************************************\n");

        choice = readInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            insertRecord();
            break;
        case 2:
            deleteRecord();
            break;
        case 3:
            updateRecord();
            break;
        case 4:
            displayRecords();
            break;
        case 5:
            searchRecord();
            break;
        case 6:
            saveRecordsToFile();
            break;
        case 7:
            loadRecordsFromFile(1);
            break;
        case 8:
            if (askYesNo("Save records to file before leaving? (y/n): "))
                saveRecordsToFile();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);
}

/* ================= ATM Transactions ================= */

int checkPin()
{
    for (int attempt = 1; attempt <= MAX_PIN_ATTEMPTS; attempt++)
    {
        int pin = readInt("\n ENTER YOUR PIN NUMBER: ");
        if (pin == ATM_PIN)
            return 1;

        if (attempt < MAX_PIN_ATTEMPTS)
            printf("\n WRONG PIN. %d attempt(s) left.\n", MAX_PIN_ATTEMPTS - attempt);
    }

    printf("\n TOO MANY WRONG ATTEMPTS. YOUR CARD HAS BEEN BLOCKED.\n");
    return 0;
}

void showRemainingBalance(long amount)
{
    if (askYesNo("\nDO YOU WANT TO KNOW YOUR REMAINING BALANCE? (y/n): "))
    {
        printf("\n NOW YOUR CURRENT BALANCE IS Rs : %ld", amount);
        printDateTime();
    }
}

/* Takes cash out of the account if the rules allow it. Returns 1 on success. */
int withdrawCash(long *amount, long withdraw)
{
    if (withdraw <= 0)
    {
        printf("\n PLEASE ENTER A VALID AMOUNT.\n");
        return 0;
    }
    if (withdraw > WITHDRAW_LIMIT)
    {
        printf("\n YOUR LIMIT IS Rs 25,000 PER TRANSACTION.\n PLEASE DO NOT EXCEED THE LIMIT.\n");
        return 0;
    }
    if (withdraw > *amount)
    {
        printf("\n INSUFFICIENT BALANCE.\n");
        return 0;
    }

    *amount -= withdraw;
    printf("\n PLEASE COLLECT CASH OF Rs : %ld\n", withdraw);
    printDateTime();
    return 1;
}

void printThankYou()
{
    printf(".....................................\n");
    printf(".                                   .\n");
    printf(".  THANK YOU FOR USING ATM SERVICE  .\n");
    printf(".                                   .\n");
    printf(".....................................\n");
}

void atmMenu()
{
    long amount = STARTING_BALANCE;
    const long fastCashOptions[] = {5000, 10000, 15000, 20000, 25000};
    int choice;

    printf("//////////////////////////////////////////////////////////////\n");
    printf("/            Welcome To Static Bank Of Pakistan.             /\n");
    printf("//////////////////////////////////////////////////////////////\n");

    if (!checkPin())
        return;

    do
    {
        clearScreen();
        printf("============= Welcome to ATM Service =============\n");
        printf("*          Please Choose Transaction:            *\n");
        printf("*          1. Balance Enquiry                    *\n");
        printf("*          2. Withdraw Cash                      *\n");
        printf("*          3. Deposit Cash                       *\n");
        printf("*          4. Fast Cash Withdraw                 *\n");
        printf("*          5. Quit                               *\n");
        printf("==================================================\n");

        choice = readInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            printf("\n YOUR BALANCE IS Rs : %ld", amount);
            printDateTime();
            break;

        case 2:
        {
            long withdraw = readLong("\n ENTER THE AMOUNT TO WITHDRAW: ");
            if (withdrawCash(&amount, withdraw))
                showRemainingBalance(amount);
            break;
        }

        case 3:
        {
            long deposit = readLong("\n ENTER THE AMOUNT TO DEPOSIT: ");
            if (deposit <= 0)
            {
                printf("\n PLEASE ENTER A VALID AMOUNT.\n");
            }
            else
            {
                amount += deposit;
                printf("\n Rs %ld DEPOSITED SUCCESSFULLY.\n YOUR BALANCE IS Rs : %ld", deposit, amount);
                printDateTime();
            }
            break;
        }

        case 4:
        {
            printf("++++++++++++++++++++++++\n");
            printf("+  Press 1 For 5,000   +\n");
            printf("+  Press 2 For 10,000  +\n");
            printf("+  Press 3 For 15,000  +\n");
            printf("+  Press 4 For 20,000  +\n");
            printf("+  Press 5 For 25,000  +\n");
            printf("++++++++++++++++++++++++\n");

            int fastCash = readInt("Please Choose Fast Cash Option: ");
            if (fastCash < 1 || fastCash > 5)
            {
                printf("\n INVALID FAST CASH OPTION.\n");
            }
            else if (withdrawCash(&amount, fastCashOptions[fastCash - 1]))
            {
                showRemainingBalance(amount);
            }
            break;
        }

        case 5:
            break;

        default:
            printf("\n INVALID CHOICE.\n");
        }

        if (choice != 5 && !askYesNo("\nDO YOU WISH TO HAVE ANOTHER TRANSACTION? (y/n): "))
            choice = 5;

    } while (choice != 5);

    clearScreen();
    printThankYou();
}

/* ================= Main ================= */

int main()
{
    int option;

    /* Load saved customer records (if any) when the program starts */
    loadRecordsFromFile(0);

    do
    {
        printf("\n***********************************************\n");
        printf("*        STATIC BANK OF PAKISTAN - MENU       *\n");
        printf("*          1. ATM Transactions                *\n");
        printf("*          2. Customer Records (CRUD)         *\n");
        printf("*          3. Exit                            *\n");
        printf("***********************************************\n");

        option = readInt("Enter your option: ");

        switch (option)
        {
        case 1:
            atmMenu();
            pressEnterToContinue();
            break;
        case 2:
            crudMenu();
            break;
        case 3:
            printf("Exiting... Goodbye!\n");
            break;
        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (option != 3);

    return 0;
}
