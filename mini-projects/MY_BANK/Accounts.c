#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Accounts.h"
#include <sys/stat.h>

int getInput(char *question, char *inputBuffer, int bufferLength)
{
    printf("%s  (Max %d characters)\n", question, bufferLength - 1);
    fgets(inputBuffer, bufferLength, stdin);

    if (inputBuffer[strlen(inputBuffer) -1] != '\n')
    {
        int dropped = 0;
        while (fgetc(stdin) != '\n')
            dropped++;

        if (dropped > 0) // if they input exactly (bufferLength - 1)
            // characters, there's only the \n to chop off
        {
            printf("your input was over the limit by %d characters, try again!\n", dropped );
            getInput(question, inputBuffer, bufferLength);
        }
    }
    else
    {
        inputBuffer[strlen(inputBuffer) -1] = '\0';
    }
    return (int)strlen(inputBuffer);
}

int mainMenu(Bank* bank) {
    int choice;
    char* current_logged_account = (char*)calloc(ACCOUNT_NUM_SIZE+1,sizeof(char));
    strcpy(current_logged_account,ADMIN_NUMBER);
    char input_buffer[256] = "";
    do
    {

        printf("Menu\n\n");
        printf("1. create new account\n");
        printf("2. remove existing account\n");
        printf("3. login to existing account\n");
        printf("4. list accounts\n");
        printf("5. view account details\n");
        printf("6. retrieve account password\n");
        printf("7. edit account\n");
        printf("8. transit deposit\n");
        printf("9. exit\n");

        fgets(input_buffer, 256, stdin);
        choice = strtol(input_buffer,NULL,10);
        switch (choice)
        {
            case 1:
                createNewAccount(bank,&current_logged_account);
                break;
            case 2:
                removeExistingAccount(bank,&current_logged_account);
                break;
            case 3:
                loginExistingAccount(bank,&current_logged_account);
                break;
            case 4:
                listAccounts(bank,&current_logged_account);
                break;
            case 5:
                viewAccountDetails(bank,&current_logged_account);
                break;

            case 6:
                retrievePassword(bank,&current_logged_account);
                break;
            case 7:
                editAccount(bank);
                break;
//            case 8:
//                transitFromDeposit(bank);
//                break;
            case 9:
                exitBank(bank,&current_logged_account));
                break;
            default:
                printf("Wrong Choice. Enter again\n");
                break;
        }

    } while (choice != 9);
    return 0;
}
size_t getSize(Bank* bank){
    size_t size = sizeof(*bank) / sizeof(Accounts*);
    return size;
}
void copyFromFile(Bank* bank) {

    FILE *reads;

    char filename[10] = "";
    int temp;

    getInput("Type in name of the file",filename,10);
    reads = fopen(filename, "r");
    if (reads == NULL) {
        perror("Error");
        exit(1);
    } else {
        int counter = 1;
        while (!feof(reads)) {
            bank->account_set[counter] = (Accounts *) calloc(1, sizeof(Accounts));
            fscanf(reads, "%s %s %s %d %d",
                   &(bank->account_set[counter]->account_name),
                   &(bank->account_set[counter]->account_number),
                   &(bank->account_set[counter]->account_code),
                   &(bank->account_set[counter]->currency_type),
                   &(bank->account_set[counter]->current_balance)
            );
            bank->account_set[counter]->enabled = ACCOUNT_ENABLE;

            counter++;
        }
    }
}
void initBank(Bank* bank) {
    size_t size = getSize(bank);
    for(int i=0;i<size;i++)
    {
        bank->account_set[i] = NULL;
    }
    //INIT ADMINISTRATOR AS ACCOUNT 0
    bank->account_set[0] = (Accounts*)calloc(1,sizeof(Accounts));
    strcpy(bank->account_set[0]->account_name, ADMIN_NAME);
    strcpy(bank->account_set[0]->account_number, ADMIN_NUMBER);
    strcpy(bank->account_set[0]->account_code, ACC_CODE_INIT);
    bank->account_set[0]->currency_type = CURR_TYPE_INIT;
    bank->account_set[0]->current_balance = CURR_BAL_INIT;
    bank->account_set[0]->enabled = ACCOUNT_ENABLE;

    copyFromFile(bank);

}
void buildAccount(Bank *bank,int index){
    char input_buffer[256] = "";
    char question_buffer[256] = "";
    int buffer_length;
    int input_size;

    bank->account_set[index] = (Accounts*)calloc(1,sizeof(Accounts));

    buffer_length = ACCOUNT_NAME_SIZE+1;
    strcpy(question_buffer,"Enter Name of the account");
    input_size = getInput(question_buffer,input_buffer,buffer_length);
    input_buffer[input_size] = '\0';
    strncpy(bank->account_set[index]->account_name,input_buffer,input_size+1);

    buffer_length = ACCOUNT_NUM_SIZE+1;
    strcpy(question_buffer,"Enter Account number");
    input_size = getInput(question_buffer,input_buffer,buffer_length);
    input_buffer[input_size] = '\0';
    strncpy(bank->account_set[index]->account_number,input_buffer,input_size+1);

    buffer_length = ACCOUNT_CODE_SIZE+1;
    strcpy(question_buffer,"Enter Account code");
    input_size = getInput(question_buffer,input_buffer,buffer_length);
    input_buffer[input_size] = '\0';
    strncpy(bank->account_set[index]->account_code,input_buffer,input_size+1);

    buffer_length = ACCOUNT_TYPE_SIZE+1;
    strcpy(question_buffer,"Enter Currency type");
    getInput(question_buffer, input_buffer, buffer_length);
    bank->account_set[index]->currency_type = strtol(input_buffer,NULL,10);

    buffer_length = ACCOUNT_BAL_SIZE+1;
    strcpy(question_buffer,"Enter Account initial balance");
    getInput(question_buffer,input_buffer,buffer_length);
    bank->account_set[index]->current_balance = strtol(input_buffer,NULL,10);

    bank->account_set[index]->enabled = ACCOUNT_ENABLE;


}
void createNewAccount(Bank *bank,char ** current_logged_account) {
    if(strcmp(*current_logged_account, ADMIN_NUMBER) != 0)
    {
        printf("cannot add new account from unsupervised account, return to main menu\n");
        return;
    }
    size_t size = getSize(bank);
    int account_index = 0;
    for(int i=0;i < size;i++)
    {
        if(bank->account_set[i] == NULL ||
        bank->account_set[i]->enabled == ACCOUNT_DISABLE)
        {
            account_index = i;
            break;
        }

    }
    buildAccount(bank,account_index);
}

