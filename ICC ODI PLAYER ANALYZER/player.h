#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>


typedef enum {
    ROLE_BATSMAN = 1,
    ROLE_BOWLER = 2,
    ROLE_ALLROUNDER = 3
} PlayerRole;

typedef struct {
    int playerId;
    char name[51];
    char teamName[51];
    PlayerRole role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
} PlayerData;

typedef struct PlayerNode {
    PlayerData* data;
    struct PlayerNode* next;
} PlayerNode;

typedef struct {
    int teamId;
    char name[51];
    int totalPlayers;
    double battingSRSum;
    int battingSRCount;

    PlayerNode* all_players_list;       
    PlayerNode* batsmen_sorted_list;   
    PlayerNode* bowlers_sorted_list;
    PlayerNode* allrounders_sorted_list;
} Team;

typedef struct {
    PlayerData* player;
    int team_index;
    PlayerNode* next_node;
} HeapNode;

extern Team all_teams[10];

void initializeData();
void cleanup();

float calculatePerformanceIndex(PlayerData* player);
void insertIntoSortedList(PlayerNode** list_head, PlayerNode* new_node);
PlayerNode** getSortedListHead(Team* team, PlayerRole role);
void addPlayerToTeam(Team* team, PlayerData* new_player_data);
Team* findTeamByName(const char* name);
Team* findTeamById(int id);
int compareTeamById(const void* a, const void* b);

void swapHeapNodes(HeapNode* a, HeapNode* b);
void maxHeapify(HeapNode heap[], int heapSize, int index);
HeapNode extractMax(HeapNode heap[], int* heapSizePtr);
void insertIntoHeap(HeapNode heap[], int* heapSizePtr, HeapNode new_node);

const char* getRoleName(PlayerRole role);
void printPlayer(PlayerData* p);
void printPlayerWithTeam(PlayerData* p);

#endif
