#include "maze.h"
#include <map>

SquareMaze::SquareMaze()
{
	/**
 	 * The default constructor does nothing
 	 * out of the ordinary.
 	 */
}

void SquareMaze::makeMaze(int w, int h)
{	
	forest.clear();

	forest.addelements(w*h);

	height = h;
	width = w;

	rwalls.resize(h*w);
	dwalls.resize(h*w);

	for(int i=0; i<h*w; i++)
	{
		rwalls.push_back(false);
		dwalls.push_back(false);	
	}	
}

/**
 * NOTES:
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
	
	for(int i=0; i < thing->width(); i++)
	{
		if(i>0 && i<10) continue;
				
		(*thing)(i,0)->red = (*thing)(i,0)->blue = (*thing)(i,0)->green = 0;
	
	}
	for(int i=0; i<thing->height(); i++)
		(*thing)(0,i)->red = (*thing)(0,i)->blue = (*thing)(0,i)->green = 0;

	
	
	return thing;
}

PNG * SquareMaze::drawMazeWithSolution()
{
	PNG * thing = new PNG();
	return thing;
}
