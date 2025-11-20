#ifndef TEAM_H
#define TEAM_H

#include "player.h"

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


extern Team all_teams[10];




PlayerNode** getSortedListHead(Team* team, PlayerRole role);
void insertIntoSortedList(PlayerNode** list_head, PlayerNode* new_node);
void addPlayerToTeam(Team* team, PlayerData* new_player_data);

Team* findTeamByName(const char* name);
Team* findTeamById(int id);
int compareTeamById(const void* a, const void* b);

void initializeData();
void cleanup();

#endif 
