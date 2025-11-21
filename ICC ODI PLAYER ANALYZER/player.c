#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

Team all_teams[10];
int teamCount = 0;

float calculatePerformanceIndex(PlayerData p) {
    return (p.runs * 0.7f) + (p.wickets * 1.3f) + (p.strikeRate * 0.05f);
}

void insertIntoSortedList(PlayerNode** head, PlayerData player) {
    PlayerNode* newNode = (PlayerNode*)malloc(sizeof(PlayerNode));
    newNode->data = player;
    newNode->next = NULL;

    if (*head == NULL || (*head)->data.performanceIndex < player.performanceIndex) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    PlayerNode* curr = *head;
    while (curr->next && curr->next->data.performanceIndex >= player.performanceIndex)
        curr = curr->next;

    newNode->next = curr->next;
    curr->next = newNode;
}


void printPlayer(PlayerData p) {
    printf("%d | %s | Runs: %d | Wickets: %d | SR: %.2f | Avg: %.2f | PI: %.2f\n",
           p.id, p.name, p.runs, p.wickets, p.strikeRate, p.average, p.performanceIndex);
}

void printPlayerWithTeam(PlayerData p, char* teamName) {
    printf("[%s] ", teamName);
    printPlayer(p);
}


Team* findTeamByName(char* name) {
    for (int i = 0; i < teamCount; i++)
        if (strcmp(all_teams[i].name, name) == 0)
            return &all_teams[i];
    return NULL;
}

void addPlayerToTeam(Team* team, PlayerData p) {
    p.performanceIndex = calculatePerformanceIndex(p);
    insertIntoSortedList(&team->sortedListHead, p);
}

void initializeData() {
    strcpy(all_teams[0].name, "India");
    strcpy(all_teams[1].name, "Australia");
    strcpy(all_teams[2].name, "England");
    strcpy(all_teams[3].name, "South Africa");

    teamCount = 4;

    PlayerData p1 = {1, "Rohit", BATSMAN, 500, 2, 88.5, 45.2, 0};
    PlayerData p2 = {2, "Bumrah", BOWLER, 60, 21, 72.4, 20.1, 0};

    addPlayerToTeam(&all_teams[0], p1);
    addPlayerToTeam(&all_teams[0], p2);
}

void printAllTeams() {
    for (int i = 0; i < teamCount; i++) {
        printf("\n===== Team: %s =====\n", all_teams[i].name);
        PlayerNode* temp = all_teams[i].sortedListHead;

        if (!temp) {
            printf("No players.\n");
            continue;
        }

        while (temp) {
            printPlayerWithTeam(temp->data, all_teams[i].name);
            temp = temp->next;
        }
    }
}

void menu() {
    printf("\n========== ICC MENU ==========\n");
    printf("1. View Teams & Players\n");
    printf("2. Add Player to Team\n");
    printf("0. Exit\n");
    printf("Your choice: ");
}
