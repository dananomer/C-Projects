#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Accounts.h"

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
    return strlen(inputBuffer);
}

int mainMenu(Bank* bank) {
    int choice;
    char current_logged_account[16] = ADMIN_NUMBER;
    char input_buffer[256] = "";
    do
    {
        //TODO: loginDocument();
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
        choice = atoi(input_buffer);
        switch (choice)
        {
            case 1:
                createNewAccount(bank);
                break;
            case 2:
                removeExistingAccount(bank);
                break;
//            case 3:
//                loginExistingAccount(bank);
//                break;
            case 4:
                listAccounts(bank);
                break;
//            case 5:
//                retrievePassword(bank);
//                break;
//            case 6:
//                editAccount(bank);
//                break;
//            case 7:
//                transitFromDeposit(bank);
//                break;
//            case 8:
//                exitBank(bank);
//                break;
//            default:
//                printf("Wrong Choice. Enter again\n");
//                break;
        }

    } while (choice != 8);
    return 0;
}
size_t getSize(Bank* bank){
    size_t size = sizeof(*bank) / sizeof(Accounts*);
    return size;
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
    bank->account_set[index]->currency_type = atoi(input_buffer);

    buffer_length = ACCOUNT_BAL_SIZE+1;
    strcpy(question_buffer,"Enter Account initial balance");
    getInput(question_buffer,input_buffer,buffer_length);
    bank->account_set[index]->current_balance = atoi(input_buffer);

    bank->account_set[index]->enabled = ACCOUNT_ENABLE;

}
void createNewAccount(Bank *bank) {
    size_t size = getSize(bank);
    int account_index;
    for(int i=0;i<size;i++)
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

void removeExistingAccount(Bank *bank) {

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
    int account_index;
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

void listAccounts(Bank *bank) {

    size_t size = getSize(bank);
    int account_index;
    for (int i = 0; i < size; i++) {
        if (bank->account_set[i] != NULL) {
            return;
        }
    }
}