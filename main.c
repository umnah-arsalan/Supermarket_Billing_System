#include<stdio.h>
#include<stdlib.h>
#include "bill.h"

int main() {
    int choice;

    while (1) {
        printf("1. Add Product");
        printf("\n2. View products.");
        printf("\n3. Generate Bill");
        printf("\n4. Daily Sales Report");
        printf("\n5. Exit\n\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_product(); break;
            case 2: view_products(); break;
            case 3: generate_bill(); break;
            case 4: daily_report(); break;
            case 5: exit(0);
        }
    }
}