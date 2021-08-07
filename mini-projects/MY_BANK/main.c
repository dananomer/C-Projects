#include <stdio.h>
#include "Accounts.h"
#include<unistd.h>

int main() {
    chdir("..");
    Bank bank;
    initBank(&bank);
    mainMenu(&bank);
    printf("Bank is closing\n");
    return 0;
}