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
        while (getchar() != '\n');
        return -1;
    }
    return choice;
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
    if (!new_player) {
        printf("Allocation failed\n");
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
    if (!team) {
        printf("Team with ID %d not found.\n", teamId);
        return;
    }

    printf("\nPlayers of Team %s:\n", team->name);
    printf("=====\n");
    printf("| %-5s | %-20s | %-12s | %-6s | %-6s | %-7s | %-6s | %-6s | %-10s |\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("---\n");

    PlayerNode* cur = team->all_players_list;
    if (!cur) {
        printf("| No players found in this team.                                                                                  |\n");
    }
    while (cur) {
        printPlayer(cur->data);
        cur = cur->next;
    }
   printf("=====\n");

    double avg_sr = (team->battingSRCount == 0) ? 0.0 : (team->battingSRSum / team->battingSRCount);
    printf("Total Players: %d\n", team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", avg_sr);
}

int compareTeamsBySR(const void* a, const void* b) {
    const Team* A = *(const Team**)a;
    const Team* B = *(const Team**)b;
    double avgA = (A->battingSRCount == 0) ? 0.0 : (A->battingSRSum / A->battingSRCount);
    double avgB = (B->battingSRCount == 0) ? 0.0 : (B->battingSRSum / B->battingSRCount);
    if (avgB > avgA) return 1;
    if (avgB < avgA) return -1;
    return 0;
}

void handleDisplayTeamsBySR() {
    Team* ptrs[10];
    for (int i = 0; i < 10; ++i) ptrs[i] = &all_teams[i];

    qsort(ptrs, 10, sizeof(Team*), compareTeamsBySR);

    printf("\nTeams Sorted by Average Batting Strike Rate:\n");
  printf("=====\n");
    printf("| %-4s | %-20s | %-12s | %-13s |\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("----\n");
    for (int i = 0; i < 10; ++i) {
        Team* t = ptrs[i];
        double avg_sr = (t->battingSRCount == 0) ? 0.0 : (t->battingSRSum / t->battingSRCount);
        printf("| %-4d | %-20s | %-12.2f | %-13d |\n", t->teamId, t->name, avg_sr, t->totalPlayers);
    }
 printf("=====\n");
}

void handleDisplayTopK() {
    int teamId, roleChoice, k;
    printf("\nEnter Team ID: ");
    if (scanf("%d", &teamId) != 1) { while (getchar() != '\n'); return; }
    Team* team = findTeamById(teamId);
    if (!team) { printf("Team not found.\n"); return; }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1 || roleChoice < 1 || roleChoice > 3) { printf("Invalid role\n"); return; }

    printf("Enter number of players (K): ");
    if (scanf("%d", &k) != 1 || k < 1) { printf("Invalid K\n"); return; }

    PlayerNode** headptr = getSortedListHead(team, (PlayerRole)roleChoice);
    if (!headptr || *headptr == NULL) { printf("No players found for this role in Team %s.\n", team->name); return; }

    printf("\nTop %d %s(s) of Team %s:\n", k, getRoleName((PlayerRole)roleChoice), team->name);
   printf("=====\n");
    printf("| %-5s | %-20s | %-12s | %-6s | %-6s | %-7s | %-6s | %-6s | %-10s |\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("-----\n");

    PlayerNode* cur = *headptr;
    int printed = 0;
    while (printed < k && cur) {
        printPlayer(cur->data);
        printed++;
        cur = cur->next;
    }
    if (printed == 0) printf("| No players found for this role.|\n");
    printf("=====\n");
}

void handleDisplayAllByRole() {
    int roleChoice;
    printf("\nEnter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1 || roleChoice < 1 || roleChoice > 3) { printf("Invalid role\n"); return; }

    printf("\nAll %s(s) Across All Teams:\n", getRoleName((PlayerRole)roleChoice));
printf("=====\n");
    printf("| %-5s | %-20s | %-12s | %-12s | %-6s | %-6s | %-7s | %-6s | %-6s | %-10s |\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("----\n");

    HeapNode heap[10];
    int heap_size = 0;
    for (int i = 0; i < 10; ++i) {
        PlayerNode** hp = getSortedListHead(&all_teams[i], (PlayerRole)roleChoice);
        if (hp && *hp) {
            HeapNode node = { (*hp)->data, i, (*hp)->next };
            insertIntoHeap(heap, &heap_size, node);
        }
    }

    if (heap_size == 0) {
        printf("| No players found for this role across all teams|\n");
        printf("=====\n");
        return;
    }

    while (heap_size > 0) {
        HeapNode top = extractMax(heap, &heap_size);
        printPlayerWithTeam(top.player);

        if (top.next_node != NULL) {
            HeapNode nextn = { top.next_node->data, top.team_index, top.next_node->next };
            insertIntoHeap(heap, &heap_size, nextn);
        }
    }

    printf("===\n");
}

int main() {
    initializeData();

    int choice = 0;
    while (choice != 6) {
        choice = showMenu();
        switch (choice) {
            case 1: handleAddPlayer(); break;
            case 2: handleDisplayTeamPlayers(); break;
            case 3: handleDisplayTeamsBySR(); break;
            case 4: handleDisplayTopK(); break;
            case 5: handleDisplayAllByRole(); break;
            case 6: printf("Exiting... Thank you!\n"); break;
            default: printf("Invalid choice.\n");
        }
    }

    cleanup();
    return 0;
}
