#ifndef EDSETS_H
#define EDSETS_H
#include <vector>
class DisjointSets
{
	public:
		DisjointSets();
		void addelements(int num);
		int find(int elem);
		void setunion(int a, int b);
		void clear();
		//int findWithoutPathCompression(int elem);
		bool isConnected();
	private:
		std::vector<int> elems;
};
#endif
