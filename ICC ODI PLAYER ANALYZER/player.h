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


float calculatePerformanceIndex(PlayerData* player);
const char* getRoleName(PlayerRole role);
void printPlayer(PlayerData* player);
void printPlayerWithTeam(PlayerData* player);

#endif 
