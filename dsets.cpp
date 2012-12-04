#include "dsets.h"

/**
 * The default constructor.
 */
DisjointSets::DisjointSets()
{

}

/**
 * This function creates n unconnected root nodes 
 * at the end of the vector.
 */
void DisjointSets::addelements(int num)
{
	for(int i=0; i<num; i++)
		elems.push_back(-1);
}

/**
 * This function should compress paths, and works as described
 * in lecture.
 */
int DisjointSets::find(int a)
{
	if(elems[a]<0) return a;
	return elems[a] = find(elems[a]);
}

/**
 * This function should be implemented as a union by size.
 */
void DisjointSets::setunion(int a, int b)
{
	while(elems[a] >=0)
		a = elems[a];
	
	while(elems[b] >=0)
		b = elems[b];
	
	if(a==b) return;

	int newSize = elems[a] + elems[b];
	
	if(elems[a] <= elems[b]) {
		elems[b] = a;
		elems[a] = newSize;
	}
	else {
		elems[a] = b;
		elems[b] = newSize;
	}
}
