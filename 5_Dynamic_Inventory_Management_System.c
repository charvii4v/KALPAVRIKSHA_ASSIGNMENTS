#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    int productID;
    int quantity;
    char productName[50];
    float price;
};


struct Product* addProduct(struct Product* inventory, int* totalProducts) {
    (*totalProducts)++;

    struct Product* updatedInventory = (struct Product*)realloc(inventory, (*totalProducts) * sizeof(struct Product));
    if (updatedInventory == NULL) {
        printf("Memory allocation failed while adding a new product.\n");
        free(inventory);
        exit(1);
    }

    inventory = updatedInventory;

    int newIndex = *totalProducts - 1;
    printf("\nEnter details for the new product:\n");

    printf("Enter Product ID: ");
    scanf("%d", &inventory[newIndex].productID);

    printf("Enter Product Name: ");
    scanf("%49s", inventory[newIndex].productName);

    printf("Enter Product Quantity: ");
    scanf("%d", &inventory[newIndex].quantity);

    printf("Enter Product Price: ");
    scanf("%f", &inventory[newIndex].price);

    printf(" Product added successfully!\n");
    return inventory;
}


void displayAllProducts(const struct Product* inventory, int totalProducts) {
    if (totalProducts == 0) {
        printf("No products available in inventory.\n");
        return;
    }

    printf("%-10s %-20s %-10s %-10s\n", "Product ID", "Product Name", "Quantity", "Price");
    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        printf("%-10d %-20s %-10d %-10.2f\n",
               inventory[productIndex].productID,
               inventory[productIndex].productName,
               inventory[productIndex].quantity,
               inventory[productIndex].price);
    }
}


void updateProductQuantity(struct Product* inventory, int totalProducts) {
    int targetID;
    int productFound = 0;

    printf("Enter Product ID to update quantity: ");
    scanf("%d", &targetID);

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        if (inventory[productIndex].productID == targetID) {
            printf("Enter new quantity: ");
            scanf("%d", &inventory[productIndex].quantity);
            printf(" Quantity updated successfully!\n");
            productFound = 1;
            break;
        }
    }

    if (!productFound)
        printf(" Product not found.\n");
}


void searchProductByID(const struct Product* inventory, int totalProducts) {
    int targetID;
    int productFound = 0;

    printf("Enter Product ID to search: ");
    scanf("%d", &targetID);

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        if (inventory[productIndex].productID == targetID) {
            printf("Product Found:\n");
            printf("ID: %d | Name: %s | Quantity: %d | Price: %.2f\n",
                   inventory[productIndex].productID,
                   inventory[productIndex].productName,
                   inventory[productIndex].quantity,
                   inventory[productIndex].price);
            productFound = 1;
            break;
        }
    }

    if (!productFound)
        printf("Product not found.\n");
}


void searchProductByName(const struct Product* inventory, int totalProducts) {
    char searchName[50];
    int productFound = 0;

    printf("Enter name (or part of name) to search: ");
    scanf("%49s", searchName);

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        if (strstr(inventory[productIndex].productName, searchName)) {
            printf("Match Found: %d | %s | Qty: %d | Price: %.2f\n",
                   inventory[productIndex].productID,
                   inventory[productIndex].productName,
                   inventory[productIndex].quantity,
                   inventory[productIndex].price);
            productFound = 1;
        }
    }

    if (!productFound)
        printf("No products found matching the name.\n");
}


void searchProductsByPriceRange(const struct Product* inventory, int totalProducts) {
    float minPrice, maxPrice;
    int productFound = 0;

    printf("Enter price range (min max): ");
    scanf("%f %f", &minPrice, &maxPrice);

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        if (inventory[productIndex].price >= minPrice && inventory[productIndex].price <= maxPrice) {
            printf("Product: %d | %s | Qty: %d | Price: %.2f\n",
                   inventory[productIndex].productID,
                   inventory[productIndex].productName,
                   inventory[productIndex].quantity,
                   inventory[productIndex].price);
            productFound = 1;
        }
    }

    if (!productFound)
        printf("No products found in this price range.\n");
}


struct Product* deleteProductByID(struct Product* inventory, int* totalProducts) {
    int targetID;
    int productFound = 0;

    printf("Enter Product ID to delete: ");
    scanf("%d", &targetID);

    for (int productIndex = 0; productIndex < *totalProducts; productIndex++) {
        if (inventory[productIndex].productID == targetID) {
            for (int shiftIndex = productIndex; shiftIndex < *totalProducts - 1; shiftIndex++) {
                inventory[shiftIndex] = inventory[shiftIndex + 1];
            }

            (*totalProducts)--;

            struct Product* updatedInventory = (struct Product*)realloc(inventory, (*totalProducts) * sizeof(struct Product));
            if (updatedInventory == NULL && *totalProducts > 0) {
                printf("Memory reallocation failed while deleting the product.\n");
                free(inventory);
                exit(1);
            }

            inventory = updatedInventory;
            printf("Product deleted successfully!\n");
            productFound = 1;
            break;
        }
    }

    if (!productFound)
        printf("Product not found.\n");

    return inventory;
}

int main() {
    printf(" Welcome to Charvi’s Inventory Management System \n");

    int totalProducts = 0;
    printf("Enter the number of initial products: ");
    scanf("%d", &totalProducts);

    struct Product* inventory = (struct Product*)calloc(totalProducts, sizeof(struct Product));
    if (inventory == NULL) {
        printf("Memory allocation failed while initializing inventory.\n");
        exit(1);
    }

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        printf("\nEnter details for product %d:\n", productIndex + 1);

        printf("Enter Product ID: ");
        scanf("%d", &inventory[productIndex].productID);

        printf("Enter Product Name: ");
        scanf("%49s", inventory[productIndex].productName);

        printf("Enter Product Quantity: ");
        scanf("%d", &inventory[productIndex].quantity);

        printf("Enter Product Price: ");
        scanf("%f", &inventory[productIndex].price);
    }

    int userChoice;
    do {
        printf("\n--- Inventory Menu ---\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Products by Price Range\n");
        printf("7. Delete Product by ID\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);

        switch (userChoice) {
            case 1:
                inventory = addProduct(inventory, &totalProducts);
                break;
            case 2:
                displayAllProducts(inventory, totalProducts);
                break;
            case 3:
                updateProductQuantity(inventory, totalProducts);
                break;
            case 4:
                searchProductByID(inventory, totalProducts);
                break;
            case 5:
                searchProductByName(inventory, totalProducts);
                break;
            case 6:
                searchProductsByPriceRange(inventory, totalProducts);
                break;
            case 7:
                inventory = deleteProductByID(inventory, &totalProducts);
                break;
            case 8:
                printf("Exiting program... Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (userChoice != 8);

    free(inventory);
    return 0;
}
