#include "maze.h"
#include <map>
#include <stdlib.h>
#include <time.h>
#include <climits>
#include <stack>
#include <queue>
#include <map>

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
 *
 * (It is hoped that this function works as it should.)
 */
bool SquareMaze::canTravel(int x, int y, int dir) const
{
	int cell = x + y*width;
	switch(dir)
	{
		case 0: return (x+1 < width) && !rwalls[cell];
		case 1: return (y+1 < height) && !dwalls[cell];
		case 2: return (x-1 > -1) && !rwalls[cell-1]; //The cell to the left
		case 3: return (y-1 > -1) && !dwalls[cell-width]; //The cell above

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

#include <iostream>
using namespace std;
vector<int> SquareMaze::solveMaze()
{
	/**
 	 * Uses breadth-first search.
 	 */
	queue<int> structure;
	structure.push(0);
	
	vector<bool> beenhere;
	//Initialize all the cells as unvisited.
	for(int i=0; i<width*height; i++)
		beenhere.push_back(false);

	int exitX = -1; //The x-coordinate of the exit

	/**
	 * Each key represents a cell, and has as its value
	 * the cell whence it was reached.
	 */
 	map<int, int> prev; //A breadcrumb trail, if we are lucky.

	while(!structure.empty())
	{
		int curr = structure.front(); //Remove a cell from the ordering structure.
		structure.pop();					
		beenhere[curr] = true; //Mark this cell as having been visited.
		distance++; //Update the distance it has moved.

		int x = curr%width;
		int y = curr/width;
		
		int right = x+1 + y*width;
		int down = x + (y+1)*width;
		int left = x-1 + y*width;
		int up = x + (y-1)*width;

		/**
		 * Add any neighboring cells to which it is possible to
		 * travel in a single step, and which have not been
		 * visited yet. Also make entries for them in the 
		 * map, in order that it be possible to trace paths through
		 * the maze later.
		 */
		if(canTravel(x, y, 0) && !beenhere[right])
	    {
			structure.push(right);
			prev[right] = curr;
		}
		if(canTravel(x, y, 1) && !beenhere[down])
		{
			structure.push(down);
			prev[down] = curr;
		}
		if(canTravel(x, y, 2) && !beenhere[left])
		{
			structure.push(left);
			prev[left] = curr;
		}
		if(canTravel(x, y, 3) && !beenhere[up])
		{
			structure.push(up);
			prev[up] = curr;
		}

	}	

	/**
	 * These will eventually have the values of the x-coordinate
	 * of the exit and the distance of the exit from the start.
	 */
	int exitX = 0, distance = 0;

	/**
	 * By the end of this for loop, we shall have the position
	 * of the exit.
	 */
	for(int i=0; i<width; i++)
	{
		/**
		 * We are only interested in the cells in the bottom row
		 * of the grid. More specifically, we wish to find the 
		 * cell from which the distance to the start is the longest
		 * We also assume that the breadth-first search succeeded
		 * in adding every cell of the grid to the map. 
		 */
		int cell = i + (height-1)*width;

		//The distance of the given path from the start.
		int localDist = 0;

		/**
		 * Calculate the distance of the current cell from the 
		 * start.
		 */
		while(cell != 0)
		{
			cell = prev[cell]; //This is the previous cell.
			localDist++;
		}

		/**
		 * Compare this distance with the best so far. We wish to
		 * find the largest distance, so perform the necessary
		 * operations to this end.
		 */
		if(distance < localDist)
		{
			distance = localDist;
			exitX = cell%width;
		}
		/**
		 * This addresses a special though arbitrary tie-breaking
		 * case adumbrated in the documentation.
		 */
		if(distance == localDist)
		{
			exitX = exitX < cell%width? exitX : cell%width;
		}
	}

	/**
	 * Now it remains for us to generate the solution vector.
	 * This will be done first by pushing the necessary values
	 * onto a stack, and then copying them over to the solution
	 * vector, and will save us the need to reverse the vector
	 * once it has been generated.
	 */
	stack<int> sol;
	int derp = exitX + (height-1)*width;
	while(derp != 0)
	{
		currX = derp%width;
		currY = derp/width;
		
		herp = prev[derp];
		prevX = herp%width;
		prevY = herp/width;

		if(prevX+1 == currX)
			sol.push(0);
		else if(prevY+1 == currY)
			sol.push(1);
		else if(prevX-1 == currX)
			sol.push(2);
		else sol.push(3);

		derp = herp;
	}

	/**
	 * Now we copy the elements of the stack to the solution vector
	 */
	vector<int> solution;
	while(!sol.empty())
	{
		solution.push_back(sol.top());
		sol.pop();
	}

	return solution;
}	

/**
 * This works as it should.
 */
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

/**
 * The last time I checked, this worked fine.
 */
PNG * SquareMaze::drawMazeWithSolution()
{
	PNG * thing = drawMaze();
	vector<int> solution = solveMaze();
	
	int x=5, y=5;
	for(int i=0; i<solution.size(); i++)
	{
		for(int j=0; j<11; j++)
		{
			RGBAPixel * pixel;
			switch(solution[i])
			{
				case 0: pixel = (*thing)(x+j, y);
						pixel->red = 255;
						pixel->blue = pixel->green = 0;
						break;
				case 1:	pixel = (*thing)(x, y+j);
						pixel->red = 255;
						pixel->blue = pixel->green = 0;
						break;
				case 2:	pixel = (*thing)(x-j, y);
						pixel->red = 255;
						pixel->blue = pixel->green = 0;
						break;
				case 3:	pixel = (*thing)(x, y-j);
						pixel-> red = 255;
						pixel->blue = pixel->green = 0;
				default: break;
			}
		}
		switch(solution[i])
		{
			case 0: x += 10;
					break;
			case 1: y += 10;
					break;
			case 2:	x -= 10;
					break;
			case 3:	y -= 10;
			default: break;
		}
	}	
	return thing;
}
