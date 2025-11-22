/*
================================================================================
SMART BUS NAVIGATION SYSTEM
Jaypee Institute of Information Technology, Noida
Department: Computer Science and Engineering
A comprehensive bus route finding system using DFS algorithm
Features: Route optimization, fare calculation, card compatibility
================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAX_STATIONS 40
#define MAX_ROUTES 1000
#define MAX_PATH_LENGTH 20
#define INFINITY_DIST 9999
// ==================== STRUCTURES AND UNIONS ====================
// Structure to represent a bus station
typedef struct {
int id;
char name[50];
char cardType[30];
int platform;
char zone[20];
} Station;
// Structure to represent a route between stations
typedef struct {
int from;
int to;
int distance;
int fare;
int travelTime;
int crowd;
} Route;
// Structure to store complete path information
typedef struct {
int stations[MAX_PATH_LENGTH];
int pathLength;
int totalDistance;
int totalFare;
int totalTime;
int avgCrowd;
} PathInfo;
// Union for flexible data storage
typedef union {
int intValue;
float floatValue;
char stringValue[50];
} FlexibleData;
// ==================== GLOBAL VARIABLES ====================
Station stations[MAX_STATIONS];
int adjacencyMatrix[MAX_STATIONS][MAX_STATIONS];
int fareMatrix[MAX_STATIONS][MAX_STATIONS];
int timeMatrix[MAX_STATIONS][MAX_STATIONS];
int crowdMatrix[MAX_STATIONS][MAX_STATIONS];
int visited[MAX_STATIONS];
int totalStations = 0;
PathInfo allPaths[MAX_ROUTES];
int pathCount = 0;
// ==================== FUNCTION PROTOTYPES ====================
void initializeSystem();
void setupStations();
void setupConnections();
void displayAllStations();
void displayStationInfo(int stationId);
void findAllRoutes(int source, int dest);
void dfsExplore(int current, int dest, int path[], int pathLen,
int dist, int fare, int time, int crowd);
void rankAndDisplayRoutes(int source, int dest);
int compareRoutesByDistance(const void *a, const void *b);
int compareRoutesByFare(const void *a, const void *b);
int compareRoutesByTime(const void *a, const void *b);
void displayPath(PathInfo *path);
void saveRoutesToFile();
void loadRoutesFromFile();
void addNewConnection();
void displayMenu();
void clearVisited();
int getStationIndexByName(char *name);
void displayConnectionMatrix();
void calculateRouteMetrics(int path[], int pathLen, PathInfo *info);
void displayDetailedRoute(PathInfo *path);
void userInteraction();
void displayStatistics();
// ==================== MAIN FUNCTION ====================
int main() {
int choice;
int source, dest;
char sourceName[50], destName[50];
printf("\n");
printf("================================================================================\n")
;
printf(" SMART BUS NAVIGATION SYSTEM - DELHI NCR\n");
printf(" Jaypee Institute of Information Technology\n");
printf("================================================================================\n")
;
printf("\n");
initializeSystem();
setupStations();
setupConnections();
while(1) {
displayMenu();
printf("\nEnter your choice: ");
scanf("%d", &choice);
switch(choice) {
case 1:
displayAllStations();
break;
case 2:
printf("\nEnter source station name: ");
scanf(" %[^\n]", sourceName);
source = getStationIndexByName(sourceName);
if(source == -1) {
printf("\nSource station not found!\n");
break;
}
printf("Enter destination station name: ");
scanf(" %[^\n]", destName);
dest = getStationIndexByName(destName);
if(dest == -1) {
printf("\nDestination station not found!\n");
break;
}
if(source == dest) {
printf("\nSource and destination cannot be same!\n");
break;
}
findAllRoutes(source, dest);
rankAndDisplayRoutes(source, dest);
break;
case 3:
printf("\nEnter station ID (0-%d): ", totalStations-1);
scanf("%d", &source);
if(source >= 0 && source < totalStations) {
displayStationInfo(source);
} else {
printf("\nInvalid station ID!\n");
}
break;
case 4:
displayConnectionMatrix();
break;
case 5:
addNewConnection();
break;
case 6:
saveRoutesToFile();
break;
case 7:
loadRoutesFromFile();
break;
case 8:
displayStatistics();
break;
case 9:
printf("\n");
printf("================================================================================\n");
printf(" Thank you for using Smart Bus Navigation System!\n");
printf(" Journey safe, travel smart!\n");
printf("================================================================================\n");
printf("\n");
exit(0);
default:
printf("\nInvalid choice! Please try again.\n");
}
}
return 0;
}
void initializeSystem() {
// Initialize all matrices with infinity/zero values
for(int i = 0; i < MAX_STATIONS; i++) {
for(int j = 0; j < MAX_STATIONS; j++) {
if(i == j) {
adjacencyMatrix[i][j] = 0;
fareMatrix[i][j] = 0;
timeMatrix[i][j] = 0;
crowdMatrix[i][j] = 0;
} else {
adjacencyMatrix[i][j] = INFINITY_DIST;
fareMatrix[i][j] = INFINITY_DIST;
timeMatrix[i][j] = INFINITY_DIST;
crowdMatrix[i][j] = 0;
}
}
visited[i] = 0;
}
pathCount = 0;
}
void setupStations() {
// Station 0
stations[0].id = 0;
strcpy(stations[0].name, "Connaught Place");
strcpy(stations[0].cardType, "Metro Card, Bus Card");
stations[0].platform = 1;
strcpy(stations[0].zone, "Central Delhi");
// Station 1
stations[1].id = 1;
strcpy(stations[1].name, "India Gate");
strcpy(stations[1].cardType, "Metro Card, Bus Card");
stations[1].platform = 1;
strcpy(stations[1].zone, "Central Delhi");
// Station 2
stations[2].id = 2;
strcpy(stations[2].name, "AIIMS");
strcpy(stations[2].cardType, "Metro Card, Bus Card");
stations[2].platform = 2;
strcpy(stations[2].zone, "South Delhi");
// Station 3
stations[3].id = 3;
strcpy(stations[3].name, "Hauz Khas");
strcpy(stations[3].cardType, "Metro Card, Bus Card");
stations[3].platform = 2;
strcpy(stations[3].zone, "South Delhi");
// Station 4
stations[4].id = 4;
strcpy(stations[4].name, "Saket");
strcpy(stations[4].cardType, "Metro Card, Bus Card");
stations[4].platform = 1;
strcpy(stations[4].zone, "South Delhi");
// Station 5
stations[5].id = 5;
strcpy(stations[5].name, "Nehru Place");
strcpy(stations[5].cardType, "Metro Card, Bus Card");
stations[5].platform = 3;
strcpy(stations[5].zone, "South Delhi");
// Station 6
stations[6].id = 6;
strcpy(stations[6].name, "Kalkaji");
strcpy(stations[6].cardType, "Metro Card, Bus Card");
stations[6].platform = 2;
strcpy(stations[6].zone, "South Delhi");
// Station 7
stations[7].id = 7;
strcpy(stations[7].name, "Lajpat Nagar");
strcpy(stations[7].cardType, "Metro Card, Bus Card");
stations[7].platform = 2;
strcpy(stations[7].zone, "South Delhi");
// Station 8
stations[8].id = 8;
strcpy(stations[8].name, "Kashmere Gate");
strcpy(stations[8].cardType, "Metro Card, Bus Card, ISBT Pass");
stations[8].platform = 4;
strcpy(stations[8].zone, "North Delhi");
// Station 9
stations[9].id = 9;
strcpy(stations[9].name, "Red Fort");
strcpy(stations[9].cardType, "Metro Card, Bus Card");
stations[9].platform = 1;
strcpy(stations[9].zone, "Old Delhi");
// Station 10
stations[10].id = 10;
strcpy(stations[10].name, "Chandni Chowk");
strcpy(stations[10].cardType, "Metro Card, Bus Card");
stations[10].platform = 2;
strcpy(stations[10].zone, "Old Delhi");
// Station 11
stations[11].id = 11;
strcpy(stations[11].name, "Civil Lines");
strcpy(stations[11].cardType, "Metro Card, Bus Card");
stations[11].platform = 1;
strcpy(stations[11].zone, "North Delhi");
// Station 12
stations[12].id = 12;
strcpy(stations[12].name, "Azadpur");
strcpy(stations[12].cardType, "Metro Card, Bus Card");
stations[12].platform = 2;
strcpy(stations[12].zone, "North Delhi");
// Station 13
stations[13].id = 13;
strcpy(stations[13].name, "Pitampura");
strcpy(stations[13].cardType, "Metro Card, Bus Card");
stations[13].platform = 2;
strcpy(stations[13].zone, "North West Delhi");
// Station 14
stations[14].id = 14;
strcpy(stations[14].name, "Rohini");
strcpy(stations[14].cardType, "Metro Card, Bus Card");
stations[14].platform = 3;
strcpy(stations[14].zone, "North West Delhi");
// Station 15
stations[15].id = 15;
strcpy(stations[15].name, "Dwarka");
strcpy(stations[15].cardType, "Metro Card, Bus Card");
stations[15].platform = 3;
strcpy(stations[15].zone, "South West Delhi");
// Station 16
stations[16].id = 16;
strcpy(stations[16].name, "IGI Airport");
strcpy(stations[16].cardType, "Metro Card, Bus Card, Airport Pass");
stations[16].platform = 4;
strcpy(stations[16].zone, "South West Delhi");
// Station 17
stations[17].id = 17;
strcpy(stations[17].name, "Rajouri Garden");
strcpy(stations[17].cardType, "Metro Card, Bus Card");
stations[17].platform = 2;
strcpy(stations[17].zone, "West Delhi");
// Station 18
stations[18].id = 18;
strcpy(stations[18].name, "Janakpuri");
strcpy(stations[18].cardType, "Metro Card, Bus Card");
stations[18].platform = 2;
strcpy(stations[18].zone, "West Delhi");
// Station 19
stations[19].id = 19;
strcpy(stations[19].name, "Uttam Nagar");
strcpy(stations[19].cardType, "Metro Card, Bus Card");
stations[19].platform = 2;
strcpy(stations[19].zone, "West Delhi");
// Station 20
stations[20].id = 20;
strcpy(stations[20].name, "Noida Sector 15");
strcpy(stations[20].cardType, "Metro Card, Bus Card");
stations[20].platform = 2;
strcpy(stations[20].zone, "Noida");
// Station 21
stations[21].id = 21;
strcpy(stations[21].name, "Noida Sector 18");
strcpy(stations[21].cardType, "Metro Card, Bus Card");
stations[21].platform = 3;
strcpy(stations[21].zone, "Noida");
// Station 22
stations[22].id = 22;
strcpy(stations[22].name, "Noida Sector 62");
strcpy(stations[22].cardType, "Metro Card, Bus Card");
stations[22].platform = 2;
strcpy(stations[22].zone, "Noida");
// Station 23
stations[23].id = 23;
strcpy(stations[23].name, "Greater Noida");
strcpy(stations[23].cardType, "Metro Card, Bus Card");
stations[23].platform = 3;
strcpy(stations[23].zone, "Greater Noida");
// Station 24
stations[24].id = 24;
strcpy(stations[24].name, "Vaishali");
strcpy(stations[24].cardType, "Metro Card, Bus Card");
stations[24].platform = 3;
strcpy(stations[24].zone, "Ghaziabad");
// Station 25
stations[25].id = 25;
strcpy(stations[25].name, "Anand Vihar");
strcpy(stations[25].cardType, "Metro Card, Bus Card, ISBT Pass");
stations[25].platform = 4;
strcpy(stations[25].zone, "East Delhi");
// Station 26
stations[26].id = 26;
strcpy(stations[26].name, "Preet Vihar");
strcpy(stations[26].cardType, "Metro Card, Bus Card");
stations[26].platform = 2;
strcpy(stations[26].zone, "East Delhi");
// Station 27
stations[27].id = 27;
strcpy(stations[27].name, "Mayur Vihar");
strcpy(stations[27].cardType, "Metro Card, Bus Card");
stations[27].platform = 2;
strcpy(stations[27].zone, "East Delhi");
// Station 28
stations[28].id = 28;
strcpy(stations[28].name, "Faridabad");
strcpy(stations[28].cardType, "Metro Card, Bus Card");
stations[28].platform = 3;
strcpy(stations[28].zone, "Faridabad");
// Station 29
stations[29].id = 29;
strcpy(stations[29].name, "Gurgaon Cyber City");
strcpy(stations[29].cardType, "Metro Card, Bus Card");
stations[29].platform = 4;
strcpy(stations[29].zone, "Gurgaon");
// Station 30
stations[30].id = 30;
strcpy(stations[30].name, "MG Road Gurgaon");
strcpy(stations[30].cardType, "Metro Card, Bus Card");
stations[30].platform = 2;
strcpy(stations[30].zone, "Gurgaon");
// Station 31
stations[31].id = 31;
strcpy(stations[31].name, "Sikanderpur");
strcpy(stations[31].cardType, "Metro Card, Bus Card");
stations[31].platform = 2;
strcpy(stations[31].zone, "Gurgaon");
// Station 32
stations[32].id = 32;
strcpy(stations[32].name, "Botanical Garden");
strcpy(stations[32].cardType, "Metro Card, Bus Card");
stations[32].platform = 3;
strcpy(stations[32].zone, "Noida");
// Station 33
stations[33].id = 33;
strcpy(stations[33].name, "Karol Bagh");
strcpy(stations[33].cardType, "Metro Card, Bus Card");
stations[33].platform = 2;
strcpy(stations[33].zone, "Central Delhi");
// Station 34
stations[34].id = 34;
strcpy(stations[34].name, "Shahdara");
strcpy(stations[34].cardType, "Metro Card, Bus Card");
stations[34].platform = 2;
strcpy(stations[34].zone, "East Delhi");
// Station 35
stations[35].id = 35;
strcpy(stations[35].name, "Mundka");
strcpy(stations[35].cardType, "Metro Card, Bus Card");
stations[35].platform = 2;
strcpy(stations[35].zone, "West Delhi");
// Station 36
stations[36].id = 36;
strcpy(stations[36].name, "Badarpur");
strcpy(stations[36].cardType, "Metro Card, Bus Card");
stations[36].platform = 2;
strcpy(stations[36].zone, "South Delhi");
// Station 37
stations[37].id = 37;
strcpy(stations[37].name, "Okhla");
strcpy(stations[37].cardType, "Metro Card, Bus Card");
stations[37].platform = 2;
strcpy(stations[37].zone, "South Delhi");
// Station 38
stations[38].id = 38;
strcpy(stations[38].name, "Safdarjung");
strcpy(stations[38].cardType, "Metro Card, Bus Card");
stations[38].platform = 1;
strcpy(stations[38].zone, "South Delhi");
// Station 39
stations[39].id = 39;
strcpy(stations[39].name, "Vasant Vihar");
strcpy(stations[39].cardType, "Metro Card, Bus Card");
stations[39].platform = 1;
strcpy(stations[39].zone, "South West Delhi");
totalStations = 40;
}
void setupConnections() {
// Helper function to add bidirectional connection
void addConnection(int from, int to, int dist, int fare, int time, int crowd) {
adjacencyMatrix[from][to] = dist;
adjacencyMatrix[to][from] = dist;
fareMatrix[from][to] = fare;
fareMatrix[to][from] = fare;
timeMatrix[from][to] = time;
timeMatrix[to][from] = time;
crowdMatrix[from][to] = crowd;
crowdMatrix[to][from] = crowd;
}
// Connaught Place connections
addConnection(0, 1, 3, 10, 8, 7);
addConnection(0, 8, 5, 15, 12, 8);
addConnection(0, 33, 2, 8, 5, 9);
// India Gate connections
addConnection(1, 2, 4, 12, 10, 6);
addConnection(1, 9, 3, 10, 8, 7);
// AIIMS connections
addConnection(2, 3, 3, 10, 8, 7);
addConnection(2, 38, 2, 8, 6, 5);
// Hauz Khas connections
addConnection(3, 4, 4, 12, 10, 6);
addConnection(3, 5, 5, 15, 12, 7);
// Saket connections
addConnection(4, 39, 3, 10, 8, 5);
addConnection(4, 5, 6, 18, 15, 6);
// Nehru Place connections
addConnection(5, 6, 3, 10, 8, 8);
addConnection(5, 7, 2, 8, 6, 7);
// Kalkaji connections
addConnection(6, 7, 2, 8, 6, 7);
addConnection(6, 36, 5, 15, 12, 6);
// Lajpat Nagar connections
addConnection(7, 26, 8, 20, 18, 7);
addConnection(7, 37, 4, 12, 10, 6);
// Kashmere Gate connections
addConnection(8, 9, 2, 8, 6, 9);
addConnection(8, 10, 3, 10, 8, 10);
addConnection(8, 11, 3, 10, 8, 7);
// Red Fort connections
addConnection(9, 10, 1, 5, 4, 9);
// Chandni Chowk connections
addConnection(10, 11, 4, 12, 10, 8);
// Civil Lines connections
addConnection(11, 12, 5, 15, 12, 6);
// Azadpur connections
addConnection(12, 13, 6, 18, 15, 7);
// Pitampura connections
addConnection(13, 14, 5, 15, 12, 6);
// Rohini connections
addConnection(14, 35, 8, 20, 18, 5);
// Dwarka connections
addConnection(15, 16, 4, 12, 10, 7);
addConnection(15, 18, 6, 18, 15, 6);
// IGI Airport connections
addConnection(16, 29, 15, 50, 35, 8);
// Rajouri Garden connections
addConnection(17, 18, 3, 10, 8, 7);
addConnection(17, 33, 4, 12, 10, 8);
// Janakpuri connections
addConnection(18, 19, 4, 12, 10, 7);
addConnection(18, 15, 6, 18, 15, 6);
// Uttam Nagar connections
addConnection(19, 35, 7, 20, 16, 6);
// Noida Sector 15 connections
addConnection(20, 21, 2, 8, 6, 8);
addConnection(20, 32, 5, 15, 12, 7);
// Noida Sector 18 connections
addConnection(21, 22, 8, 20, 18, 7);
addConnection(21, 32, 3, 10, 8, 8);
// Noida Sector 62 connections
addConnection(22, 23, 12, 30, 25, 6);
// Vaishali connections
addConnection(24, 25, 5, 15, 12, 7);
addConnection(24, 27, 4, 12, 10, 7);
// Anand Vihar connections
addConnection(25, 26, 3, 10, 8, 9);
addConnection(25, 34, 6, 18, 15, 8);
// Preet Vihar connections
addConnection(26, 27, 2, 8, 6, 8);
// Mayur Vihar connections
addConnection(27, 32, 7, 20, 16, 7);
// Faridabad connections
addConnection(28, 36, 10, 25, 22, 6);
addConnection(28, 37, 8, 20, 18, 6);
// Gurgaon Cyber City connections
addConnection(29, 30, 4, 12, 10, 8);
addConnection(29, 31, 3, 10, 8, 9);
// MG Road Gurgaon connections
addConnection(30, 31, 2, 8, 6, 8);
// Sikanderpur connections
addConnection(31, 15, 8, 20, 18, 7);
// Botanical Garden connections
addConnection(32, 20, 5, 15, 12, 7);
addConnection(32, 21, 3, 10, 8, 8);
// Karol Bagh connections
addConnection(33, 17, 4, 12, 10, 8);
addConnection(33, 8, 6, 18, 15, 9);
// Shahdara connections
addConnection(34, 25, 6, 18, 15, 8);
addConnection(34, 10, 8, 20, 18, 8);
// Badarpur connections
addConnection(36, 37, 3, 10, 8, 6);
// Okhla connections
addConnection(37, 5, 4, 12, 10, 6);
// Safdarjung connections
addConnection(38, 39, 4, 12, 10, 5);
// Vasant Vihar connections
addConnection(39, 16, 10, 25, 20, 6);
}
void displayMenu() {
printf("\n");
printf("================================================================================\n")
;
printf(" MAIN MENU\n");
printf("================================================================================\n")
;
printf(" 1. Display All Stations\n");
printf(" 2. Find Routes Between Stations\n");
printf(" 3. Display Station Information\n");
printf(" 4. Display Connection Matrix\n");
printf(" 5. Add New Connection\n");
printf(" 6. Save Routes to File\n");
printf(" 7. Load Routes from File\n");
printf(" 8. Display System Statistics\n");
printf(" 9. Exit\n");
printf("================================================================================\n")
;
}
void displayAllStations() {
printf("\n");
printf("================================================================================\n")
;
printf(" ALL AVAILABLE STATIONS\n");
printf("================================================================================\n")
;
printf("%-4s %-25s %-30s %-8s %-20s\n", "ID", "Station Name", "Card Types", "Platform", "Zone");
printf("--------------------------------------------------------------------------------\n")
;
for(int i = 0; i < totalStations; i++) {
printf("%-4d %-25s %-30s %-8d %-20s\n",
stations[i].id,
stations[i].name,
stations[i].cardType,
stations[i].platform,
stations[i].zone);
}
printf("================================================================================\n")
;
}
void displayStationInfo(int stationId) {
if(stationId < 0 || stationId >= totalStations) {
printf("\nInvalid station ID!\n");
return;
}
printf("\n");
printf("================================================================================\n")
;
printf(" STATION INFORMATION\n");
printf("================================================================================\n")
;
printf(" Station ID : %d\n", stations[stationId].id);
printf(" Station Name : %s\n", stations[stationId].name);
printf(" Card Types : %s\n", stations[stationId].cardType);
printf(" Platform Number : %d\n", stations[stationId].platform);
printf(" Zone : %s\n", stations[stationId].zone);
printf("================================================================================\n")
;
printf(" Connected Stations:\n");
printf("--------------------------------------------------------------------------------\n")
;
int connectionCount = 0;
for(int i = 0; i < totalStations; i++) {
if(adjacencyMatrix[stationId][i] != INFINITY_DIST && adjacencyMatrix[stationId][i] != 0) {
printf(" -> %s (Distance: %d km, Fare: Rs %d, Time: %d min)\n",
stations[i].name,
adjacencyMatrix[stationId][i],
fareMatrix[stationId][i],
timeMatrix[stationId][i]);
connectionCount++;
}
}
if(connectionCount == 0) {
printf(" No direct connections available.\n");
}
printf("================================================================================\n")
;
}
void findAllRoutes(int source, int dest) {
pathCount = 0;
clearVisited();
int path[MAX_PATH_LENGTH];
path[0] = source;
visited[source] = 1;
printf("\nSearching for routes from %s to %s...\n",
stations[source].name, stations[dest].name);
dfsExplore(source, dest, path, 1, 0, 0, 0, 0);
if(pathCount == 0) {
printf("\nNo routes found between these stations!\n");
} else {
printf("\nFound %d possible route(s).\n", pathCount);
}
}
void dfsExplore(int current, int dest, int path[], int pathLen,
int dist, int fare, int time, int crowd) {
if(pathCount >= MAX_ROUTES) return;
if(current == dest) {
// Found a complete path
PathInfo newPath;
for(int i = 0; i < pathLen; i++) {
newPath.stations[i] = path[i];
}
newPath.pathLength = pathLen;
newPath.totalDistance = dist;
newPath.totalFare = fare;
newPath.totalTime = time;
newPath.avgCrowd = (pathLen > 1) ? crowd / (pathLen - 1) : 0;
allPaths[pathCount++] = newPath;
return;
}
for(int i = 0; i < totalStations; i++) {
if(!visited[i] && adjacencyMatrix[current][i] != INFINITY_DIST &&
adjacencyMatrix[current][i] != 0 && pathLen < MAX_PATH_LENGTH) {
visited[i] = 1;
path[pathLen] = i;
dfsExplore(i, dest, path, pathLen + 1,
dist + adjacencyMatrix[current][i],
fare + fareMatrix[current][i],
time + timeMatrix[current][i],
crowd + crowdMatrix[current][i]);
visited[i] = 0;
}
}
}
void rankAndDisplayRoutes(int source, int dest) {
if(pathCount == 0) return;
printf("\n");
printf("================================================================================\n")
;
printf(" ROUTES RANKED BY DISTANCE\n");
printf("================================================================================\n")
;
qsort(allPaths, pathCount, sizeof(PathInfo), compareRoutesByDistance);
for(int i = 0; i < pathCount && i < 5; i++) {
printf("\nRoute #%d (Distance: %d km)\n", i+1, allPaths[i].totalDistance);
printf("--------------------------------------------------------------------------------\n");
displayDetailedRoute(&allPaths[i]);
}
printf("\n");
printf("================================================================================\n")
;
printf(" ROUTES RANKED BY FARE\n");
printf("================================================================================\n")
;
qsort(allPaths, pathCount, sizeof(PathInfo), compareRoutesByFare);
for(int i = 0; i < pathCount && i < 5; i++) {
printf("\nRoute #%d (Fare: Rs %d)\n", i+1, allPaths[i].totalFare);
printf("--------------------------------------------------------------------------------\n");
displayDetailedRoute(&allPaths[i]);
}
printf("\n");
printf("================================================================================\n")
;
printf(" ROUTES RANKED BY TIME\n");
printf("================================================================================\n")
;
qsort(allPaths, pathCount, sizeof(PathInfo), compareRoutesByTime);
for(int i = 0; i < pathCount && i < 3; i++) {
printf("\nRoute #%d (Time: %d minutes)\n", i+1, allPaths[i].totalTime);
printf("--------------------------------------------------------------------------------\n");
displayDetailedRoute(&allPaths[i]);
}
}
void displayDetailedRoute(PathInfo *path) {
printf(" Path: ");
for(int i = 0; i < path->pathLength; i++) {
printf("%s", stations[path->stations[i]].name);
if(i < path->pathLength - 1) {
printf(" -> ");
}
}
printf("\n");
printf(" Total Distance: %d km\n", path->totalDistance);
printf(" Total Fare: Rs %d\n", path->totalFare);
printf(" Total Time: %d minutes\n", path->totalTime);
printf(" Average Crowd Level: %d/10\n", path->avgCrowd);
printf(" Number of Stops: %d\n", path->pathLength - 1);
}
int compareRoutesByDistance(const void *a, const void *b) {
PathInfo *pathA = (PathInfo *)a;
PathInfo *pathB = (PathInfo *)b;
return pathA->totalDistance - pathB->totalDistance;
}
int compareRoutesByFare(const void *a, const void *b) {
PathInfo *pathA = (PathInfo *)a;
PathInfo *pathB = (PathInfo *)b;
return pathA->totalFare - pathB->totalFare;
}
int compareRoutesByTime(const void *a, const void *b) {
PathInfo *pathA = (PathInfo *)a;
PathInfo *pathB = (PathInfo *)b;
return pathA->totalTime - pathB->totalTime;
}
void saveRoutesToFile() {
FILE *fp = fopen("bus_routes.dat", "wb");
if(fp == NULL) {
printf("\nError opening file for writing!\n");
return;
}
fwrite(&totalStations, sizeof(int), 1, fp);
fwrite(stations, sizeof(Station), totalStations, fp);
fwrite(adjacencyMatrix, sizeof(int), MAX_STATIONS * MAX_STATIONS, fp);
fwrite(fareMatrix, sizeof(int), MAX_STATIONS * MAX_STATIONS, fp);
fwrite(timeMatrix, sizeof(int), MAX_STATIONS * MAX_STATIONS, fp);
fwrite(crowdMatrix, sizeof(int), MAX_STATIONS * MAX_STATIONS, fp);
fclose(fp);
printf("\nRoutes saved successfully to 'bus_routes.dat'!\n");
}
void loadRoutesFromFile() {
FILE *fp = fopen("bus_routes.dat", "rb");
if(fp == NULL) {
printf("\nError opening file for reading! File may not exist.\n");
return;
}
fread(&totalStations, sizeof(int), 1, fp);
fread(stations, sizeof(Station), totalStations, fp);
fread(adjacencyMatrix, sizeof(int), MAX_STATIONS * MAX_STATIONS, fp);
fread(fareMatrix, sizeof(int), MAX_STATIONS * MAX_STATIONS, fp);
fread(timeMatrix, sizeof(int), MAX_STATIONS * MAX_STATIONS, fp);
fread(crowdMatrix, sizeof(int), MAX_STATIONS * MAX_STATIONS, fp);
fclose(fp);
printf("\nRoutes loaded successfully from 'bus_routes.dat'!\n");
}
void clearVisited() {
for(int i = 0; i < MAX_STATIONS; i++) {
visited[i] = 0;
}
}
int getStationIndexByName(char *name) {
for(int i = 0; i < totalStations; i++) {
if(strcasecmp(stations[i].name, name) == 0) {
return i;
}
}
return -1;
}
void displayConnectionMatrix() {
printf("\n");
printf("================================================================================\n")
;
printf(" DISTANCE MATRIX (First 10 stations)\n");
printf("================================================================================\n")
;
printf(" ");
for(int i = 0; i < 10; i++) {
printf("%4d ", i);
}
printf("\n");
for(int i = 0; i < 10; i++) {
printf("%4d ", i);
for(int j = 0; j < 10; j++) {
if(adjacencyMatrix[i][j] == INFINITY_DIST) {
printf(" -- ");
} else {
printf("%4d ", adjacencyMatrix[i][j]);
}
}
printf("\n");
}
printf("================================================================================\n")
;
}
void addNewConnection() {
int from, to, dist, fare, time, crowd;
printf("\n");
printf("================================================================================\n")
;
printf(" ADD NEW CONNECTION\n");
printf("================================================================================\n")
;
printf("Enter source station ID (0-%d): ", totalStations-1);
scanf("%d", &from);
if(from < 0 || from >= totalStations) {
printf("\nInvalid source station ID!\n");
return;
}
printf("Enter destination station ID (0-%d): ", totalStations-1);
scanf("%d", &to);
if(to < 0 || to >= totalStations || to == from) {
printf("\nInvalid destination station ID!\n");
return;
}
printf("Enter distance (km): ");
scanf("%d", &dist);
printf("Enter fare (Rs): ");
scanf("%d", &fare);
printf("Enter travel time (minutes): ");
scanf("%d", &time);
printf("Enter crowd level (0-10): ");
scanf("%d", &crowd);
adjacencyMatrix[from][to] = dist;
adjacencyMatrix[to][from] = dist;
fareMatrix[from][to] = fare;
fareMatrix[to][from] = fare;
timeMatrix[from][to] = time;
timeMatrix[to][from] = time;
crowdMatrix[from][to] = crowd;
crowdMatrix[to][from] = crowd;
printf("\nConnection added successfully!\n");
printf(" %s <-> %s\n", stations[from].name, stations[to].name);
printf(" Distance: %d km, Fare: Rs %d, Time: %d min\n", dist, fare, time);
}
void displayStatistics() {
int totalConnections = 0;
int totalDistance = 0;
int maxDistance = 0;
int minDistance = INFINITY_DIST;
for(int i = 0; i < totalStations; i++) {
for(int j = i+1; j < totalStations; j++) {
if(adjacencyMatrix[i][j] != INFINITY_DIST && adjacencyMatrix[i][j] != 0) {
totalConnections++;
totalDistance += adjacencyMatrix[i][j];
if(adjacencyMatrix[i][j] > maxDistance) {
maxDistance = adjacencyMatrix[i][j];
}
if(adjacencyMatrix[i][j] < minDistance) {
minDistance = adjacencyMatrix[i][j];
}
}
}
}
printf("\n");
printf("================================================================================\n")
;
printf(" SYSTEM STATISTICS\n");
printf("================================================================================\n")
;
printf(" Total Stations : %d\n", totalStations);
printf(" Total Connections : %d\n", totalConnections);
printf(" Average Distance : %d km\n",
totalConnections > 0 ? totalDistance / totalConnections : 0);
printf(" Maximum Distance : %d km\n", maxDistance);
printf(" Minimum Distance : %d km\n",
minDistance == INFINITY_DIST ? 0 : minDistance);
printf(" Network Coverage : Delhi NCR\n");
printf(" Supported Card Types : Metro Card, Bus Card, ISBT Pass, Airport Pass\n");
printf("================================================================================\n")
;
}
