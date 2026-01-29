#include<stdio.h>
#include<string.h>
#include<time.h>
#include "bill.h"

int id_check(int id) {
    struct Product p;
    FILE *fp = fopen(PROD_FILE, "rb");
    if (!fp)
        return 0;

    while (fread(&p, sizeof(p), 1, fp)) {
        if (id == p.id) {
            printf("\nProduct already exists!\n\n");
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void sort_inventory() {
    struct Product products[100], temp;
    FILE *fp = fopen(PROD_FILE, "rb");
    int i, j,count = 0;

    if (!fp) {
        printf("\nNo products to print!\n\n");
        return;
    }

    while (fread(&products[count], sizeof(struct Product), 1, fp))
        count++;
    fclose(fp);

    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (strcmp(products[j].name, products[j + 1].name) > 0) {
                temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }

    for (i = 0; i < count; i++) {
        printf("%-6d %-20s %-10.2f\n", 
            products[i].id, products[i].name, products[i].price);
    }
    printf("\n");
}

void add_product() {
    FILE *fp = fopen(PROD_FILE, "ab");
    struct Product p;

    printf("\nProduct ID: ");
    scanf("%d", &p.id);

    printf("Product Name: ");
    scanf(" %[^\n]", p.name);

    printf("Price: ");
    scanf("%f", &p.price);

    fwrite(&p, sizeof(p), 1, fp);
    fclose(fp);

    printf("\nProduct added successfully!\n\n");
}

void view_products() {
    printf("\n%-6s %-20s %-10s %-10s %s\n",
           "ID", "Name", "Qty", "Price", "Value");
    printf("---------------------------------------------\n");

    sort_inventory();
}

int load_product(int id, struct Cart *c) {
    FILE *fp = fopen(PROD_FILE, "rb");
    struct Product p;

    if (!fp) {
        printf("\nFile error!\n\n");
        fclose(fp);
        return 0;
    }

    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.id == id) {
            c->id = p.id;
            strcpy(c->name, p.name);
            c->price = p.price;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void generate_bill() {
    struct Cart cart[20];
    int id, qty, count = 0;
    float subtotal = 0, tax, discount, grandtotal;

    while (1) {
        printf("\nEnter product ID (0 to finish): ");
        scanf("%d", &id);
        if (id == 0) break;

        if (!load_product(id, &cart[count])) {
            printf("\nProduct not found!\n\n");
            continue;
        }

        printf("Enter quantity: ");
        scanf("%d", &qty);

        cart[count].quantity = qty;
        cart[count].total = qty * cart[count].price;
        subtotal += cart[count].total;
        count++;
    }

    discount = subtotal * 0.10;
    tax = (subtotal - discount) * 0.05;
    grandtotal = subtotal - discount + tax;

    FILE *fp = fopen("bills.txt", "w");

    fprintf(fp, "======= SUPERMARKET BILL =======\n");
    fprintf(fp, "%-20s %-5s %-10s\n", "Item", "Qty", "Total");
    fprintf(fp, "--------------------------------\n");

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%-20s %-5d %-10.2f\n",
                cart[i].name, cart[i].quantity, cart[i].total);
    }

    fprintf(fp, "--------------------------------\n");
    fprintf(fp, "Subtotal: %.2f\n", subtotal);
    fprintf(fp, "Discount: %.2f\n", discount);
    fprintf(fp, "Tax: %.2f\n", tax);
    fprintf(fp, "GRAND TOTAL: %.2f\n", grandtotal);

    fclose(fp);

    printf("\nBill generated: bill.txt\n\n");

    FILE *fps = fopen(SALES_FILE, "ab");
    struct Sales s;

    time_t now = time(NULL);
    struct tm *tm = localtime(&now);

    sprintf(s.date_time, "%02d-%02d-%04d %02d:%02d",
            tm->tm_mday, tm->tm_mon + 1,
            tm->tm_year + 1900,
            tm->tm_hour, tm->tm_min);

    s.amount = grandtotal;
    fwrite(&s, sizeof(s), 1, fps);
    fclose(fps);
}

void daily_report() {
    FILE *fp = fopen(SALES_FILE, "rb");
    struct Sales s;
    float total = 0;

    while (fread(&s, sizeof(s), 1, fp)) {
        total += s.amount;
    }

    fclose(fp);
    printf("\nTotal Sales Today: %.2f\n\n", total);
}