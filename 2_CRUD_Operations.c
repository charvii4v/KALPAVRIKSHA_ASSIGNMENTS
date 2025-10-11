#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "users.txt"

typedef struct 

{
    int id;
    char name[50];
    int age;
} 
User;

void createUser();
void readUsers();
void updateUser();
void deleteUser();
void ensureFileExists();

int main() 
{
    int option;
    ensureFileExists();

    while (1)
     {
        printf("\nUSER MANAGEMENT SYSTEM\n");

        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");

        printf("Enter your choice (1-5): ");

        scanf("%d", &option);

        switch (option) 
        {
            case 1:
                createUser();
                break;
            case 2:
                readUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

void ensureFileExists() 
{
    FILE *filePtr = fopen(FILE_NAME, "a");
    if (filePtr == NULL)
     {
        printf("Error creating file!\n");
        exit(1);
    }
    fclose(filePtr);
}

void createUser()
 {
    User newUser, existingUser;
    FILE *filePtr = fopen(FILE_NAME, "r");
    if (filePtr == NULL)
     {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &newUser.id);

    while (fscanf(filePtr, "%d %s %d", &existingUser.id, existingUser.name, &existingUser.age) != EOF) {
        if (existingUser.id == newUser.id) {
            printf("Error: ID %d already exists!\n", newUser.id);
            fclose(filePtr);
            return;
        }
    }
    fclose(filePtr);

    filePtr = fopen(FILE_NAME, "a");
    if (filePtr == NULL)
     {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", newUser.name);
    printf("Enter Age: ");
    scanf("%d", &newUser.age);

    fprintf(filePtr, "%d %s %d\n", newUser.id, newUser.name, newUser.age);
    fclose(filePtr);
    printf("User added successfully!\n");
}

void readUsers()
 {
    User user;
    FILE *filePtr = fopen(FILE_NAME, "r");
    if (filePtr == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }

    printf("\nRecords of the users in the system:\n");
    int isUserFound = 0;

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) != EOF) 
    {
        printf("ID: %d | Name: %s | Age: %d\n", user.id, user.name, user.age);
        isUserFound = 1;
    }

    if (!isUserFound) 
    {
        printf("No users found.\n");
    }

    fclose(filePtr);
}

void updateUser() 
{
    User user;
    int id, isUserFound = 0;

    FILE *filePtr = fopen(FILE_NAME, "r");
    FILE *temporaryFilePtr = fopen("temp.txt", "w");

    if (filePtr == NULL || temporaryFilePtr == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter ID of user to update: ");
    scanf("%d", &id);

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) != EOF)
     {
        if (user.id == id) 
        {
            isUserFound = 1;

            printf("Enter new Name: ");
            scanf(" %[^\n]", user.name);
            printf("Enter new Age: ");
            scanf("%d", &user.age);
        }
        fprintf(temporaryFilePtr, "%d %s %d\n", user.id, user.name, user.age);
    }

    fclose(filePtr);
    fclose(temporaryFilePtr);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (isUserFound)
        printf("User updated successfully!\n");
    else
        printf("Error: User with ID %d not found!\n", id);
}

void deleteUser() {
    User user;
    int id, isUserFound = 0;

    FILE *filePtr = fopen(FILE_NAME, "r");
    FILE *temporaryFilePtr = fopen("temp.txt", "w");

    if (filePtr == NULL || temporaryFilePtr == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter ID of user to delete: ");
    scanf("%d", &id);

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) != EOF)
     {
        if (user.id == id)
         {
            isUserFound = 1;
            continue;
        }
        fprintf(temporaryFilePtr, "%d %s %d\n", user.id, user.name, user.age);
    }

    fclose(filePtr);
    fclose(temporaryFilePtr);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (isUserFound)
        printf("User deleted successfully!\n");
    else
        printf("Error: User with ID %d not found!\n", id);
}
