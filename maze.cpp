#include "maze.h"
#include "dsets.cpp"
#include <maps>

SquareMaze::SquareMaze()
{
	/**
 	 * The default constructor does nothing
 	 * out of the ordinary.
 	 */
}

void SquareMaze::makeMaze(int width, int height);
{	
	/**
 	 * Erases the grid.
 	 */
	grid.resize(0);
  	walls.resize(0);
	for (int i = 0; i < 0; ++i)
    {
		grid[i].resize(0);
	}
	/**
 	 * Resizes it to the specified
 	 * dimensions.
 	 */
	grid.resize(height);
  	walls.resize(height);

	for (int i = 0; i < height; ++i)
    {
	 	grid[i].resize(width);
		walls[i].resize(width);
	}
	
	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++)
			walls[i][j] = false;
}	

bool SquareMaze::canTravel(int x, int y, int dir) const
{
	switch(dir)
	{
		case 0: return (x+1 < width) && !walls[x+1][y];
		case 1: return (y+1 < height) && !walls[x][y+1];
		case 2: return (x-1 > -1) && !walls[x-1][y];
		case 3: return (y-1 > -1) && !walls[x][y-1];
	}
}

void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
	switch(dir)
	{
		case 0: walls[x+1][y] = true;
				break;
		case 1: walls[x][y+1] = true;
				break;
		default: break;
	}
}

vector<int> SquareMaze::solveMaze()
{

}

PNG * SquareMaze::drawMaze() const
{
	PNG * thing = new PNG(10*width + 1, 10*height + 1);
	
	for(int i=0; i < width; i++)
		for(int j=0; j<10; j++)
		{
			if(i>=1 && i<=9)
				continue;
		
			(*thing)(i,j)->red = (*thing)(i,j)->blue = (*thing)(i,j)->green = 255;
		}
	
	for(int i=0; i<height; i++)
		for(int j=0; j<10; j++)
			(*thing)(j,i)->red = (*thing)(j,i)->blue = (*thing)(j,i)->green = 255;

	return thing;
}

PNG * SquareMaze::drawMazeWithSolution()
{
	
}
