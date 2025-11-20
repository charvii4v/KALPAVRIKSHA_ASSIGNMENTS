#include <stdio.h>
#include "player.h"

float calculatePerformanceIndex(PlayerData* player) {
    float score = 0.0;

    if (player->role == ROLE_BATSMAN) {
        score = player->totalRuns +
                player->battingAverage * 5 +
                player->strikeRate * 0.5;
    } else if (player->role == ROLE_BOWLER) {
        score = player->wickets * 20 +
                (player->economyRate > 0 ? 100 / player->economyRate : 0);
    } else if (player->role == ROLE_ALLROUNDER) {
        score = player->totalRuns +
                player->battingAverage * 3 +
                player->strikeRate * 0.2 +
                player->wickets * 15 +
                (player->economyRate > 0 ? 50 / player->economyRate : 0);
    }
    return score;
}

void insertIntoSortedList(PlayerNode** head, PlayerNode* newNode) {
    if (*head == NULL || newNode->data->performanceIndex > (*head)->data->performanceIndex) {
        newNode->next = *head;
        *head = newNode;
        return;
    }
    PlayerNode* current = *head;
    while (current->next != NULL &&
           current->next->data->performanceIndex > newNode->data->performanceIndex) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

const char* getRoleName(PlayerRole role) {
    switch(role) {
        case ROLE_BATSMAN: return "Batsman";
        case ROLE_BOWLER: return "Bowler";
        case ROLE_ALLROUNDER: return "All-Rounder";
        default: return "Unknown";
    }
}

void printPlayer(PlayerData* p) {
    printf("ID: %d | Name: %s | Role: %s | PI: %.2f\n",
           p->playerId, p->name, getRoleName(p->role), p->performanceIndex);
}

void printPlayerWithTeam(PlayerData* p) {
    printf("ID: %d | Name: %s | Team: %s | Role: %s | PI: %.2f\n",
           p->playerId, p->name, p->teamName, getRoleName(p->role), p->performanceIndex);
}
