#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"


void handleAddPlayer();
void handleDisplayTeamPlayers();
void handleDisplayTeamsBySR();
void handleDisplayTopK();
void handleDisplayAllByRole();
int showMenu();

const char* getRoleName(PlayerRole role) {
    switch (role) {
        case ROLE_BATSMAN: return "Batsman";
        case ROLE_BOWLER: return "Bowler";
        case ROLE_ALLROUNDER: return "All-rounder";
        default: return "Unknown";
    }
}

void printPlayer(PlayerData* player) {
    if (player == NULL) return;
    printf("| %-5d | %-20s | %-12s | %-6d | %-6.2f | %-7.2f | %-6d | %-6.2f | %-10.2f |\n",
           player->playerId,
           player->name,
           getRoleName(player->role),
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->wickets,
           player->economyRate,
           player->performanceIndex);
}

void printPlayerWithTeam(PlayerData* p) {
    if (p == NULL) return;
    printf("| %-5d | %-20s | %-12s | %-12s | %-6d | %-6.2f | %-7.2f | %-6d | %-6.2f | %-10.2f |\n",
           p->playerId,
           p->name,
           p->teamName,
           getRoleName(p->role),
           p->totalRuns,
           p->battingAverage,
           p->strikeRate,
           p->wickets,
           p->economyRate,
           p->performanceIndex);
}

void handleAddPlayer() {
    int teamId, roleChoice;
    printf("\nEnter Team ID to add player: ");
    if (scanf("%d", &teamId) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); 
        return;
    }

    Team* team = findTeamById(teamId);
    if (team == NULL) {
        printf("Team with ID %d not found.\n", teamId);
        return;
    }

    PlayerData* new_player = (PlayerData*)malloc(sizeof(PlayerData));
    if (new_player == NULL) {
        printf("Failed to allocate memory for new player.\n");
        return;
    }

    printf("Enter Player Details:\n");
    printf("Player ID: ");
    scanf("%d", &new_player->playerId);

    printf("Name: ");
    scanf(" %[^\n]", new_player->name); 

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);
    new_player->role = (PlayerRole)roleChoice;

    printf("Total Runs: ");
    scanf("%d", &new_player->totalRuns);

    printf("Batting Average: ");
    scanf("%f", &new_player->battingAverage);

    printf("Strike Rate: ");
    scanf("%f", &new_player->strikeRate);

    printf("Wickets: ");
    scanf("%d", &new_player->wickets);

    printf("Economy Rate: ");
    scanf("%f", &new_player->economyRate);

    strncpy(new_player->teamName, team->name, 50);
    new_player->teamName[50] = '\0';
    new_player->performanceIndex = calculatePerformanceIndex(new_player);

    addPlayerToTeam(team, new_player);

    printf("Player added successfully to Team %s!\n", team->name);
}

