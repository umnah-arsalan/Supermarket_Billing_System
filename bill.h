#ifndef BILL_H
#define BILL_H

#define PROD_FILE "products.dat"
#define SALES_FILE "sales.dat"

struct Product {
    int id;
    char name[50];
    float price;
};

struct Cart {
    int id;
    char name[50];
    float price;
    int quantity;
    float total;
};

struct Sales {
    float amount;
    char date_time[60];
};

int id_check(int id);
void sort_inventory(void);
void add_product(void);
void view_products(void);
int load_product(int id, struct Cart *c);
void generate_bill(void);
void daily_report(void);

#endif