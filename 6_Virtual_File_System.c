#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define TOTAL_BLOCKS 1024
#define MAX_NAME_LENGTH 50
#define MAX_BLOCK_POINTERS 100



typedef struct FreeBlock {
    int blockIndex;
    struct FreeBlock *previous;
    struct FreeBlock *next;
} FreeBlock;

typedef struct FileNode {
    char name[MAX_NAME_LENGTH];
    int isDirectory;
    struct FileNode *parent;
    struct FileNode *next;
    struct FileNode *previous;
    struct FileNode *children; 
    int allocatedBlockCount;
    int blockPointers[MAX_BLOCK_POINTERS];
} FileNode;


// Global Variables


char virtualDisk[TOTAL_BLOCKS][BLOCK_SIZE];
FreeBlock *freeBlockHead = NULL;
FreeBlock *freeBlockTail = NULL;
FileNode *rootDirectory = NULL;
FileNode *currentWorkingDirectory = NULL;


// Function Declarations


void initializeVirtualFileSystem();

void insertChildNode(FileNode *parentDirectory, FileNode *newNode) {
    if (parentDirectory->children == NULL) {
       
        parentDirectory->children = newNode;
        newNode->next = newNode;
        newNode->previous = newNode;
    } else {
       
        FileNode *lastNode = parentDirectory->children->previous;
        lastNode->next = newNode;
        newNode->previous = lastNode;
        newNode->next = parentDirectory->children;
        parentDirectory->children->previous = newNode;
    }
}

void initializeFreeBlockList();

void createDirectory(char *directoryName);
void removeDirectory(char *directoryName);
void changeDirectory(char *directoryName);
void listDirectoryContents();
void showCurrentPath();

void createFile(char *fileName);
void writeFile(char *fileName, char *content);
void readFile(char *fileName);
void deleteFile(char *fileName);

void displayDiskUsage();
void exitSystem();

FileNode *findFileNode(FileNode *directory, char *targetName);
void insertChildNode(FileNode *parentDirectory, FileNode *newNode);
int isDirectoryEmpty(FileNode *directory);
void freeFileBlocks(FileNode *fileNode);

FreeBlock *allocateFreeBlock();
void returnBlockToFreeList(int blockIndex);

void freeAllMemory(FileNode *directory);
void executeCommand(char *commandLine);


// Initialization


void initializeFreeBlockList() {
    for (int blockIndex = 0; blockIndex < TOTAL_BLOCKS; blockIndex++) {
        FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
        newBlock->blockIndex = blockIndex;
        newBlock->previous = freeBlockTail;
        newBlock->next = NULL;

        if (freeBlockTail != NULL)
            freeBlockTail->next = newBlock;

        freeBlockTail = newBlock;

        if (freeBlockHead == NULL)
            freeBlockHead = newBlock;
    }
}

void initializeVirtualFileSystem() {
    initializeFreeBlockList();

    rootDirectory = (FileNode *)malloc(sizeof(FileNode));
    strcpy(rootDirectory->name, "/");
    rootDirectory->isDirectory = 1;
    rootDirectory->parent = NULL;
    rootDirectory->next = rootDirectory->previous = rootDirectory;
    rootDirectory->children = NULL;
    rootDirectory->allocatedBlockCount = 0;

    currentWorkingDirectory = rootDirectory;

    printf("Compact VFS initialized successfully.\n");
    printf("Type 'exit' to quit.\n\n");
}


// Directory Operations


void createDirectory(char *directoryName) {
    if (findFileNode(currentWorkingDirectory, directoryName) != NULL) {
        printf("Error: Directory '%s' already exists.\n", directoryName);
        return;
    }

    FileNode *newDirectory = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newDirectory->name, directoryName);
    newDirectory->isDirectory = 1;
    newDirectory->parent = currentWorkingDirectory;
    newDirectory->children = NULL;
    newDirectory->allocatedBlockCount = 0;
    newDirectory->next = newDirectory->previous = newDirectory;

    insertChildNode(currentWorkingDirectory, newDirectory);

    printf("Directory '%s' created successfully.\n", directoryName);
}

void removeDirectory(char *directoryName) {
    FileNode *targetDirectory = findFileNode(currentWorkingDirectory, directoryName);
    if (targetDirectory == NULL || !targetDirectory->isDirectory) {
        printf("Error: Directory '%s' not found.\n", directoryName);
        return;
    }

    if (!isDirectoryEmpty(targetDirectory)) {
        printf("Error: Directory '%s' is not empty.\n", directoryName);
        return;
    }

    // Adjusting th4 circular links


    if (targetDirectory->next == targetDirectory)
        currentWorkingDirectory->children = NULL;
    else {
        if (currentWorkingDirectory->children == targetDirectory)
            currentWorkingDirectory->children = targetDirectory->next;
        targetDirectory->previous->next = targetDirectory->next;
        targetDirectory->next->previous = targetDirectory->previous;
    }

    free(targetDirectory);
    printf("Directory '%s' removed successfully.\n", directoryName);
}