void removeExistingAccount(Bank *bank,char ** current_logged_account) {

    if(strcmp(*current_logged_account, ADMIN_NUMBER) != 0)
    {
        printf("cannot delete existing account from unsupervised account, return to main menu\n");
        return;
    }
    char input_buffer[256] = "";
    char question_buffer[256] = "";
    char query_number[ACCOUNT_NUM_SIZE+1] = "";
    int buffer_length;
    int input_size;

    buffer_length = ACCOUNT_NUM_SIZE+1;
    strcpy(question_buffer,"Enter Account number you wish to delete");
    input_size = getInput(question_buffer,input_buffer,buffer_length);
    input_buffer[input_size] = '\0';
    printf("input size is %d\n",input_size);
    strncpy(query_number,input_buffer,input_size+1);

    size_t size = getSize(bank);
    for (int i = 0; i < size; i++) {
        if (bank->account_set[i] != NULL) {
            if(bank->account_set[i]->enabled == ACCOUNT_ENABLE)
            {
                if (strcmp(bank->account_set[i]->account_number, query_number) == 0){
                    bank->account_set[i]->enabled = ACCOUNT_DISABLE;
                    printf("found %s in index %d, disable account\n",query_number,i);
                    break;
                }
            }
        }
    }
}

void listAccounts(Bank *bank,char ** current_logged_account) {

    if(strcmp(*current_logged_account, ADMIN_NUMBER) != 0)
    {
        printf("cannot add new account from unsupervised account, return to main menu\n");
        return;
    }

    printf("Listing all registered bank accounts\n");
    size_t size = getSize(bank);
    int status;
    for (int i = 0; i < size; i++) {
        if (bank->account_set[i] != NULL) {
            printf("Index: %d\n",i);
            printf("Name: %s\n",bank->account_set[i]->account_name);
            printf("Account Number: %s\n",bank->account_set[i]->account_number);
            printf("Account Access Code: %s\n",bank->account_set[i]->account_code);
            printf("Account Type: %d\n",bank->account_set[i]->currency_type);
            printf("Account Current Balance: %d\n",bank->account_set[i]->current_balance);
            status = bank->account_set[i]->enabled;
            if(status)
                printf("Status: Enabled\n\n");
            else
                printf("Status: Disabled\n\n");

        }
    }
}

