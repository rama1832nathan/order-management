#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include "registration.h"


void registration(void);
int login(void);

bool fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}


void createFiles() {
    if (!fileExists("userdata.csv")) {
        FILE *file = fopen("userdata.csv", "w");
        fclose(file);
    }
    if (!fileExists("purchase_history.csv")) {
        FILE *file = fopen("purchase_history.csv", "w");
        fclose(file);
    }
    if (!fileExists("product.csv")) {
        FILE *file = fopen("product.csv", "w");
        fprintf(file, "%d,%s,%.2f,%d\n",1,"Dove",35.00,10);
        fprintf(file, "%d,%s,%.2f,%d\n",2,"Dairy milk",10.00,10);
        fprintf(file, "%d,%s,%.2f,%d\n",3,"Curd",10.00,15);
        fprintf(file, "%d,%s,%.2f,%d\n",4,"Chips",15.00,15);
        fprintf(file, "%d,%s,%.2f,%d\n",5,"Biscuit",40.00,10);

        fclose(file);
    }
}


int main() {
    
    printf("**************************WELCOME TO ORDER MANAGEMENT**************************\n\n");

    int option;
    createFiles();
    do {
        printf("\nIf you don't have an account, please register.\n\n");   
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Enter your choice:");
        scanf("%d", &option);

        switch (option) {
            case 1:
                login();
                break;
            case 2:
                registration(); // Corrected function name
                break;
            case 3:
                exit(0);
                break;
            default:
                printf("Invalid operation.\n");
                break;
        }
    } while (option != 3);

    return 0;
}
