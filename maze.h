#ifndef EMAZE_H
#define EMAZE_H

#include <vector>
#include "png.h"
#include "dsets.h"

using namespace std;

class SquareMaze
{
	public:
		SquareMaze();
		void makeMaze(int width, int height);
		bool canTravel(int x, int y, int dir) const;
		void setWall(int x, int y, int dir, bool exists);
		std::vector<int> solveMaze();
		PNG * drawMaze() const;
		PNG * drawMazeWithSolution();

	private:
		int pathfinder(int x, int y, int destX, int destY, vector<bool> & beenhere);
		void makeVector(int x, int y, int destX, int destY, vector<int> & solution, vector<bool> & beenhere);
		int randgen();	
		DisjointSets forest;	
		int width;
		int height;
		vector<bool> dwalls;
		vector<bool> rwalls;

};

#endif