void exitBank(Bank *bank, char ** current_logged_account_ptr) {
    size_t size = getSize(bank);
    for(int i=0;i<size;i++)
    {
        if(bank->account_set[i] != NULL)
        {
            free(bank->account_set[i]);
            bank->account_set[i] = NULL;
        }

    }
    free(current_logged_account_ptr);
}

void editAccount(Bank *bank) {

    char input_buffer[256] = "";
    char question_buffer[256] = "";
    char query_number[ACCOUNT_NUM_SIZE+1] = "";
    int buffer_length;
    int input_size;

    buffer_length = ACCOUNT_NUM_SIZE+1;
    strcpy(question_buffer,"Enter Account number you wish to edit");
    input_size = getInput(question_buffer,input_buffer,buffer_length);
    input_buffer[input_size] = '\0';
    printf("input size is %d\n",input_size);
    strncpy(query_number,input_buffer,input_size+1);

    size_t size = getSize(bank);
    for (int i = 0; i < size; i++) {
        if (bank->account_set[i] != NULL) {
            if (bank->account_set[i]->enabled == ACCOUNT_ENABLE) {
                if (strcmp(bank->account_set[i]->account_number, query_number) == 0) {
                    editMenu(bank,i);
                    break;
                }
            }
        }
    }
}

void editMenu(Bank *bank, int account_index) {
    int choice;
    char input_buffer[256] = "";
    char question_buffer[256] = "";
    int buffer_length = 0;
    int input_size = 0;
    do
    {
        printf("Edit Menu\n\n");
        printf("1. edit account name\n");
        printf("2. edit account number\n");
        printf("3. return\n");

        fgets(input_buffer, 256, stdin);
        choice = strtol(input_buffer,NULL,10);
        switch (choice)
        {
            case 1:
                buffer_length = ACCOUNT_NAME_SIZE+1;
                strcpy(question_buffer,"Enter new Name");
                input_size = getInput(question_buffer,input_buffer,buffer_length);
                input_buffer[input_size] = '\0';
                strncpy(bank->account_set[account_index]->account_name,input_buffer,input_size+1);
                break;
            case 2:
                buffer_length = ACCOUNT_NUM_SIZE+1;
                strcpy(question_buffer,"Enter new Account number");
                input_size = getInput(question_buffer,input_buffer,buffer_length);
                input_buffer[input_size] = '\0';
                strncpy(bank->account_set[account_index]->account_number,input_buffer,input_size+1);
                break;
            case 3:
                printf("Account remain unchanged, return to main menu\n");
                break;
            default:
                printf("Wrong Choice. Enter again\n");
                break;
        }

    } while (choice != 3);
}

