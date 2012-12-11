#include "maze.h"
#include <map>
#include <stdlib.h>
#include <time.h>
#include <climits>
#include <queue>
/**
 * The default constructor.
 */
SquareMaze::SquareMaze()
{
	/**
 	 * Nothing here.
 	 */
}

/**
 * NOTES:
 * rwalls - right walls, indexed by cell.
 * dwalls - bottom walls, indexed by cell.
 */
void SquareMaze::makeMaze(int w, int h)
{	
	//Clears the dset.
	forest.clear();

	//Now each cell is a tree unconnected
	//to any other cell.
	forest.addelements(w*h);

	height = h;
	width = w;

	rwalls.resize(0);
	dwalls.resize(0);

	for(int i=0; i<w*h; i++)
	{
		/**
 		 * Walls are built everywhere,
		 * since the dset above was initialized
		 * to contain no connected cells.
		 */
		rwalls.push_back(true);
		dwalls.push_back(true);	
	}

	/**
 	 * A list of walls:
 	 * Even numbered elements -- rwalls
 	 * Odd numbered elements -- dwalls
 	 * The value of the indices must not be assumed
 	 * to bear any relation to the elements,
 	 * except immediately after initialization.
 	 */
    vector<int> walls;
	for(int i=0; i<2*w*h; i++)
		walls.push_back(i);

	srand(time(NULL));	
	while(!forest.isConnected())
	{	
	   /**
     	* 0 - break the right wall
     	* 1 - break the bottom wall
     	* 2 - break the left wall
     	* 3 - break the top wall
 		*/
		int random = randgen() % walls.size(); //Random index;
		int atRandom = walls[random]; //random wall
		int cell = atRandom/2; //the cell bounded by the wall
		int x = cell%width; //its x coordinate
		int y = cell/width; //its y coordinate
		
		int thing = atRandom%2;	
		switch(thing) //Is the wall an rwall or a dwall?
		{
			/**
			 * 0 => right wall
 			 * 1 => bottom wall
			 */
			case 0: if(x == width-1) break; //if the cell is on the right perimeter.
					if(forest.find(cell) == forest.find(cell+1)) break; //if the cell is already in the tree.
					forest.setunion(cell, cell+1);
					rwalls[cell] = false;
					walls.erase(walls.begin() + random);
					break;
			case 1: if(y == height-1) break; //if the cell is on the bottom perimeter.
					if(forest.find(cell) == forest.find(cell+width)) break; //if the cell is already in the tree.
					forest.setunion(cell, cell+width);
					dwalls[cell] = false;
					walls.erase(walls.begin() + random);
			default: break;
		}
	} 
}

/**
 * Helps to generate a pseudo-random number.
 */
int SquareMaze::randgen()
{
	return rand()%RAND_MAX;
}

/** 
 * Each cell is given a unique number. Cells are numbered
 * from left to right in each row.
 */
bool SquareMaze::canTravel(int x, int y, int dir) const
{
	int cell = y*width + x;
	switch(dir)
	{
		case 0: return (x+1 < width) && !rwalls[cell];
		case 1: return (y+1 < height) && !dwalls[cell];
		case 2: return (x-1 > -1) && !rwalls[cell-1];
		case 3: return (y-1 > -1) && !dwalls[cell-width];

		default: return false;
	}
}

/**
 * NOTES:
 * Read the notes above the makeMaze function for information on
 * the schemata used to represent walls.
 */
void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
	int cell = y*width + x;
	switch(dir)
	{
		case 0: rwalls[cell] = exists;
				break;
		case 1: dwalls[cell] = exists;
		default: break;
	}
}

/**
 * Returns the length of the path from the start of the maze to the specified
 * coordinate at the bottom of the maze.
 */
int SquareMaze::pathfinder(int x, int y, int destX, int destY, vector<bool> & beenhere)
{
	if(y == destY && x == destX)
		return 1;
	
	int cell = x + y*width;
	
	beenhere[cell] = true;

	if(x < destX && canTravel(x, y, 0) && !beenhere[x+1 + y*width])
			return 1 + pathfinder(x+1, y, destX, destY, beenhere);
	if(canTravel(x, y, 3) && !beenhere[x + (y-1)*width])
			return 1 + pathfinder(x, y-1, destX, destY, beenhere);

	if(y < destY && canTravel(x, y, 1) && !beenhere[x + (y+1)*width])
			return 1 + pathfinder(x, y+1, destX, destY, beenhere);

	if(canTravel(x, y, 2) && !beenhere[x-1 + y*width])
		return 1 + pathfinder(x-1, y, destX, destY, beenhere);

	return 0;

}

