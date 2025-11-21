#include "player.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Players_data.h"

Team all_teams[10];

float calculatePerformanceIndex(PlayerData* player) {
    if (player == NULL) return 0.0f;

    switch (player->role) {
        case ROLE_BATSMAN:
            return (player->battingAverage * player->strikeRate) / 100.0f;
        case ROLE_BOWLER:
            return (player->wickets * 2) + (100.0f - player->economyRate);
        case ROLE_ALLROUNDER:
            return ((player->battingAverage * player->strikeRate) / 100.0f) + (player->wickets * 2);
        default:
            return 0.0f;
    }
}

void insertIntoSortedList(PlayerNode** list_head, PlayerNode* new_node) {
    PlayerNode* current;

    if (*list_head == NULL || (*list_head)->data->performanceIndex <= new_node->data->performanceIndex) {
        new_node->next = *list_head;
        *list_head = new_node;
    } else {
        current = *list_head;
        while (current->next != NULL && current->next->data->performanceIndex > new_node->data->performanceIndex) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

PlayerNode** getSortedListHead(Team* team, PlayerRole role) {
    if (!team) return NULL;
    switch (role) {
        case ROLE_BATSMAN: return &team->batsmen_sorted_list;
        case ROLE_BOWLER: return &team->bowlers_sorted_list;
        case ROLE_ALLROUNDER: return &team->allrounders_sorted_list;
        default: return NULL;
    }
}

void addPlayerToTeam(Team* team, PlayerData* new_player_data) {
    if (team == NULL || new_player_data == NULL) return;

    PlayerNode* node_for_all = (PlayerNode*)malloc(sizeof(PlayerNode));
    if (!node_for_all) {
        perror("Failed to allocate node for all list");
        return;
    }
    node_for_all->data = new_player_data;
    node_for_all->next = team->all_players_list;
    team->all_players_list = node_for_all;

    PlayerNode* node_for_sorted = (PlayerNode*)malloc(sizeof(PlayerNode));
    if (!node_for_sorted) {
        perror("Failed to allocate node for sorted list");
        
        team->all_players_list = node_for_all->next;
        free(node_for_all);
        return;
    }
    node_for_sorted->data = new_player_data;
    node_for_sorted->next = NULL;

    PlayerNode** sorted_list_head = getSortedListHead(team, new_player_data->role);
    if (sorted_list_head) {
        insertIntoSortedList(sorted_list_head, node_for_sorted);
    } else {
        free(node_for_sorted);
    }

    team->totalPlayers++;
    if (new_player_data->role == ROLE_BATSMAN || new_player_data->role == ROLE_ALLROUNDER) {
        team->battingSRSum += new_player_data->strikeRate;
        team->battingSRCount++;
    }
}

Team* findTeamByName(const char* name) {
    for (int teamIndex = 0; teamIndex < 10; teamIndex++) {
        if (strcmp(all_teams[teamIndex].name, name) == 0) {
            return &all_teams[teamIndex];
        }
    }
    return NULL;
}

int compareTeamById(const void* a, const void* b) {
    const Team* teamA = (const Team*)a;
    const Team* teamB = (const Team*)b;
    return (teamA->teamId - teamB->teamId);
}

Team* findTeamById(int id) {
    Team key;
    key.teamId = id;
    return (Team*)bsearch(&key, all_teams, 10, sizeof(Team), compareTeamById);
}

void initializeData() {
    for (int teamIndex = 0; teamIndex < 10; teamIndex++) {
        all_teams[teamIndex].teamId = teamIndex + 1;
        strncpy(all_teams[teamIndex].name, teams[teamIndex], 50);
        all_teams[teamIndex].name[50] = '\0';
        all_teams[teamIndex].totalPlayers = 0;
        all_teams[teamIndex].battingSRSum = 0.0;
        all_teams[teamIndex].battingSRCount = 0;
        all_teams[teamIndex].all_players_list = NULL;
        all_teams[teamIndex].batsmen_sorted_list = NULL;
        all_teams[teamIndex].bowlers_sorted_list = NULL;
        all_teams[teamIndex].allrounders_sorted_list = NULL;
    }

    qsort(all_teams, 10, sizeof(Team), compareTeamById);

    for (int playerIndex = 0; playerIndex < playerCount; playerIndex++) {
        const Player* initialPlayer = &players[playerIndex];

        PlayerData* new_player_data = (PlayerData*)malloc(sizeof(PlayerData));
        if (new_player_data == NULL) {
            perror("Failed to allocate memory for player data");
            continue;
        }

        new_player_data->playerId = initialPlayer->id;
        strncpy(new_player_data->name, initialPlayer->name, 50);
        new_player_data->name[50] = '\0';
        strncpy(new_player_data->teamName, initialPlayer->team, 50);
        new_player_data->teamName[50] = '\0';

        if (strcmp(initialPlayer->role, "Batsman") == 0) {
            new_player_data->role = ROLE_BATSMAN;
        } else if (strcmp(initialPlayer->role, "Bowler") == 0) {
            new_player_data->role = ROLE_BOWLER;
        } else {
            new_player_data->role = ROLE_ALLROUNDER;
        }

        new_player_data->totalRuns = initialPlayer->totalRuns;
        new_player_data->battingAverage = initialPlayer->battingAverage;
        new_player_data->strikeRate = initialPlayer->strikeRate;
        new_player_data->wickets = initialPlayer->wickets;
        new_player_data->economyRate = initialPlayer->economyRate;

        new_player_data->performanceIndex = calculatePerformanceIndex(new_player_data);

        Team* team = findTeamByName(new_player_data->teamName);
        if (team) {
            addPlayerToTeam(team, new_player_data);
        } else {
            printf("Warning: Team '%s' not found for player '%s'. Player not added.\n", new_player_data->teamName, new_player_data->name);
            free(new_player_data);
        }
    }
}

void cleanup() {
    for (int teamIndex = 0; teamIndex < 10; teamIndex++) {
        PlayerNode* current = all_teams[teamIndex].all_players_list;
        while (current != NULL) {
            PlayerNode* nodeToFree = current;
            current = current->next;

            free(nodeToFree->data);
            free(nodeToFree);
        }

        current = all_teams[teamIndex].batsmen_sorted_list;
        while (current != NULL) {
            PlayerNode* nodeToFree = current;
            current = current->next;
            free(nodeToFree);
        }

        current = all_teams[teamIndex].bowlers_sorted_list;
        while (current != NULL) {
            PlayerNode* nodeToFree = current;
            current = current->next;
            free(nodeToFree);
        }

        current = all_teams[teamIndex].allrounders_sorted_list;
        while (current != NULL) {
            PlayerNode* nodeToFree = current;
            current = current->next;
            free(nodeToFree);
        }
    }
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

void maxHeapify(HeapNode heap[], int heapSize, int index) {
    int largestIndex = index;
    int leftIndex = 2 * index + 1;
    int rightIndex = 2 * index + 2;

    if (leftIndex < heapSize && heap[leftIndex].player->performanceIndex > heap[largestIndex].player->performanceIndex) {
        largestIndex = leftIndex;
    }
    if (rightIndex < heapSize && heap[rightIndex].player->performanceIndex > heap[largestIndex].player->performanceIndex) {
        largestIndex = rightIndex;
    }

    if (largestIndex != index) {
        swapHeapNodes(&heap[index], &heap[largestIndex]);
        maxHeapify(heap, heapSize, largestIndex);
    }
}

HeapNode extractMax(HeapNode heap[], int* heapSizePtr) {
    if (*heapSizePtr <= 0) return (HeapNode){NULL, -1, NULL};

    HeapNode maxNode = heap[0];
    heap[0] = heap[*heapSizePtr - 1];
    (*heapSizePtr)--;
    maxHeapify(heap, *heapSizePtr, 0);
    return maxNode;
}

void insertIntoHeap(HeapNode heap[], int* heapSizePtr, HeapNode new_node) {
    if (*heapSizePtr == 10) {
        
        return;
    }

    (*heapSizePtr)++;
    int currentIndex = *heapSizePtr - 1;
    heap[currentIndex] = new_node;

    while (currentIndex != 0 && heap[(currentIndex - 1) / 2].player->performanceIndex < heap[currentIndex].player->performanceIndex) {
        swapHeapNodes(&heap[currentIndex], &heap[(currentIndex - 1) / 2]);
        currentIndex = (currentIndex - 1) / 2;
    }
}

