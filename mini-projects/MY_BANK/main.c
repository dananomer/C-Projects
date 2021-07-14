//
// Created by omerd on 7/11/2021.
//
#include <stdio.h>
#include "Accounts.h"
#include "string.h"

int main() {
    Bank bank;
    initBank(&bank);
    while(1)
    {
        mainMenu(&bank);
    }
    printf("Hello, World!");
    return 0;
}