int isDirectoryEmpty(FileNode *directory) {
    return directory->children == NULL;
}

void listDirectoryContents() {
    if (currentWorkingDirectory->children == NULL) {
        printf("(empty)\n");
        return;
    }

    FileNode *iterator = currentWorkingDirectory->children;
    do {
        printf("%s%s\n", iterator->name, iterator->isDirectory ? "/" : "");
        iterator = iterator->next;
    } while (iterator != currentWorkingDirectory->children);
}

void changeDirectory(char *directoryName) {
    if (strcmp(directoryName, "..") == 0) {
        if (currentWorkingDirectory->parent != NULL)
            currentWorkingDirectory = currentWorkingDirectory->parent;
        printf("Moved to %s\n", currentWorkingDirectory->name);
        return;
    }

    FileNode *targetDirectory = findFileNode(currentWorkingDirectory, directoryName);
    if (targetDirectory == NULL || !targetDirectory->isDirectory) {
        printf("Error: Directory '%s' not found.\n", directoryName);
        return;
    }

    currentWorkingDirectory = targetDirectory;
    printf("Moved to %s\n", currentWorkingDirectory->name);
}

void showCurrentPath() {
    FileNode *iterator = currentWorkingDirectory;
    char pathBuffer[1024] = "";
    char tempBuffer[1024];

    while (iterator != NULL) {
        strcpy(tempBuffer, "/");
        strcat(tempBuffer, iterator->name);
        strcat(tempBuffer, pathBuffer);
        strcpy(pathBuffer, tempBuffer);
        iterator = iterator->parent;
    }

    printf("%s\n", strcmp(pathBuffer, "//") == 0 ? "/" : pathBuffer);
}


// File Operations


void createFile(char *fileName) {
    if (findFileNode(currentWorkingDirectory, fileName) != NULL) {
        printf("Error: File '%s' already exists.\n", fileName);
        return;
    }

    FileNode *newFile = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newFile->name, fileName);
    newFile->isDirectory = 0;
    newFile->parent = currentWorkingDirectory;
    newFile->children = NULL;
    newFile->allocatedBlockCount = 0;
    newFile->next = newFile->previous = newFile;

    insertChildNode(currentWorkingDirectory, newFile);
    printf("File '%s' created successfully.\n", fileName);
}

void writeFile(char *fileName, char *content) {
    FileNode *targetFile = findFileNode(currentWorkingDirectory, fileName);
    if (targetFile == NULL || targetFile->isDirectory) {
        printf("Error: File '%s' not found.\n", fileName);
        return;
    }

    int contentSize = strlen(content);
    int requiredBlocks = (contentSize / BLOCK_SIZE) + 1;

    for (int block = 0; block < requiredBlocks; block++) {
        FreeBlock *allocatedBlock = allocateFreeBlock();
        if (allocatedBlock == NULL) {
            printf("Error: Disk full. Write failed.\n");
            return;
        }

        strncpy(virtualDisk[allocatedBlock->blockIndex],
                content + (block * BLOCK_SIZE), BLOCK_SIZE);
        targetFile->blockPointers[targetFile->allocatedBlockCount++] = allocatedBlock->blockIndex;
    }

    printf("Data written successfully (size=%d bytes).\n", contentSize);
}

void readFile(char *fileName) {
    FileNode *targetFile = findFileNode(currentWorkingDirectory, fileName);
    if (targetFile == NULL || targetFile->isDirectory) {
        printf("Error: File '%s' not found.\n", fileName);
        return;
    }

    for (int block = 0; block < targetFile->allocatedBlockCount; block++) {
        printf("%s", virtualDisk[targetFile->blockPointers[block]]);
    }
    printf("\n");
}

void deleteFile(char *fileName) {
    FileNode *targetFile = findFileNode(currentWorkingDirectory, fileName);
    if (targetFile == NULL || targetFile->isDirectory) {
        printf("Error: File '%s' not found.\n", fileName);
        return;
    }

    freeFileBlocks(targetFile);

    if (targetFile->next == targetFile)
        currentWorkingDirectory->children = NULL;
    else {
        if (currentWorkingDirectory->children == targetFile)
            currentWorkingDirectory->children = targetFile->next;
        targetFile->previous->next = targetFile->next;
        targetFile->next->previous = targetFile->previous;
    }

    free(targetFile);
    printf("File '%s' deleted successfully.\n", fileName);
}
 

