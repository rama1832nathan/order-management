#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include "adminlogin.h"
#include "registration.h"

#define ITEMS_FILENAME "product.csv"
#define MAX_PURCHASES 1001

#define MAX_ITEMS 100

typedef struct {
    int id;
    char name[101];
    float price;
    int availability;
    // float discount;
} Item;

typedef struct {
    int userId;
    int itemId;
    char itemName[101];
} Purchase;

int findTopSellingItem() {
    FILE *file = fopen("purchase_history.csv", "r");
    if (file == NULL) {
        printf("Error opening file purchasehistory.csv.\n");
        return -1;
    }

    Purchase purchases[MAX_PURCHASES];
    int purchaseCount = 0;

    while (fscanf(file, "%d,%d,%100[^\n]", &purchases[purchaseCount].userId,
                  &purchases[purchaseCount].itemId,
                  purchases[purchaseCount].itemName) == 3) {
        purchaseCount++;
    }

    fclose(file);

    int itemCount[MAX_ITEMS] = {0};
    for (int i = 0; i < purchaseCount; i++) {
        itemCount[purchases[i].itemId]++;
    }

    int maxSalesIndex = 0;
    for (int i = 1; i < MAX_ITEMS; i++) {
        if (itemCount[i] > itemCount[maxSalesIndex]) {
            maxSalesIndex = i;
        }
    }

    // Return the ID of the top-selling item
    return maxSalesIndex;
}



int readitems(Item items[]) {
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


void addItem() {
    Item newItem;
    Item items[MAX_ITEMS];
    int itemcount = readitems(items);

    printf("Enter product ID: ");
    scanf("%d", &newItem.id);
    getchar(); 

    for(int i=0;i<itemcount;i++){
        if(items[i].id==newItem.id){
            printf("\nThis product id already exists...\n");
            return;
        }
    }

    printf("Enter product name: ");
    fgets(newItem.name, 101, stdin);
    newItem.name[strcspn(newItem.name, "\n")] = 0; 

    printf("Enter product price: ");
    scanf("%f", &newItem.price);

    printf("Enter product availability: ");
    scanf("%d", &newItem.availability);

    FILE *file = fopen(ITEMS_FILENAME, "a");
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", ITEMS_FILENAME);
        return;
    }

    fprintf(file, "%d,%s,%.2f,%d\n",
            newItem.id,
            newItem.name,
            newItem.price,
            newItem.availability);

    fclose(file);

    printf("\nProduct added successfully.\n");
}



void updateStock(Item items[], int itemCount, int ids[], int idCount) {

    for (int i = 0; i < itemCount; i++) {
        for (int j = 0; j < idCount; j++) {
            if (items[i].id == ids[j]) {
                items[i].availability = 10;
                break;
            }
        }
    }

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
    printf("\nSuccesfully Updated......");
}


void checkStock(){
    Item items[MAX_ITEMS];
    int itemCount = readitems(items);
    if (itemCount == -1) {
        printf("Error reading item file.\n");
        return;
    }

    int count = 0;
    int ids[MAX_ITEMS]; 
    for (int i = 0; i < itemCount; i++) {
        if (items[i].availability < 3) {
            ids[count] = items[i].id; 
            count++;
        }
    }

    if (count == 0) {
        printf("\nNo items with availability less than 3.\n");
    } else {
        printf("\nItems with availability less than 3:\n");
        for (int i = 0; i < count; i++) {
            printf("\nID: %d, Name: %s, Availability: %d\n", items[ids[i]].id, items[ids[i]].name, items[ids[i]].availability);
        }
        updateStock(items, itemCount, ids, count); 
    }
}



void adminlogin(void){
    printf("\n\nEnter the choice....\n");
    printf("\n1.StockList Item\n2.Update item\n3.Add item\n4.Add New Customer.\n5.Topselling Item\n6.Logout");
    int option;
    int topSellingItemId;
    do{
        printf("\nEnter the option:");
        scanf("%d",&option);
        switch (option)
        {
        case 1:
            checkStock();
            break;
        case 2:{
            Item items[MAX_ITEMS];
            int itemCount = readitems(items);
            if (itemCount != -1) {
                int ids[MAX_ITEMS]; 
                for (int i = 0; i < itemCount; i++) {
                    ids[i] = items[i].id; 
                }
                updateStock(items, itemCount, ids, itemCount);
            }
            break;
        }
        case 3:
            addItem();
            break;
        case 4:
            registration();
            break;
        case 5:
            topSellingItemId = findTopSellingItem();
            if (topSellingItemId != -1) {
                printf("\nTop Selling Item ID: %d\n", topSellingItemId);
            }
            break;
        case 6:
            break;
        default:
            printf("\nInvalid option...");
            break;
        }
    }while(option!=6);
}