void handleDisplayTeamPlayers() {
    int teamId;
    printf("\nEnter Team ID: ");
    if (scanf("%d", &teamId) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); 
        return;
    }

    Team* team = findTeamById(teamId);
    if (team == NULL) {
        printf("Team with ID %d not found.\n", teamId);
        return;
    }

    printf("\nPlayers of Team %s:\n", team->name);
    printf("====\n");
    printf("| %-5s | %-20s | %-12s | %-6s | %-6s | %-7s | %-6s | %-6s | %-10s |\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("----\n");

    PlayerNode* current = team->all_players_list;
    if (current == NULL) {
        printf("| No players found in this team.                                                                                  |\n");
    }
    while (current != NULL) {
        printPlayer(current->data);
        current = current->next;
    }
    printf("====\n");

    double avg_sr = (team->battingSRCount == 0) ? 0.0 : (team->battingSRSum / team->battingSRCount);
    printf("Total Players: %d\n", team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", avg_sr);
}

int compareTeamsBySR(const void* a, const void* b) {
    const Team* teamA = *(const Team**)a;
    const Team* teamB = *(const Team**)b;

    double avg_sr_A = (teamA->battingSRCount == 0) ? 0.0 : (teamA->battingSRSum / teamA->battingSRCount);
    double avg_sr_B = (teamB->battingSRCount == 0) ? 0.0 : (teamB->battingSRSum / teamB->battingSRCount);

    if (avg_sr_B > avg_sr_A) return 1;
    if (avg_sr_B < avg_sr_A) return -1;
    return 0;
}

void handleDisplayTeamsBySR() {
    Team* team_pointers[10];
    for (int teamIndex = 0; teamIndex < 10; teamIndex++) {
        team_pointers[teamIndex] = &all_teams[teamIndex];
    }

    qsort(team_pointers, 10, sizeof(Team*), compareTeamsBySR);

    printf("\nTeams Sorted by Average Batting Strike Rate:\n");
    printf("====\n");
    printf("| %-4s | %-20s | %-12s | %-13s |\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("----\n");

    for (int teamIndex = 0; teamIndex < 10; teamIndex++) {
        Team* team = team_pointers[teamIndex];
        double avg_sr = (team->battingSRCount == 0) ? 0.0 : (team->battingSRSum / team->battingSRCount);
        printf("| %-4d | %-20s | %-12.2f | %-13d |\n",
               team->teamId,
               team->name,
               avg_sr,
               team->totalPlayers);
    }
    printf("====\n");
}

void handleDisplayTopK() {
    int teamId, roleChoice, k;

    printf("\nEnter Team ID: ");
    if (scanf("%d", &teamId) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); return;
    }
    Team* team = findTeamById(teamId);
    if (team == NULL) {
        printf("Team with ID %d not found.\n", teamId);
        return;
    }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1 || roleChoice < 1 || roleChoice > 3) {
         printf("Invalid role.\n");
         while (getchar() != '\n'); return;
    }

    printf("Enter number of players (K): ");
    if (scanf("%d", &k) != 1 || k < 1) {
        printf("Invalid number K.\n");
        while (getchar() != '\n'); return;
    }

    PlayerNode** list_head_ptr = getSortedListHead(team, (PlayerRole)roleChoice);
    if (list_head_ptr == NULL || *list_head_ptr == NULL) {
        printf("No players found for this role in Team %s.\n", team->name);
        return;
    }

    printf("\nTop %d %s(s) of Team %s:\n", k, getRoleName((PlayerRole)roleChoice), team->name);
    printf("====\n");
    printf("| %-5s | %-20s | %-12s | %-6s | %-6s | %-7s | %-6s | %-6s | %-10s |\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("----\n");

    PlayerNode* current = *list_head_ptr;
    int playersPrinted = 0;

    for (playersPrinted = 0; playersPrinted < k && current != NULL; playersPrinted++) {
        printPlayer(current->data);
        current = current->next;
    }

    if (playersPrinted == 0) {
         printf("| No players found for this role.|\n");
    }
    printf("====\n");
}


void handleDisplayAllByRole() {
    int roleChoice;
    printf("\nEnter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1 || roleChoice < 1 || roleChoice > 3) {
         printf("Invalid role.\n");
         while (getchar() != '\n'); return;
    }

    printf("\nAll %s(s) Across All Teams:\n", getRoleName((PlayerRole)roleChoice));
    printf("=====\n");
    printf("| %-5s | %-20s | %-12s | %-12s | %-6s | %-6s | %-7s | %-6s | %-6s | %-10s |\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("----\n");

    HeapNode heap[10];
    int heap_size = 0;

    for (int teamIndex = 0; teamIndex < 10; teamIndex++) {
        PlayerNode** list_head_ptr = getSortedListHead(&all_teams[teamIndex], (PlayerRole)roleChoice);
        if (list_head_ptr && *list_head_ptr) {
            HeapNode new_node = {
                (*list_head_ptr)->data,
                teamIndex,
                (*list_head_ptr)->next
            };
            insertIntoHeap(heap, &heap_size, new_node);
        }
    }

    if (heap_size == 0) {
        printf("| No players found for this role across all teams.|\n");
        printf("====\n");
        return;
    }

    while (heap_size > 0) {
        HeapNode max_node = extractMax(heap, &heap_size);
        printPlayerWithTeam(max_node.player);

        if (max_node.next_node != NULL) {
            HeapNode next_in_list = {
                max_node.next_node->data,
                max_node.team_index,
                max_node.next_node->next
            };
            insertIntoHeap(heap, &heap_size, next_in_list);
        }
    }
    printf("====\n");
}

int showMenu() {
    int choice;
    printf("\n--- ICC ODI Player Performance Analyzer ---\n");
    printf("1. Add Player to Team\n");
    printf("2. Display Players of a Specific Team\n");
    printf("3. Display Teams by Average Batting Strike Rate\n");
    printf("4. Display Top K Players of a Specific Team by Role\n");
    printf("5. Display all Players of specific role Across All Teams by performance index\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n');
        return -1;
    }
    return choice;
}

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