// Helping Functions 

FreeBlock *allocateFreeBlock() {
    if (freeBlockHead == NULL)
        return NULL;

    FreeBlock *allocatedBlock = freeBlockHead;
    freeBlockHead = freeBlockHead->next;

    if (freeBlockHead != NULL)
        freeBlockHead->previous = NULL;
    else
        freeBlockTail = NULL;

    allocatedBlock->next = allocatedBlock->previous = NULL;
    return allocatedBlock;
}

void returnBlockToFreeList(int blockIndex) {
    FreeBlock *freedBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
    freedBlock->blockIndex = blockIndex;
    freedBlock->next = NULL;
    freedBlock->previous = freeBlockTail;

    if (freeBlockTail != NULL)
        freeBlockTail->next = freedBlock;
    else
        freeBlockHead = freedBlock;

    freeBlockTail = freedBlock;
}

void freeFileBlocks(FileNode *fileNode) {
    for (int block = 0; block < fileNode->allocatedBlockCount; block++)
        returnBlockToFreeList(fileNode->blockPointers[block]);

    fileNode->allocatedBlockCount = 0;
}

FileNode *findFileNode(FileNode *directory, char *targetName) {
    if (directory->children == NULL)
        return NULL;

    FileNode *iterator = directory->children;
    do {
        if (strcmp(iterator->name, targetName) == 0)
            return iterator;
        iterator = iterator->next;
    } while (iterator != directory->children);

    return NULL;
}

void displayDiskUsage() {
    int freeBlocksCount = 0;
    FreeBlock *iterator = freeBlockHead;
    while (iterator != NULL) {
        freeBlocksCount++;
        iterator = iterator->next;
    }

    int usedBlocksCount = TOTAL_BLOCKS - freeBlocksCount;
    double usagePercent = ((double)usedBlocksCount / TOTAL_BLOCKS) * 100.0;

    printf("Total Blocks: %d\n", TOTAL_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocksCount);
    printf("Free Blocks: %d\n", freeBlocksCount);
    printf("Disk Usage: %.2f%%\n", usagePercent);
}

void freeAllMemory(FileNode *directory) {
    if (directory == NULL)
        return;

    if (directory->children != NULL) {
        FileNode *iterator = directory->children;
        do {
            FileNode *nextNode = iterator->next;
            freeAllMemory(iterator);
            iterator = nextNode;
        } while (iterator != directory->children);
    }
    free(directory);
}

void exitSystem() {
    freeAllMemory(rootDirectory);

    FreeBlock *iterator = freeBlockHead;
    while (iterator != NULL) {
        FreeBlock *nextBlock = iterator->next;
        free(iterator);
        iterator = nextBlock;
    }

    printf("Memory released. Exiting program...\n");
    exit(0);
}


// Execution of the commands provided in the assignment 

void executeCommand(char *commandLine) {
    char command[20], arg1[50], arg2[512];
    memset(arg1, 0, sizeof(arg1));
    memset(arg2, 0, sizeof(arg2));

    sscanf(commandLine, "%s %s %[^\n]", command, arg1, arg2);

    if (strcmp(command, "mkdir") == 0) createDirectory(arg1);
    else if (strcmp(command, "rmdir") == 0) removeDirectory(arg1);
    else if (strcmp(command, "ls") == 0) listDirectoryContents();
    else if (strcmp(command, "cd") == 0) changeDirectory(arg1);
    else if (strcmp(command, "pwd") == 0) showCurrentPath();
    else if (strcmp(command, "create") == 0) createFile(arg1);
    else if (strcmp(command, "write") == 0) writeFile(arg1, arg2);
    else if (strcmp(command, "read") == 0) readFile(arg1);
    else if (strcmp(command, "delete") == 0) deleteFile(arg1);
    else if (strcmp(command, "df") == 0) displayDiskUsage();
    else if (strcmp(command, "exit") == 0) exitSystem();
    else printf("Unknown command: %s\n", command);
}


// Main Function


int main() {
    initializeVirtualFileSystem();

    char commandLine[600];
    while (1) {
        printf("%s > ", currentWorkingDirectory->name);
        fgets(commandLine, sizeof(commandLine), stdin);
        commandLine[strcspn(commandLine, "\n")] = '\0';
        executeCommand(commandLine);
    }
    return 0;
}
