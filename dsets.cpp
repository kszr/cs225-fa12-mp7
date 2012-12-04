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
 * This function should compress paths and work as described
 * in lecture.
 */
int DisjointSets::find(int elem)
{
	if(elems[elem]<0) return elem;
	return elems[elem] = find(elems[elem]);
}

/**
 * This function should be implemented a union by size.
 */
void DisjointSets::setunion(int a, int b)
{
	int newSize = elems[a] + elems[b];
	if(a > b) {
		elems[b] = a;
		elems[a] = newSize;
	}
	else {
		elems[a] = b;
		elems[b] = newSize;
	}
}
