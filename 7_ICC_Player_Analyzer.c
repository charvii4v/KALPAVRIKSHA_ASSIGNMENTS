
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "Players_data.h"

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


Team all_teams[10]; 


float calculatePerformanceIndex(PlayerData* player);
void insertIntoSortedList(PlayerNode** list_head, PlayerNode* new_node);
PlayerNode** getSortedListHead(Team* team, PlayerRole role);
void addPlayerToTeam(Team* team, PlayerData* new_player_data);
Team* findTeamByName(const char* name);
int compareTeamById(const void* a, const void* b);
Team* findTeamById(int id);
void printPlayer(PlayerData* p);
void printPlayerWithTeam(PlayerData* p);


float calculatePerformanceIndex(PlayerData* player) {
    if (player == NULL) return 0.0;

    switch (player->role) {
        case ROLE_BATSMAN:
            
            return (player->battingAverage * player->strikeRate) / 100.0f;
        case ROLE_BOWLER:
            
            return (player->wickets * 2) + (100.0f - player->economyRate);
        case ROLE_ALLROUNDER:
            return ((player->battingAverage * player->strikeRate) / 100.0f) + (player->wickets * 2);
        default:
            return 0.0;
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
    switch (role) {
        case ROLE_BATSMAN:
            return &team->batsmen_sorted_list;
        case ROLE_BOWLER:
            return &team->bowlers_sorted_list;
        case ROLE_ALLROUNDER:
            return &team->allrounders_sorted_list;
    }
    return NULL; 
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
        all_teams[i].battingSRSum = 0.0;
        all_teams[i].battingSRCount = 0;
        all_teams[i].all_players_list = NULL;
        all_teams[i].batsmen_sorted_list = NULL;
        all_teams[i].bowlers_sorted_list = NULL;
        all_teams[i].allrounders_sorted_list = NULL;
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

void handleAddPlayer() {
    int teamId, roleChoice;
    printf("\nEnter Team ID to add player: ");
    if (scanf("%d", &teamId) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); // Clear buffer
        return;
    }

    Team* team = findTeamById(teamId);
    if (team == NULL) {
        printf("Team with ID %d not found.\n", teamId);
        return;
    }

    PlayerData* new_player = (PlayerData*)malloc(sizeof(PlayerData));
    if (new_player == NULL) {
        printf("Failed to allocate memory for new player.\n");
        return;
    }

    printf("Enter Player Details:\n");
    printf("Player ID: ");
    scanf("%d", &new_player->playerId);
    
    printf("Name: ");
    scanf(" %[^\n]", new_player->name); // Read string with spaces
    
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

void printPlayerWithTeam(PlayerData* p) {
    if (p == NULL) return;
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


void handleDisplayTeamPlayers() {
    int teamId;
    printf("\nEnter Team ID: ");
     if (scanf("%d", &teamId) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); // Clear buffer
        return;
    }

    Team* team = findTeamById(teamId);
    if (team == NULL) {
        printf("Team with ID %d not found.\n", teamId);
        return;
    }

    printf("\nPlayers of Team %s:\n", team->name);
    printf("===================================================================================================================\n");
    printf("| %-5s | %-20s | %-12s | %-6s | %-6s | %-7s | %-6s | %-6s | %-10s |\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    PlayerNode* current = team->all_players_list;
    if (current == NULL) {
        printf("| No players found in this team.                                                                                  |\n");
    }
    while (current != NULL) {
        printPlayer(current->data);
        current = current->next;
    }
    printf("===================================================================================================================\n");

    double avg_sr = (team->battingSRCount == 0) ? 0.0 : (team->battingSRSum / team->battingSRCount);
    printf("Total Players: %d\n", team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", avg_sr);
}


int compareTeamsBySR(const void* a, const void* b) {
    const Team* teamA = *(const Team**)a;
    const Team* teamB = *(const Team**)b;

    double avg_sr_A = (teamA->battingSRCount == 0) ? 0.0 : (teamA->battingSRSum / teamA->battingSRCount);
    double avg_sr_B = (teamB->battingSRCount == 0) ? 0.0 : (teamB->battingSRSum / teamB->battingSRCount);

    if (avg_sr_B > avg_sr_A) return 1;
    if (avg_sr_B < avg_sr_A) return -1;
    return 0;
}


void handleDisplayTeamsBySR() {
    Team* team_pointers[10];
    for (int teamIndex = 0; teamIndex < 10; teamIndex++) {
        team_pointers[teamIndex] = &all_teams[teamIndex];
    }

    qsort(team_pointers, 10, sizeof(Team*), compareTeamsBySR);

    printf("\nTeams Sorted by Average Batting Strike Rate:\n");
    printf("==============================================================\n");
    printf("| %-4s | %-20s | %-12s | %-13s |\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("--------------------------------------------------------------\n");

    for (int teamIndex = 0; teamIndex < 10; teamIndex++) {
        Team* team = team_pointers[teamIndex];
        double avg_sr = (team->battingSRCount == 0) ? 0.0 : (team->battingSRSum / team->battingSRCount);
        printf("| %-4d | %-20s | %-12.2f | %-13d |\n",
               team->teamId,
               team->name,
               avg_sr,
               team->totalPlayers);
    }
    printf("==============================================================\n");
}


void handleDisplayTopK() {
    int teamId, roleChoice, k;
    
    printf("\nEnter Team ID: ");
    if (scanf("%d", &teamId) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); return;
    }
    Team* team = findTeamById(teamId);
    if (team == NULL) {
        printf("Team with ID %d not found.\n", teamId);
        return;
    }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1 || roleChoice < 1 || roleChoice > 3) {
         printf("Invalid role.\n");
         while (getchar() != '\n'); return;
    }

    printf("Enter number of players (K): ");
    if (scanf("%d", &k) != 1 || k < 1) {
        printf("Invalid number K.\n");
        while (getchar() != '\n'); return;
    }

    PlayerNode** list_head_ptr = getSortedListHead(team, (PlayerRole)roleChoice);
    if (list_head_ptr == NULL || *list_head_ptr == NULL) {
        printf("No players found for this role in Team %s.\n", team->name);
        return;
    }

    printf("\nTop %d %s(s) of Team %s:\n", k, getRoleName((PlayerRole)roleChoice), team->name);
    printf("===================================================================================================================\n");
    printf("| %-5s | %-20s | %-12s | %-6s | %-6s | %-7s | %-6s | %-6s | %-10s |\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    PlayerNode* current = *list_head_ptr;
    int playersPrinted = 0;
    
    for (playersPrinted = 0; playersPrinted < k && current != NULL; playersPrinted++) {
        printPlayer(current->data);
        current = current->next;
    }
    
    if (playersPrinted == 0) {
         printf("| No players found for this role.                                                                                 |\n");
    }
    printf("===================================================================================================================\n");
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
        
        printf("Heap is full\n");
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


HeapNode extractMax(HeapNode heap[], int* n) {
    if (*n <= 0) return (HeapNode){NULL, -1, NULL};
    
    HeapNode max = heap[0];
    heap[0] = heap[*n - 1];
    (*n)--;
    maxHeapify(heap, *n, 0);
    return max;
}


void insertIntoHeap(HeapNode heap[], int* n, HeapNode new_node) {
    if (*n == 10) {
        
        printf("Heap is full\n");
        return;
    }
    
    (*n)++;
    int i = *n - 1;
    heap[i] = new_node;

    while (i != 0 && heap[(i - 1) / 2].player->performanceIndex < heap[i].player->performanceIndex) {
        swapHeapNodes(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void handleDisplayAllByRole() {
    int roleChoice;
    printf("\nEnter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (scanf("%d", &roleChoice) != 1 || roleChoice < 1 || roleChoice > 3) {
         printf("Invalid role.\n");
         while (getchar() != '\n'); return;
    }

    printf("\nAll %s(s) Across All Teams:\n", getRoleName((PlayerRole)roleChoice));
    printf("=============================================================================================================================\n");
    printf("| %-5s | %-20s | %-12s | %-12s | %-6s | %-6s | %-7s | %-6s | %-6s | %-10s |\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");

    HeapNode heap[10];
    int heap_size = 0;

    
    for (int teamIndex = 0; teamIndex < 10; teamIndex++) {
        PlayerNode** list_head_ptr = getSortedListHead(&all_teams[teamIndex], (PlayerRole)roleChoice);
        if (list_head_ptr && *list_head_ptr) {
            HeapNode new_node = {
                (*list_head_ptr)->data,
                teamIndex,
                (*list_head_ptr)->next
            };
            insertIntoHeap(heap, &heap_size, new_node);
        }
    }

    if (heap_size == 0) {
        printf("| No players found for this role across all teams.                                                                        |\n");
        printf("=============================================================================================================================\n");
        return;
    }

  
    while (heap_size > 0) {
        
        HeapNode max_node = extractMax(heap, &heap_size);
        
        
        printPlayerWithTeam(max_node.player);

        
        if (max_node.next_node != NULL) {
            HeapNode next_in_list = {
                max_node.next_node->data,
                max_node.team_index,
                max_node.next_node->next
            };
            insertIntoHeap(heap, &heap_size, next_in_list);
        }
    }
    printf("=============================================================================================================================\n");
}


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
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n'); 
        return -1; 
    }
    return choice;
}

int main() {
   
    initializeData();
    
    int choice = 0;
    while (choice != 6) {
        choice = showMenu();
        switch (choice) {
            case 1:
                handleAddPlayer();
                break;
            case 2:
                handleDisplayTeamPlayers();
                break;
            case 3:
                handleDisplayTeamsBySR();
                break;
            case 4:
                handleDisplayTopK();
                break;
            case 5:
                handleDisplayAllByRole();
                break;
            case 6:
                printf("Exiting... Thank you!\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    }

    cleanup();
    
    return 0;
}