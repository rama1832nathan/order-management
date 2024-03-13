#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> 
#include "registration.h"
// #include "adminlogin.h"

void encrypt(char password[], int key) {
    for (int i = 0; i < strlen(password); ++i) {
        password[i] = password[i] ^ key; 
    }
}

typedef struct {
    int id;
    char name[101];
    char password[101];
    char role[101];
} User;

void registration(void) {
    FILE *file;
    file = fopen("userdata.csv", "a+"); 

    if (file == NULL) {
        printf("Error in the DB..\n");
        return;
    }

    int last_id = 0;
    char line[1024];
    fseek(file, 0, SEEK_SET); 
    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, ",");
        last_id = atoi(token);
    }


    int next_id = last_id + 1;


    User user;
    printf("Enter username: ");
    scanf("%s", user.name);
    printf("Enter password: ");
    int i = 0;
    char ch;
    while ((ch = _getch()) != 13) {
        if (ch == '\b' && i > 0) { 
            printf("\b \b"); 
            i--;
        } else if (ch != '\b') { 
            user.password[i] = ch;
            i++;
            printf("*");
        }
    }
    user.password[i] = '\0';
    printf("\n");


    encrypt(user.password, 0xFACA); 

    printf("\n****************************************************\n");
    printf("\nIf you are a customer enter customer as your role\n");
    printf("\nIf you are a admin enter admin as your role\n");
    printf("\nEnter role: ");
    scanf("%s", user.role);


    fprintf(file, "%d,%s,%s,%s\n", next_id, user.name, user.password, user.role);
    printf("\nUser registered successfully with ID: %d\n", next_id);

    fclose(file);
}
