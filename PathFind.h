//Josh Stone JStone3
#pragma once
#include <memory>
#include <iostream>
#include <deque>
#include <fstream>
#include <list>
#include <algorithm>
#include <vector>
#include <sstream>
#include <TL-Engine.h>
using namespace std;
using namespace tle;
//Structure used by both each node to store information.
struct SCoords
{
	int x;//X coordinate.
	int y;//Y coordinate.
	SCoords* pParent;//Parent is the node that was used to generate the current node.
	int score;//Score is the combined cost of a node along with its heuristic value.
	int cost;//Cost is the cost of moving from the value assigned to a square based on its terrain type. This value is gathered from the file of the current map.
};
enum EMaps { mMap, dMap };//Enum used to indicate which map is being searched.
//Function used to check whether the goal has been reached.
bool CheckFinish(SCoords* currentNode, int x, int y);
//Function used to search a list for a specific value.
//Returns true if node is on list. If node isn't on list then returns false.
bool FindValue(deque <unique_ptr < SCoords > > &list, SCoords n);
//Function used to search closed list and check whether a node has already been visited/generated.
//This function is used in the event that we find a cheaper route of reaching a specific node.
//If this is true then we update the node using the new values and add it to the open list.
void FindValueRemove(deque <unique_ptr < SCoords > > &closedList, SCoords n, deque <unique_ptr < SCoords > > &openList);
//Function used to add a new node to the open list.
void InsertNode(deque <unique_ptr < SCoords > > &openList, SCoords node);
//Function used to calculate the hueristic of a node. The heuristic is the absolute distance from the end to a node measured square by square.
int HeuristicCalc(SCoords mapEnd, SCoords n);
//Function used in the sorting algorithm to sort the list by score from lowest to highest.
//Returns true or false based on which score is higher.
bool CompareCoords(unique_ptr<SCoords>& lhs, unique_ptr<SCoords>& rhs);
//Function used to generate the new nodes using the current node from the open list.
void GenerateNodes(SCoords &north, SCoords &east, SCoords &south, SCoords &west, SCoords* currentNode);
//Function used to check if a node is already on the open or the closed list and then return the cost of that node.
//This will be used to determine whether to update the values of a node if it has already been visited.
int GetCost(deque <unique_ptr < SCoords > > &openList, deque <unique_ptr < SCoords > > &closedList, SCoords node);
//Function used to calculate the path from start to finish.
void CalculatePath(SCoords* currentNode, int mapArray[10][10], int newCost, int existingCost, SCoords &node, SCoords mapEnd,
	deque <unique_ptr < SCoords > > &openList, deque <unique_ptr < SCoords > > &closedList, bool &validNode);
//Function used to move through the chain of parents and create the route from start to end for the map.
void GenerateRoute(deque <unique_ptr < SCoords > > &openList, vector<int> &routeX, vector<int> &routeY);
//Function used to read in files and store information in appropriate variables.
void ReadFiles(ifstream& infile, string SCoordsName, SCoords& start, SCoords& end, string mapName, int mapArray[10][10]);
//Function used to set up open list and push start point onto the list.
void InitialiseList(SCoords mapStart, SCoords mapEnd, deque <unique_ptr < SCoords > > &openList, deque <unique_ptr < SCoords > > &closedList, int &newCost, int &existingCost);
//Function used to tie together whole program and allow us to use a single function to invoke an A Star search.
void PathFind(bool &goalReached, deque <unique_ptr < SCoords > > &openList, deque <unique_ptr < SCoords > > &closedList, SCoords mapEnd, SCoords &north, SCoords &east, SCoords &south, SCoords &west
	, int mapArray[10][10], IModel* mapSquares[10][10], IMesh* squareMesh, int newCost, int existingCost, string visitedNodeSkin, string openListNodeSkin, bool &validNode);
//Function used to 
void setGrid(IMesh* squareMesh, IModel* mapSqaures[10][10], string water, string wood, string open, string wall,
	string start, string end, int mapArray[10][10], SCoords mapStart, SCoords mapEnd);
//Function used to reset the camera to its starting position and rotate it correctly.
void cameraReset(ICamera* &camera);
//Function used to reset all variables involved in displaying real time output of the A Star search.
void resetValues(int &pathCounter, float &timer, bool &goalReached, bool &routeGenerated, bool &displayRoute, bool &validNode, bool &startSearch, vector<int> &routeX, vector<int> &routeY);

