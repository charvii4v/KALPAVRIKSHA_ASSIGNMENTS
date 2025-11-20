#include <stdio.h>
#include "team.h"
#include "handlers.h"

int main() {
    initializeData();

    int choice = 0;
    while (choice != 6) {
        choice = showMenu();
        switch (choice) {
            case 1:
                handleAddPlayer();
                break;
            case 2:
                handleDisplayTeamPlayers();
                break;
            case 3:
                handleDisplayTeamsBySR();
                break;
            case 4:
                handleDisplayTopK();
                break;
            case 5:
                handleDisplayAllByRole();
                break;
            case 6:
                printf("Exiting... Thank you!\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    }

    cleanup();
    return 0;
}
