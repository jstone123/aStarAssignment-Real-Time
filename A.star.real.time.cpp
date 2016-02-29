// A star real time.cpp: A program using the TL-Engine
//Josh Stone JStone3
#include "PathFind.h"
void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder(".//aStarMedia");
	/**** Set up your scene here ****/
	
	//Camera.
	ICamera* camera = myEngine->CreateCamera(kFPS, 50, 100, -65);
	camera->RotateX(40);

	ifstream infile;
	ofstream outfile;
	//File names with map information.
	string dCoordsFile = "dCoords.txt";
	string dMapFile = "dMap.txt";
	string mCoordsFile = "mCoords.txt";
	string mMapFile = "mMap.txt";
	string outputFile = "Output.txt";
	//Sqaure colours.
	string water = "blue.png";
	string wood = "green.png";
	string open = "grey.png";
	string wall = "black.png";
	string start = "purple.png";
	string end = "red.png";
	string path = "yellow.png";
	string currentNodeSkin = "pink.png";
	string openListNodeSkin = "orange.png";
	string visitedNodeSkin = "white.png";
	//Variabls to hold start and end coordinates for each map.
	SCoords mMapStart;//Variable used to store start position for mMap. 
	SCoords mMapEnd;//Variable used to store end position for mMap. 
	SCoords dMapStart;//Variable used to store start position for dMap. 
	SCoords dMapEnd;//Variable used to store end position for dMap. 
	SCoords north;//Variable used to store information about north node.
	SCoords east;//Variable used to store information about east node.
	SCoords south;//Variable used to store information about south node.
	SCoords west;//Variable used to store information about west node.
	vector<int> routeX;//Vector used to store the x coordinates of the route.
	vector<int>  routeY;//Vector used to store the y coordinates of the route.
	int mMapArray[10][10];//Array used to store the costs of each square for mMap.
	int dMapArray[10][10];//Array used to store the costs of each square for dMap.
	int mapNumber = 1;//Variable used to indicate what map is being searched. 
	int newCost;//Variable used to store the cost of moving to the new node.
	int existingCost;//Variable used to store the cost of moving to the new node if it has already been visited.
	int pathCounter = 0;//Variable used to display the final path by iterating through routeX and routeY vectors. Also used to control some text displays.
	bool goalReached = false;//Variable used to ensure that search doesn't end until the goal is reached.
	bool startSearch = false;//Variable used to indicate whether the user has started the search.
	bool validNode;//Variable used to indicate if a node is valid, if node is a wall the its invalid, this is used to control colour change of nodes.
	bool routeGenerated = false;
	deque <unique_ptr < SCoords > > openList;//Open list is a list sorted by score and the new nodes are generated using this list.
	deque <unique_ptr < SCoords > > closedList;//Closed list is an unordered list that stores nodes that have already been visited.
	unique_ptr <SCoords> currentNode;//Current node is used to store the information when each node is popped off the open list.
	EMaps map = mMap;//Enum used to indicate which map is being searched.
	ReadFiles(infile, mCoordsFile, mMapStart, mMapEnd, mMapFile, mMapArray);//Read in the files for the mMap.
	ReadFiles(infile, dCoordsFile, dMapStart, dMapEnd, dMapFile, dMapArray);//Read in the files for the mMap.
	InitialiseList(mMapStart, mMapEnd, openList, closedList, newCost, existingCost);//We then set up open list and push the start point onto it.
	IMesh* squareMesh = myEngine->LoadMesh("cube.x");
	IModel* mapSquares[10][10];
	//This loop is used to create the 100 squares used to represent the maps. 
	//The x and z coordinates are multiplied by 10 so that the squares are spread out and visually more pleasing. 
	for (int i = 0; i < 10; i++)//rows 
	{
		for (int j = 0; j < 10; j++)//collums 
		{
			mapSquares[i][j] = squareMesh->CreateModel(j * 10, 0, i * 10);
		}
	}
	setGrid(squareMesh, mapSquares, water, wood, open, wall, start, end, mMapArray, mMapStart, mMapEnd);
	IFont* gameFont = myEngine->LoadFont("Arial", 36);
	stringstream mMapText;
	mMapText << "mMap = 1 key";
	stringstream dMapText;
	dMapText << "dMap = 2 key";
	stringstream startSearchText;
	startSearchText << "Start search = Enter key";
	stringstream displayFinalRouteText;
	displayFinalRouteText << "Display route = Space key";
	stringstream routeFoundText;
	routeFoundText << "Route Found," << endl << "Press Space to display route.";
	stringstream routeCompleteText;
	routeCompleteText << "Route complete!";
	stringstream resetCamera;
	resetCamera << "Reset Camera = C key";

	stringstream waterText;
	waterText << "Water = blue";
	stringstream woodText;
	woodText << "Wood = green";
	stringstream openText;
	openText << "Clear = grey";
	stringstream wallText;
	wallText << "Wall = black";
	stringstream startText;
	startText << "Start = purple";
	stringstream endText;
	endText << "End = red";
	stringstream pathText;
	pathText << "Path = yellow";
	stringstream visitedNodeText;
	visitedNodeText << "Visited node = white";
	stringstream potentialNodeText;
	potentialNodeText << "Potential node = orange";
	IFont* infoFont = myEngine->LoadFont("Arial", 26);
	
	bool routeFinished;

	bool quiteGameKeyPressed = false;
	bool startSearchKeyPressed = false;
	bool displayFinalRouteKeyPressed = false;
	bool route1KeyPressed = false;
	bool route2KeyPressed = false;
	bool resetCameraKeyPressed = false;
	bool displayRoute = false;
	float timer = 0;
	float searchTime = 0.5;
	float displayTime = 0.5;
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{		
		float frameTime = myEngine->Timer();
		//startSearchKeyPressed = (myEngine->KeyHit(Key_Return));
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		gameFont->Draw(mMapText.str(), 0, 20, kMagenta, kLeft, kVCentre);
		gameFont->Draw(dMapText.str(), 0, 50, kMagenta, kLeft, kVCentre);
		gameFont->Draw(startSearchText.str(), 0, 80, kMagenta, kLeft, kVCentre);
		gameFont->Draw(displayFinalRouteText.str(), 0, 110, kMagenta, kLeft, kVCentre);

		infoFont->Draw(startText.str(), 1000, 10, kMagenta, kLeft, kVCentre);
		infoFont->Draw(endText.str(), 1000, 30, kRed, kLeft, kVCentre);
		infoFont->Draw(openText.str(), 1000, 50, kGrey, kLeft, kVCentre);
		infoFont->Draw(wallText.str(), 1000, 70, kBlack, kLeft, kVCentre);
		infoFont->Draw(woodText.str(), 1000, 90, kGreen, kLeft, kVCentre);
		infoFont->Draw(waterText.str(), 1000, 110, kBlue, kLeft, kVCentre);
		infoFont->Draw(pathText.str(), 1000, 130, kYellow, kLeft, kVCentre);	  
		infoFont->Draw(visitedNodeText.str(), 1000, 150, kWhite, kLeft, kVCentre);
		infoFont->Draw(potentialNodeText.str(), 1000, 170, kWhite, kLeft, kVCentre);

		quiteGameKeyPressed = (myEngine->KeyHit(Key_Escape));
		startSearchKeyPressed = (myEngine->KeyHit(Key_Return));
		displayFinalRouteKeyPressed = (myEngine->KeyHit(Key_Space));
		route1KeyPressed = (myEngine->KeyHit(Key_1));
		route2KeyPressed = (myEngine->KeyHit(Key_2));
		resetCameraKeyPressed = (myEngine->KeyHit(Key_C));

		if (resetCameraKeyPressed)
		{
			cameraReset(camera);
		}

		if (route1KeyPressed)
		{
			mapNumber = 1;
			setGrid(squareMesh, mapSquares, water, wood, open, wall, start, end, mMapArray, mMapStart, mMapEnd);
			resetValues(pathCounter, timer, goalReached, routeGenerated, displayRoute, validNode, startSearch, routeX, routeY);		
			InitialiseList(mMapStart, mMapEnd, openList, closedList, newCost, existingCost);//We then set up open list and push the start point onto it.
		}
		if (route2KeyPressed)
		{
			mapNumber = 2;
			setGrid(squareMesh, mapSquares, water, wood, open, wall, start, end, dMapArray, dMapStart, dMapEnd);
			resetValues(pathCounter, timer, goalReached, routeGenerated, displayRoute, validNode, startSearch, routeX, routeY);
			InitialiseList(dMapStart, dMapEnd, openList, closedList, newCost, existingCost);//We then set up open list and push the start point onto it.
		}

		if (quiteGameKeyPressed)
		{
			myEngine->Stop();
		}
		if (startSearchKeyPressed)
		{
			startSearch = !startSearch;
		}
		if (startSearch)
		{				
			timer += frameTime;
			if (timer > searchTime)
			{
				if (mapNumber == 1)
				{
					PathFind(goalReached, openList, closedList, mMapEnd, north, east, south, west, mMapArray, mapSquares, squareMesh, newCost, existingCost, visitedNodeSkin, openListNodeSkin, validNode);
					timer = 0;
				}
				if (mapNumber == 2)
				{
					PathFind(goalReached, openList, closedList, dMapEnd, north, east, south, west, dMapArray, mapSquares, squareMesh, newCost, existingCost, visitedNodeSkin, openListNodeSkin, validNode);
					timer = 0;
				}
			}
		}

		if (goalReached && !routeGenerated)
		{
			GenerateRoute(openList, routeX, routeY);
			routeGenerated = true;
		}
		if (routeGenerated && !displayRoute)
		{
			gameFont->Draw(routeFoundText.str(), 500, 30, kRed, kLeft, kVCentre);
		}
		if (displayFinalRouteKeyPressed && routeGenerated)
		{
			displayRoute = true;
		}
		if (displayRoute)
		{		
			timer += frameTime;
			if (timer > displayTime)
			{
				if (pathCounter < routeX.size())
				{
					mapSquares[routeY[pathCounter]][routeX[pathCounter]]->SetSkin(path);
					pathCounter++;
					timer = 0;
				}
			}
			if (pathCounter == routeX.size() && pathCounter > 0)
			{
				gameFont->Draw(routeCompleteText.str(), 500, 80, kRed, kLeft, kVCentre);
			}
		}

	}
	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
