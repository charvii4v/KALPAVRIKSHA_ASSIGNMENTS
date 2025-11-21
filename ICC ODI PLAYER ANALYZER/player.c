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
    for (int i = 0; i < 10; ++i) {
        if (strcmp(all_teams[i].name, name) == 0) return &all_teams[i];
    }
    return NULL;
}

int compareTeamById(const void* a, const void* b) {
    const Team* A = (const Team*)a;
    const Team* B = (const Team*)b;
    return (A->teamId - B->teamId);
}

Team* findTeamById(int id) {
    Team key;
    key.teamId = id;
    return (Team*)bsearch(&key, all_teams, 10, sizeof(Team), compareTeamById);
}

    for (int t = 0; t < 10; ++t) {
        all_teams[t].teamId = t + 1;
        strncpy(all_teams[t].name, teams[t], 50);
        all_teams[t].name[50] = '\0';
        all_teams[t].totalPlayers = 0;
        all_teams[t].battingSRSum = 0.0;
        all_teams[t].battingSRCount = 0;
        all_teams[t].all_players_list = NULL;
        all_teams[t].batsmen_sorted_list = NULL;
        all_teams[t].bowlers_sorted_list = NULL;
        all_teams[t].allrounders_sorted_list = NULL;
    }


    qsort(all_teams, 10, sizeof(Team), compareTeamById);

    for (int i = 0; i < playerCount; ++i) {
        const Player* orig = &players[i];

        PlayerData* pd = (PlayerData*)malloc(sizeof(PlayerData));
        if (!pd) {
            perror("malloc PlayerData");
            continue;
        }

        pd->playerId = orig->id;
        strncpy(pd->name, orig->name, 50);
        pd->name[50] = '\0';
        strncpy(pd->teamName, orig->team, 50);
        pd->teamName[50] = '\0';
        if (strcmp(orig->role, "Batsman") == 0) pd->role = ROLE_BATSMAN;
        else if (strcmp(orig->role, "Bowler") == 0) pd->role = ROLE_BOWLER;
        else pd->role = ROLE_ALLROUNDER;

        pd->totalRuns = orig->totalRuns;
        pd->battingAverage = orig->battingAverage;
        pd->strikeRate = orig->strikeRate;
        pd->wickets = orig->wickets;
        pd->economyRate = orig->economyRate;
        pd->performanceIndex = calculatePerformanceIndex(pd);

        Team* team = findTeamByName(pd->teamName);
        if (team) addPlayerToTeam(team, pd);
        else {
            printf("Warning: Team '%s' not found for player '%s'\n", pd->teamName, pd->name);
            free(pd);
        }
    }
}

void cleanup() {
    for (int t = 0; t < 10; ++t) {
    
        PlayerNode* cur = all_teams[t].all_players_list;
        while (cur) {
            PlayerNode* next = cur->next;
            if (cur->data) free(cur->data);
            free(cur);
            cur = next;
        }
        cur = all_teams[t].batsmen_sorted_list;
        while (cur) {
            PlayerNode* next = cur->next;
            free(cur);
            cur = next;
        }
        cur = all_teams[t].bowlers_sorted_list;
        while (cur) {
            PlayerNode* next = cur->next;
            free(cur);
            cur = next;
        }
        cur = all_teams[t].allrounders_sorted_list;
        while (cur) {
            PlayerNode* next = cur->next;
            free(cur);
            cur = next;
        }
    }
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

void maxHeapify(HeapNode heap[], int heapSize, int index) {
    int largest = index;
    int left = 2*index + 1;
    int right = 2*index + 2;

    if (left < heapSize && heap[left].player->performanceIndex > heap[largest].player->performanceIndex)
        largest = left;
    if (right < heapSize && heap[right].player->performanceIndex > heap[largest].player->performanceIndex)
        largest = right;

    if (largest != index) {
        swapHeapNodes(&heap[index], &heap[largest]);
        maxHeapify(heap, heapSize, largest);
    }
}

HeapNode extractMax(HeapNode heap[], int* heapSizePtr) {
    if (*heapSizePtr <= 0) return (HeapNode){NULL,-1,NULL};
    HeapNode top = heap[0];
    heap[0] = heap[*heapSizePtr - 1];
    (*heapSizePtr)--;
    maxHeapify(heap, *heapSizePtr, 0);
    return top;
}

void insertIntoHeap(HeapNode heap[], int* heapSizePtr, HeapNode new_node) {
    if (*heapSizePtr >= 10) { 
        return;
    }
    int i = (*heapSizePtr)++;
    heap[i] = new_node;
    while (i != 0 && heap[(i-1)/2].player->performanceIndex < heap[i].player->performanceIndex) {
        swapHeapNodes(&heap[i], &heap[(i-1)/2]);
        i = (i-1)/2;
    }
}

const char* getRoleName(PlayerRole role) {
    switch (role) {
        case ROLE_BATSMAN: return "Batsman";
        case ROLE_BOWLER: return "Bowler";
        case ROLE_ALLROUNDER: return "All-rounder";
        default: return "Unknown";
    }
}

void printPlayer(PlayerData* player) {
    if (!player) return;
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
    if (!p) return;
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
