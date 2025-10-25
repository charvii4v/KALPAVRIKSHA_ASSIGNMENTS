#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    int productID;
    int quantity;
    char productName[50];
    float price;
};

// Function to add new product
struct Product* addProduct(struct Product* inventory, int *NumberOfProducts) {
    (*NumberOfProducts)++;  // increment the count

    inventory = (struct Product*)realloc(inventory, (*NumberOfProducts) * sizeof(struct Product));
    int i = *NumberOfProducts - 1;

    printf("\nEnter the details of the new product:\n");

    printf("Enter Product ID: ");
    scanf("%d", &inventory[i].productID);

    printf("Enter Product Name: ");
    scanf("%s", inventory[i].productName);

    printf("Enter Product Quantity: ");
    scanf("%d", &inventory[i].quantity);

    printf("Enter Product Price: ");
    scanf("%f", &inventory[i].price);

    printf("Product added successfully!\n");
    return inventory;
}

// Function to view all products
void ViewAllProducts(struct Product* inventory, int *NumberOfProducts) {
    if (*NumberOfProducts == 0) {
        printf("No products available.\n");
        return;
    }

    printf("\nProduct ID\tProduct Name\tQuantity\tPrice\n");
    for (int i = 0; i < *NumberOfProducts; i++) {
        printf("%d\t\t%s\t\t%d\t\t%.2f\n",
               inventory[i].productID,
               inventory[i].productName,
               inventory[i].quantity,
               inventory[i].price);
    }
}

// Function to update quantity
void UpdateQuantityProduct(struct Product* inventory, int *NumberOfProducts) {
    int id, found = 0;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);

    for (int i = 0; i < *NumberOfProducts; i++) {
        if (inventory[i].productID == id) {
            printf("Enter new quantity: ");
            scanf("%d", &inventory[i].quantity);
            printf("Quantity updated!\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Product not found.\n");
}

// Search by ID
void SearchProductByID(struct Product* inventory, int NumberOfProducts) {
    int id, found = 0;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);

    for (int i = 0; i < NumberOfProducts; i++) {
        if (inventory[i].productID == id) {
            printf("Product found: %d %s %d %.2f\n",
                   inventory[i].productID,
                   inventory[i].productName,
                   inventory[i].quantity,
                   inventory[i].price);
            found = 1;
            break;
        }
    }
    if (!found) printf("Product not found.\n");
}

// Search by Name (partial match)
void SearchProductByName(struct Product* inventory, int NumberOfProducts) {
    char name[50];
    int found = 0;
    printf("Enter name to search: ");
    scanf("%s", name);

    for (int i = 0; i < NumberOfProducts; i++) {
        if (strstr(inventory[i].productName, name)) {
            printf("Product found: %d %s %d %.2f\n",
                   inventory[i].productID,
                   inventory[i].productName,
                   inventory[i].quantity,
                   inventory[i].price);
            found = 1;
        }
    }
    if (!found) printf("No matching products found.\n");
}

// Search by Price Range
void SearchProductsByPriceRange(struct Product* inventory, int NumberOfProducts) {
    float low, high;
    int found = 0;
    printf("Enter price range (low high): ");
    scanf("%f %f", &low, &high);

    for (int i = 0; i < NumberOfProducts; i++) {
        if (inventory[i].price >= low && inventory[i].price <= high) {
            printf("Product: %d %s %.2f %d\n",
                   inventory[i].productID,
                   inventory[i].productName,
                   inventory[i].price,
                   inventory[i].quantity);
            found = 1;
        }
    }
    if (!found) printf("No products found in this price range.\n");
}

// Delete product by ID
struct Product* DeleteProductByID(struct Product* inventory, int *NumberOfProducts) {
    int id, found = 0;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *NumberOfProducts; i++) {
        if (inventory[i].productID == id) {
            // Shift remaining products
            for (int j = i; j < *NumberOfProducts - 1; j++)
                inventory[j] = inventory[j + 1];

            (*NumberOfProducts)--;
            inventory = (struct Product*)realloc(inventory, (*NumberOfProducts) * sizeof(struct Product));
            printf("Product deleted successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Product not found.\n");

    return inventory;
}

int main() {
    printf("Welcome to the Inventory Menu of Charvi\n");

    int NumberOfProducts;
    printf("Enter the Number of Products: ");
    scanf("%d", &NumberOfProducts);

    struct Product* inventory = (struct Product*)calloc(NumberOfProducts, sizeof(struct Product));

    for (int i = 0; i < NumberOfProducts; i++) {
        printf("\nEnter details of product %d\n", i + 1);
        printf("Enter Product ID: ");
        scanf("%d", &inventory[i].productID);
        printf("Enter Product Name: ");
        scanf("%s", inventory[i].productName);
        printf("Enter Product Quantity: ");
        scanf("%d", &inventory[i].quantity);
        printf("Enter Product Price: ");
        scanf("%f", &inventory[i].price);
    }

    int choice;
    do {
        printf("\n--- Inventory Menu ---\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity of a Product\n");
        printf("4. Search for a Product by ID\n");
        printf("5. Search for Products by Name (partial match allowed)\n");
        printf("6. Search for Products by Price Range\n");
        printf("7. Delete a Product by ID\n");
        printf("8. Exit the Program\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inventory = addProduct(inventory, &NumberOfProducts);
                break;
            case 2:
                ViewAllProducts(inventory, &NumberOfProducts);
                break;
            case 3:
                UpdateQuantityProduct(inventory, &NumberOfProducts);
                break;
            case 4:
                SearchProductByID(inventory, NumberOfProducts);
                break;
            case 5:
                SearchProductByName(inventory, NumberOfProducts);
                break;
            case 6:
                SearchProductsByPriceRange(inventory, NumberOfProducts);
                break;
            case 7:
                inventory = DeleteProductByID(inventory, &NumberOfProducts);
                break;
            case 8:
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 8);

    free(inventory);
    return 0;
}
