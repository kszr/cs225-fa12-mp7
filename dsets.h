#include <vector>
class DisjointSets
{
	public:
		DisjointSets();
		void addelements(int num);
		int find(int elem);
		void setunion(int a, int b);
	
	private:
		std::vector<int> elems;
};
