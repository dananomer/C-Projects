#ifndef MY_BANK_ACCOUNTS_H
#define MY_BANK_ACCOUNTS_H

#define ADMIN_NUMBER "FFFF"
#define ADMIN_NAME "admin"

#define ACC_CODE_INIT "000"
#define CURR_TYPE_INIT 0
#define CURR_BAL_INIT 0

#define ACCOUNT_ENABLE 1
#define ACCOUNT_DISABLE 0

#define ACCOUNT_NAME_SIZE 10
#define ACCOUNT_NUM_SIZE 4
#define ACCOUNT_CODE_SIZE 3
#define ACCOUNT_TYPE_SIZE 1
#define ACCOUNT_BAL_SIZE 5

typedef struct Accounts {
    int enabled;
    char  account_name[ACCOUNT_NAME_SIZE+1];
    char  account_number[ACCOUNT_NUM_SIZE+1];
    char account_code[ACCOUNT_CODE_SIZE+1];
    int  currency_type;
    int current_balance;
} Accounts;

typedef struct Bank{
    Accounts* account_set[100];
} Bank;

int getInput(char *question, char *inputBuffer, int bufferLength);

/** a text-based menu which allow users to access all bank functions:
 * \methods - create new account ( only administrator )
 * \methods - login to existing account ( require account_code )
 * \methods - edit account ( require login with user )
 * \methods - list account details
 * \methods - transact from logged account to other existing account
 * \methods - erase account ( only administrator )
 * @return no return
 * */
int mainMenu(Bank* bank);

size_t getSize(Bank* bank);

void copyFromFile(Bank* bank);

void initBank(Bank* bank);

void buildAccount(Bank *bank,int index);

void createNewAccount(Bank* bank,char ** current_logged_account);

void removeExistingAccount(Bank* bank,char ** current_logged_account);

void loginExistingAccount(Bank* bank,char** current_logged_account_ptr);

int checkCredentials(Bank* bank,int index,char* query_number);

void listAccounts(Bank* bank,char ** current_logged_account);

void viewAccountDetails(Bank* bank, char **current_logged_account_ptr);

void retrievePassword(Bank* bank, char **current_logged_account_ptr);

void editAccount(Bank* bank);

void editMenu(Bank* bank,int account_index);

void transitFromDeposit(Bank* bank);

void exitBank(Bank* bank, char **current_logged_account_ptr);

#endif //MY_BANK_ACCOUNTS_H
