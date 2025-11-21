#include "player.h"

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

void printPlayerWithTeam(PlayerData* player) {
    if (player == NULL) return;
    printf("| %-5d | %-20s | %-12s | %-12s | %-6d | %-6.2f | %-7.2f | %-6d | %-6.2f | %-10.2f |\n",
           player->playerId,
           player->name,
           player->teamName,
           getRoleName(player->role),
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->wickets,
           player->economyRate,
           player->performanceIndex);
}
