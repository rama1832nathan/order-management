#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "login.h"
#include "adminlogin.h" 
#include "customerlogin.h"

void decrypt(char password[], int key) {
    for (int i = 0; i < strlen(password); ++i) {
        password[i] = password[i] ^ key; 
    }
}

int login(void) {
    char username[101];
    char password[101];
    int isAdmin = 0,userFound=0; 

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    int i = 0;
    char ch;
    while ((ch = _getch()) != 13) {
        if (ch == '\b' && i > 0) { 
            printf("\b \b"); 
            i--;
        } else if (ch != '\b') { 
            password[i] = ch;
            i++;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");


    FILE *file;
    file = fopen("userdata.csv", "r");
    if (file == NULL) {
        printf("Error in the DB\n");
        return 0;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, ",");
        int id = atoi(token);
        token = strtok(NULL, ",");
        char *name = token;
        token = strtok(NULL, ",");
        char *encrypted_password = token;
        token = strtok(NULL, ",");
        char *role = token;

        role[strcspn(role, "\n")] = 0;

        char decrypted_password[101];
        strcpy(decrypted_password, encrypted_password);
        decrypt(decrypted_password, 0xFACA);
        // printf("%s %s",name,role); 
        
        if (strcmp(name, username) == 0 && strcmp(decrypted_password, password) == 0) {
            userFound = 1;
            if (strcmp(role, "admin") == 0) {
                isAdmin = 1; 
                printf("\nAdmin user found: %s\n", username);
                break; 
            }
        }
    }

    fclose(file);

    if(userFound){
        if (isAdmin) {
            printf("\nLogin successful as admin. Welcome, %s!\n", username);
            adminlogin();
            return 1;
        } 
        else {
        printf("\nLogin successful as customer. Welcome, %s!\n", username);
        customerlogin(username);
        return 1;
        }
    }
    else{
        printf("\nUsername not found Please register.....\n");
        return 0;
    }

}
