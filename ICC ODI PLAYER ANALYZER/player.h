#ifndef PLAYER_H
#define PLAYER_H

typedef enum {
    BATSMAN,
    BOWLER,
    ALL_ROUNDER,
    WICKET_KEEPER
} Role;

typedef struct {
    int id;
    char name[50];
    Role role;
    int runs;
    int wickets;
    float strikeRate;
    float average;
    float performanceIndex;
} PlayerData;

typedef struct PlayerNode {
    PlayerData data;
    struct PlayerNode* next;
} PlayerNode;

typedef struct {
    char name[50];
    PlayerNode* sortedListHead;

    float battingSRSum;
    float bowlingEcoSum;
    int battingSRCount;
    int bowlingEcoCount;
} Team;

extern Team all_teams[10];
extern int teamCount;

void initializeData();
void insertIntoSortedList(PlayerNode** head, PlayerData player);
void printPlayer(PlayerData p);
void printPlayerWithTeam(PlayerData p, char* teamName);
void printAllTeams();

void menu();
Team* findTeamByName(char* name);
void addPlayerToTeam(Team* team, PlayerData p);

float calculatePerformanceIndex(PlayerData p);

#endif
