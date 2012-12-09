#include "maze.h"
#include <map>
#include <stdlib.h>
#include <time.h>

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

	/**
 	 * Indicates whether a cell has been processed. The possible values for each index are 2,
 	 * 1, and 0, indicating in order, whether a cell has been processed twice, once, or not
 	 * at all. This is not, perhaps, the best way to process cells, but it serves the purpose
 	 * of preventing the algorithm from attempting to make changes to a cell, when it is known
 	 * for certain that both its walls have been considered for removal.
 	 */
//	vector<int> processed;
//	for(int i=0; i<w*h; i++)
//		processed.push_back(0);

	srand(time(NULL));	
	while(!forest.isConnected() && walls.size()>0)
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
		
//		if(processed[cell]==2) continue;
	
		switch(atRandom%2) //Is the wall an rwall or a dwall?
		{
			/**
			 * 0 => right wall
 			 * 1 => bottom wall
			 */
			case 0: //processed[cell]++;
					if(x == width-1) break; //if the cell is on the right perimeter.
					if(forest.find(cell) == forest.find(cell+1)) break; //if the cell is already in the tree.
					forest.setunion(cell, cell+1);
					setWall(x, y, 0, false);
					walls.erase(walls.begin() + random);
					break;
			case 1: //processed[cell]++;
					if(y == height-1) break; //if the cell is on the bottom perimeter.
					if(forest.find(cell) == forest.find(cell+width)) break; //if the cell is already in the tree.
					forest.setunion(cell, cell+width);
					setWall(x, y, 1, false);
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
		case 1: dwalls[cell] = exists;
		default: break;
	}
}

vector<int> SquareMaze::solveMaze()
{
	return vector<int>();
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
	PNG * thing = new PNG();
	return thing;
}
