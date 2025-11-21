#include <stdio.h>
#include "player.h"

int main() {
    initializeData();

    int choice;
    while (1) {
        menu();
        scanf("%d", &choice);

        if (choice == 1) {
            printAllTeams();
        }
        else if (choice == 2) {
            PlayerData p;
            char teamName[50];

            printf("Enter Team Name: ");
            scanf("%s", teamName);

            Team* t = findTeamByName(teamName);
            if (!t) {
                printf("Team not found!\n");
                continue;
            }

            printf("Enter Player ID: ");
            scanf("%d", &p.id);

            printf("Enter Name: ");
            scanf("%s", p.name);

            printf("Enter Runs: ");
            scanf("%d", &p.runs);

            printf("Enter Wickets: ");
            scanf("%d", &p.wickets);

            printf("Enter Strike Rate: ");
            scanf("%f", &p.strikeRate);

            printf("Enter Average: ");
            scanf("%f", &p.average);

            p.role = BATSMAN;

            addPlayerToTeam(t, p);
            printf("\nPlayer added successfully!\n");
        }
        else if (choice == 0) {
            return 0;
        }
        else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