void loginExistingAccount(Bank *bank, char **current_logged_account_ptr) {

    char input_buffer[256] = "";
    char question_buffer[256] = "";
    char query_number[ACCOUNT_NUM_SIZE+1] = "";
    int buffer_length;
    int input_size;

    buffer_length = ACCOUNT_NUM_SIZE+1;
    strcpy(question_buffer,"Enter Account number you wish to login");
    input_size = getInput(question_buffer,input_buffer,buffer_length);
    input_buffer[input_size] = '\0';
    printf("input size is %d\n",input_size);
    strncpy(query_number,input_buffer,input_size+1);


    size_t size = getSize(bank);
    for (int i = 0; i < size; i++) {
        if (bank->account_set[i] != NULL) {
            if (bank->account_set[i]->enabled == ACCOUNT_ENABLE) {
                if (strcmp(bank->account_set[i]->account_number, query_number) == 0) {
                    if(checkCredentials(bank,i,query_number))
                    {
                        printf("login as %s\n",bank->account_set[i]->account_name);
                        strncpy(*current_logged_account_ptr,bank->account_set[i]->account_number,input_size+1);
                    }
                    break;
                }
            }
        }
    }

}

int checkCredentials(Bank *bank, int index,char* query_number) {

    char input_buffer[256] = "";
    char question_buffer[256] = "";
    char code_number[ACCOUNT_CODE_SIZE+1] = "";
    int buffer_length;
    int input_size;


    int tries = 3;
    while(tries > 0)
    {
        buffer_length = ACCOUNT_CODE_SIZE+1;
        strcpy(question_buffer,"Enter Code to login");
        input_size = getInput(question_buffer,input_buffer,buffer_length);
        input_buffer[input_size] = '\0';
        printf("input size is %d\n",input_size);
        strncpy(query_number,input_buffer,input_size+1);

        if(strcmp(query_number,bank->account_set[index]->account_code) == 0)
        {
            return 1;
        }
        else if(tries > 1)
        {
            printf("Wrong code try again - %d more tries left\n",tries);
            tries--;

        }
        else
        {
            printf("You failed the third time, return to menu...\n");
            return 0;
        }
    }
}


void viewAccountDetails(Bank *bank, char **current_logged_account_ptr) {

    size_t size = getSize(bank);
    for(int i=0;i< size;i++)
    {
        if(strcmp(*current_logged_account_ptr,bank->account_set[i]->account_number) == 0) {
            printf("List account details of logged account:\n");
            printf("Name: %s\n",bank->account_set[i]->account_name);
            printf("Account Number: %s\n",bank->account_set[i]->account_number);
            printf("Account Access Code: %s\n",bank->account_set[i]->account_code);
            printf("Account Type: %d\n",bank->account_set[i]->currency_type);
            printf("Account Current Balance: %d\n",bank->account_set[i]->current_balance);
            return;
        }
    }
}

void retrievePassword(Bank *bank, char **current_logged_account_ptr) {

    size_t size = getSize(bank);

    if(strcmp(*current_logged_account_ptr, ADMIN_NUMBER) == 0)
    {
        char input_buffer[256] = "";
        char question_buffer[256] = "";
        char query_number[ACCOUNT_NUM_SIZE+1] = "";
        int buffer_length;
        int input_size;

        buffer_length = ACCOUNT_NUM_SIZE+1;
        strcpy(question_buffer,"Enter Account number you wish to retrieve password");
        input_size = getInput(question_buffer,input_buffer,buffer_length);
        input_buffer[input_size] = '\0';
        printf("input size is %d\n",input_size);
        strncpy(query_number,input_buffer,input_size+1);

        for(int i=0;i< size;i++)
        {
            if(strcmp(query_number,bank->account_set[i]->account_number) == 0) {
                printf("retrieve user password:\n");
                printf("Access Code: %s\n",bank->account_set[i]->account_code);
                return;
            }
        }

        return;
    }
    else
    {
        for(int i=0;i< size;i++)
        {
            if(strcmp(*current_logged_account_ptr,bank->account_set[i]->account_number) == 0) {
                printf("retreive current user's password:\n");
                printf("Access Code: %s\n",bank->account_set[i]->account_code);
                return;
            }
        }
    }

}

void transitFromDeposit(Bank *bank) {

}

