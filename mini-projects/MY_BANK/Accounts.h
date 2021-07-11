//
// Created by omerd on 7/11/2021.
//

#ifndef MY_BANK_ACCOUNTS_H
#define MY_BANK_ACCOUNTS_H

struct Accounts {
    char  account_name[10];
    char  account_number[16];
    char account_code[3];
    int  currency_type;
    int current_balance;
};
/**
 * a text-based menu which allow users to access all bank functions:
 * create new account ( only administrator )
 * login to existing account ( require account_code )
 * edit account ( require login with user )
 * list account details
 * transact from logged account to other existing account
 * erase account ( only administrator )
 * @return
 */
int menu();
/**
 * login to existing account, require account code
 * @return
 */
int login();
/**
 * create new account, require to login with administrator account
 * @return
 */
int new_acc();
/**
 * list all account details
 * @return
 */
int list();
int edit();
int transact();
int erase();
#endif //MY_BANK_ACCOUNTS_H
