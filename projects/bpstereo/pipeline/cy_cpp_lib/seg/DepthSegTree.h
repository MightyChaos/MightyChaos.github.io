#include <vector>
#include "cy_basic.h"
using namespace std;

class DepthSegTree
{
struct node
{
	int x, y;
//	vector<node*> sons;
	node(int x,int y)
	{
		this->x = x;
		this->y = y;
	}
};

public:
	DepthSegTree(int x, int y, double** depthmap, int h, int w);
	~DepthSegTree(void);
	void Build();
	int GetCoordinates(int**&coordinates);
	bool** GetMask();
private:
	void SearchNode(node* a);
//attributes:
public:
	int m_size;
	int m_h, m_w;
	static const int THRESHOLD = 1; 
private:
	node* m_root;
	vector<node*> m_queue;
	bool** m_bsearched;
	double** m_depthmap;
};

