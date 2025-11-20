#include <stdio.h>
#include "handlers.h"
#include "team.h"

int main() {
    initializeData();

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Add Player\n");
        printf("2. Display Players of Team\n");
        printf("3. Display Teams by Strike Rate\n");
        printf("4. Display Top K by Role\n");
        printf("5. Display All by Role\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: handleAddPlayer(); break;
            case 2: handleDisplayTeamPlayers(); break;
            case 3: handleDisplayTeamsBySR(); break;
            case 4: handleDisplayTopK(); break;
            case 5: handleDisplayAllByRole(); break;
        }

    } while(choice != 0);

    cleanup();
    return 0;
}
