#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "customerlogin.h"

#define MAX_ORDERS 100
#define MAX_ITEMS 100
#define MAX_USERS 100
#define ITEMS_FILENAME "product.csv"
#define USERS_FILENAME "purchase_history.csv"



typedef struct {
    int id;
    char name[101];
    float price;
    int availability;
    // float discount;
} Item;

typedef struct {
    int id;
    int itemCount;
    int itemIds[MAX_ITEMS];
    char itemNames[MAX_ITEMS][101];
    float itemPrices[MAX_ITEMS];
    float totalPrice[MAX_ITEMS];
} User;

typedef struct {
    int id;
    int userId;
    int itemId;
    char itemName[101]; 
    int totalPrice;
} Order;


int readItems(Item items[]) {
    FILE *file = fopen(ITEMS_FILENAME, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", ITEMS_FILENAME);
        return -1;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%[^,],%f,%d",
               &items[count].id,
               items[count].name,
               &items[count].price,
               &items[count].availability);
        count++;
    }

    fclose(file);
    return count;
}

void writeItems(Item items[], int itemCount) {
    FILE *file = fopen(ITEMS_FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", ITEMS_FILENAME);
        return;
    }

    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%d,%s,%.2f,%d\n",
                items[i].id,
                items[i].name,
                items[i].price,
                items[i].availability);
    }

    fclose(file);
}

int readUsers(User users[]) {
    FILE *file = fopen("userdata.csv", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d",
               &users[count].id);
        count++;
    }

    fclose(file);
    return count;
}

void writeUsers(User users[], int userCount) {
    FILE *file = fopen(USERS_FILENAME, "a+");
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", USERS_FILENAME);
        return;
    }

    for (int i = 0; i < userCount; i++) {
        for (int j = 0; j < users[i].itemCount; j++) {
            fprintf(file, "%d,%d,%s,%.2f\n", users[i].id, users[i].itemIds[j], users[i].itemNames[j], users[i].totalPrice[j]);
        }
        users[i].itemCount = 0;

        // fprintf(file, "\n");
    }

    fclose(file);
}

// Function to place an order
void placeOrder(int userId) {
    Item items[MAX_ITEMS];
    int itemCount = readItems(items);
    if (itemCount == -1) {
        printf("Error reading item file.\n");
        return;
    }

    User users[MAX_USERS];
    int userCount = readUsers(users);
    if (userCount == -1) {
        printf("Error reading user file.\n");
        return;
    }

    int itemId, quantity;
    char choice;

    do {
        int foundUser = 0;
        int userIndex = 0;
        for (int i = 0; i < userCount; i++) {
            if (users[i].id == userId) {
                foundUser = 1;
                userIndex = i;
                break;
            }
        }

        printf("\nAvailable Items:\n");
        printf("\nID\tName\t\tPrice\tAvailability\n"); 
        for (int i = 0; i < itemCount; i++) {
            printf("%d\t%-10s\t%.2f\t%d\n", items[i].id, items[i].name, items[i].price, items[i].availability); // Adjusted format strings
        }

        if (!foundUser) {
            printf("\nUser with ID %d not found.\n", userId);
            continue;
        }

        float totalAmount = 0.0; 

        printf("\nEnter item ID to order: ");
        scanf("%d", &itemId);

        int foundItem = 0;
        for (int i = 0; i < itemCount; i++) {
            char coupon[20];
            if (items[i].id == itemId) {
                foundItem = 1;
                printf("Enter quantity: ");
                scanf("%d", &quantity);

                if (items[i].availability >= quantity) {
                    items[i].availability -= quantity;
                    users[userIndex].itemIds[users[userIndex].itemCount] = itemId;
                    strcpy(users[userIndex].itemNames[users[userIndex].itemCount], items[i].name); 
                    totalAmount += items[i].price * quantity; 
                    printf("\nDo you have a coupon? (Y/N): ");
                    scanf(" %c", &choice);
                    if (choice == 'Y' || choice == 'y') {
                        printf("Enter coupon code: ");
                        scanf("%s", coupon);

                        if (strcmp(coupon, "PROMO10") == 0) {
                            totalAmount *= 0.9; 
                        } else if (strcmp(coupon, "DISCOUNT10") == 0) {
                            totalAmount -= 10.0; 
                        } else {
                            printf("\nInvalid coupon. No discount applied.\n");
                        }
                    }
                    users[userIndex].totalPrice[users[userIndex].itemCount] = totalAmount;
                    users[userIndex].itemCount++;
                    printf("\nOrder placed successfully!\n");
                } else {
                    printf("\nInsufficient availability for this item.\n");
                }
                break;
            }
        }


        if (!foundItem) {
            printf("\nItem with ID %d not found.\n", itemId);
        }

        printf("\nDo you want to add another item for this user? (Y/N): ");
        scanf(" %c", &choice); 

    } while (choice == 'Y' || choice == 'y');


    writeItems(items, itemCount);
    writeUsers(users, userCount);
}


void viewOrderHistory(int userId) {
    FILE *file = fopen(USERS_FILENAME, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", USERS_FILENAME);
        return;
    }

    printf("\n\nUser Order History:\n");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);

        if (id == userId) {
            printf("%s", line); // Print the line if it matches the user ID
        }
    }

   fclose(file);
}




void customerlogin(const char* username) {
    int userId; // Uncomment this line
    FILE *file1;
    file1 = fopen("userdata.csv", "r");
    if (file1 == NULL) {
        printf("Error in the DB\n");
    }

    char line[1024];
    while (fgets(line, sizeof(line), file1) != NULL) {
        char *token = strtok(line, ",");
        int id = atoi(token);
        token = strtok(NULL, ",");
        char *name = token;
        if (strcmp(name, username) == 0) {
            userId=id;
        }
    }

    fclose(file1);
    char choice;
    int userIndex; 
    do {
        printf("\nCustomer Menu:\n");
        printf("1. Place Order\n");
        printf("2. View Order History\n");
        printf("3. Logout\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                placeOrder(userId);
                break;
            case '2':
                viewOrderHistory(userId);
                break;
            case '3':
                printf("\nLogging out.\n");
                return;
            default:
                printf("\nInvalid choice. Please try again.\n");
                break;
        }
    } while (1);
}