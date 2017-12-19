#include <iostream>
#include <string>
#include <unordered_set>

#define COLUMN_COUNT 0
#define STARTING_SPOT 1
#define ENDING_SPOT 2

// This is not an a* implementation or any kind of AI pathfinding, 
// it is only a maze runner who goes through a string maze
// Will implement node cost and heuristic later

using namespace std;

// Direction was set this way to fasten a bit the process of pathfinding,
// By having to go South and East first, it reduces the process time of having to do the recursion
// since my goal is at the bottom right of the maze or and the end of the string
enum Direction { South, East, West, North };
bool FindPath(string& maze, int mazeSize, int currentPos, int currentRow, vector<vector<int>>* columnCount, Direction direction, unordered_set<int>*  unvalidSpot);

bool path_finder(string maze) {
	int columnCount = 0;
	int startingSpot = 0;
	int endingSpot = 0;
	bool columnHasBeenCount = false;

	// Use of an unordered_set the find instantly if the currentPosition as an int has already been visited
	unordered_set<int> checkedSpots = std::unordered_set<int>();

	// Use of a linked vector to create a cache about each row of the maze
	vector<vector<int>> rowLength;

	// Goes through the maze once and build the row's cache has the iteration goes through it
	for (int i = 0; i < maze.length(); ++i)
	{
		if (maze[i] == '\n')
		{
			columnCount++;
			endingSpot += columnCount;
			std::vector<int> rowInfos = { columnCount, startingSpot, endingSpot };
			rowLength.push_back(rowInfos);
			startingSpot += columnCount;
			columnCount = 0;
		}
		else if (i == maze.length() - 1)
		{
			columnCount++;
			endingSpot += columnCount;
			// Add 1 to the final row count to avoid missing one column since there is no \n to the line
			std::vector<int> rowInfos = { columnCount + 1, startingSpot, endingSpot };
			rowLength.push_back(rowInfos);
			startingSpot += columnCount;
			columnCount = 0;
		}
		else
			columnCount++;
	}

	int size = rowLength.size();

	// First iteration on the upper left spot in the maze or the first index
	for (int dir = South; dir <= North; dir++)
	{
		Direction currDir = static_cast<Direction>(dir);

		// Simply checks if the ending can be reach at all, to save process time if it is not the case
		if (maze[maze.length() - 1] != 'W')
		{
			if (FindPath(maze, size, 0, 0, &rowLength, currDir, &checkedSpots))
			{
				return true;
			}
		}
	}
	return false;
}