using namespace std;
vector<int> SquareMaze::solveMaze()
{
	/**
 	 * Finds the cell farthest away from the entrance.
 	 */
	queue<int> structure;
	queue<int> sol;
	structure.push(0);
	
	vector<int> beenhere;
	for(int i=0; i<width*height; i++)
		beenhere.push_back(false);

	int exitX;
	int flag = false;
	vector<int> solution;
	
	//Performs a breadth-first search. Exits as soon
	//as it hits the bottom of the grid.
	while(!structure.empty())
	{
		int curr = structure.back();
		structure.pop();
		if(flag)
		{
			int dir = sol.back();
			sol.pop();
			solution.push_back(dir);
		}
		!flag;
		beenhere[curr] = true;
		if(curr/width == height-1)
		{
			exitX = curr%width;
			break;
		}
		
		int x = curr%width;
		int y = curr/width;
		
		int right = x+1 + y*width;
		int down = x + (y+1)*width;
		int left = x-1 + y*width;
		int up = x + (y-1)*width;

		if(!beenhere[right] && canTravel(x, y, 0))
		{
			structure.push(right);
			sol.push(0);
		}
		if(!beenhere[down] && canTravel(x, y, 1))
		{
			structure.push(down);
			sol.push(1);
		}
		if(!beenhere[left] && canTravel(x, y, 2))
		{
			structure.push(left);
			sol.push(2);
		}
		if(!beenhere[up] && canTravel(x, y, 3))
		{
			structure.push(up);
			sol.push(3);
		}
	}
	
	return solution;
}	

/**
 * Creates the solution vector.
 */
void SquareMaze::makeVector(int x, int y, int destX, int destY, vector<int> & solution, vector<bool> & beenhere)
{
	if(y == destY && x == destX)
		return;

	int cell = x + y*width;
	beenhere[cell] = true;

    if(x < destX && canTravel(x, y, 0) && !beenhere[x+1 + y*width])
    {
		solution.push_back(0);
		makeVector(x+1, y, destX, destY, solution, beenhere);
		return;
	}

	if(canTravel(x, y, 3) && !beenhere[x + (y-1)*width])
    {
        solution.push_back(3);
        makeVector(x, y-1, destX, destY, solution, beenhere);
		return;
    }
 
    if(y < destY && canTravel(x, y, 1) && !beenhere[x + (y+1)*width])
    {
		solution.push_back(1);
		makeVector(x, y+1, destX, destY, solution, beenhere);
		return;
	}

    if(canTravel(x, y, 2) && !beenhere[x-1 + y*width])
	{
		solution.push_back(2);
		makeVector(x-1, y, destX, destY, solution, beenhere);
        return;
	}

}


PNG * SquareMaze::drawMaze() const
{
	PNG * thing = new PNG(10*width + 1, 10*height + 1);

	/**
 	 * Draws the top border of the grid, broken at the left corner
 	 * where the entrance is to be.
 	 */	
	for(int i=0; i < thing->width(); i++)
	{	
		//Keep the top-left corner unbounded.
		if(i>0 && i<10) continue;
			
		(*thing)(i,0)->red = (*thing)(i,0)->blue = (*thing)(i,0)->green = 0;	
	}

	/**
 	 * Draws the left border of the grid, unbroken.
 	 */
	for(int i=0; i<thing->height(); i++)
		(*thing)(0,i)->red = (*thing)(0,i)->blue = (*thing)(0,i)->green = 0;

	/**
 	 * Draws the walls of the maze.
 	 */
	for(int x=0; x<width; x++)
		for(int y=0; y<height; y++)
		{
			int cell = x + y*width;
			/**
 		     * Draws a right wall, where present.
 		     */
			if(rwalls[cell])
				for(int k=0; k<10; k++)
				{
					RGBAPixel * pixel = (*thing)((x+1)*10, y*10+k);
					pixel->red = pixel->blue = pixel->green = 0;
				}
			/**
 			 * Draws a bottom wall, where present.
 			 */
			if(dwalls[cell])
				for(int k=0; k<10; k++)
				{
					RGBAPixel * pixel = (*thing)(x*10+k, (y+1)*10);
					pixel->red = pixel->blue = pixel->green = 0;
				}
		}	
	
	return thing;
}

PNG * SquareMaze::drawMazeWithSolution()
{
	PNG * thing = drawMaze();
	vector<int> solution = solveMaze();
	return thing;
}