// Is used recursively to go all over the maze by checking if adjacent square is a valid spot to go to
bool FindPath(string& maze, int mazeSize, int currentPos, int currentRow, vector<vector<int>>* rowLenght, Direction direction, unordered_set<int>* unvalidSpot)
{
	unvalidSpot->insert(currentPos);
	std::unordered_set<int>::const_iterator iter;

	// Check if the current position is already at the end of the maze
	if (currentPos == (maze.length() - 1))
		return true;
	else
	{
		switch (direction)
		{
			// Starts by checking if the spot bellow the current position has already been visited
		case Direction::South:
			iter = unvalidSpot->find(currentPos + rowLenght->at(currentRow)[0]);

			if (iter == unvalidSpot->end())
			{
				// Once the spot has been identified as unvisited,
				// First, checks if the spot is a reachable spot i.e. not a Wall
				// Second, since the maze can be everything from a square array [4,4] -> ''''\n
				//																		 ''''\n
				//																		 ''''\n
				//									  0123456789						 ''''\n
				// to a random size array on each row '''\n			1
				//									  ''''''''''\n  2
				//									  '''''\n		3
				//									  '\n			4
				// checks if the bellow row + currentRow size still is inbound of the bellow row
				// Ex: row number1 has a columnCount of 3
				// if I'm currently at row1[1] and I add it's columnCount,
				// I'll be sitting at row2[1] and it's working since row 2 does have at least 2 indexes
				// However, if I'm currently at row2[9] and I add row2 ColumnCount, which is 10,
				// I cannot go on row3[10] because it's out of bound and it's not possible to jump over a row
				// At the end, if the spot is still a valid one, call FindPath for the spot bellow the current one
				// Else end the process and don't try to visit this new spot
				if (currentRow + 1 < mazeSize &&
				   (currentPos + rowLenght->at(currentRow)[COLUMN_COUNT]) >= rowLenght->at(currentRow + 1)[STARTING_SPOT] &&
				   (currentPos + rowLenght->at(currentRow)[COLUMN_COUNT]) < rowLenght->at(currentRow + 1)[ENDING_SPOT] &&
					maze[currentPos + rowLenght->at(currentRow)[COLUMN_COUNT]] != 'W' &&
					maze[currentPos + rowLenght->at(currentRow)[COLUMN_COUNT]] != '\n')
				{
					for (int dir = South; dir <= North; dir++)
					{
						Direction currDir = static_cast<Direction>(dir);
						if (FindPath(maze, mazeSize, currentPos + rowLenght->at(currentRow)[0], currentRow + 1, rowLenght, currDir, unvalidSpot))
							return true;
					}
				}
			}
			return false;
			break;
		case Direction::East:
			iter = unvalidSpot->find(currentPos + 1);

			// Starts by checking if the spot left to the current position has already been visited
			if (iter == unvalidSpot->end())
			{
				// Once the spot has been identified as unvisited,
				// Checks if the spot is not a wall or a row ending
				if ((currentPos + 1) < maze.length() && maze[currentPos + 1] != 'W' && maze[currentPos + 1] != '\n')
				{
					for (int dir = South; dir <= North; dir++)
					{
						Direction currDir = static_cast<Direction>(dir);
						if (FindPath(maze, mazeSize, currentPos + 1, currentRow, rowLenght, currDir, unvalidSpot))
							return true;
					}
				}
			}
			return false;
			break;
		case Direction::West:
			iter = unvalidSpot->find(currentPos - 1);

			// Starts by checking if the spot Left to the current position has already been visited
			if (iter == unvalidSpot->end())
			{
				// Idem as East
				if ((currentPos - 1) >= 0 && maze[currentPos - 1] != 'W' && maze[currentPos - 1] != '\n')
				{
					for (int dir = South; dir <= North; dir++)
					{
						Direction currDir = static_cast<Direction>(dir);
						if (FindPath(maze, mazeSize, currentPos - 1, currentRow, rowLenght, currDir, unvalidSpot))
							return true;
					}
				}
			}
			return false;
			break;
		case Direction::North:
			iter = unvalidSpot->find(currentPos - rowLenght->at(currentRow)[0]);

			// Starts by checking if the spot above to the current position has already been visited
			if (iter == unvalidSpot->end())
			{
				// Idem as South, except this one remove ColumnCount to go 1 step up 
				if (currentRow - 1 >= 0 &&
				   (currentPos - rowLenght->at(currentRow)[COLUMN_COUNT]) >= rowLenght->at(currentRow - 1)[STARTING_SPOT] &&
				   (currentPos - rowLenght->at(currentRow)[COLUMN_COUNT]) < rowLenght->at(currentRow - 1)[ENDING_SPOT] &&
					maze[currentPos - rowLenght->at(currentRow)[COLUMN_COUNT]] != 'W' &&
					maze[currentPos - rowLenght->at(currentRow)[COLUMN_COUNT]] != '\n')
				{
					for (int dir = South; dir <= North; dir++)
					{
						Direction currDir = static_cast<Direction>(dir);
						if (FindPath(maze, mazeSize, currentPos - rowLenght->at(currentRow)[0], currentRow - 1, rowLenght, currDir, unvalidSpot))
							return true;
					}
				}
			}
			return false;
			break;
		}
	}
}