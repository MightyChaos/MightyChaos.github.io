#include "DepthSegTree.h"
#include <mex.h>

DepthSegTree::DepthSegTree(int x, int y, double** depthmap, int h, int w)
{
	m_size = 0;
	m_root = new node(x,y);
	m_bsearched = cyCreateMat2<bool>(h,w,false);
	m_h = h;
	m_w = w;
	m_depthmap = depthmap;
	
}

void DepthSegTree::SearchNode(node* a)
{
	for(int i=-1; i<=1; i++)
	{
		if(a->x+i>=0 && a->x+i<m_w && i!=0)
			if(!m_bsearched[a->y][a->x+i])
				if(abs(m_depthmap[a->y][a->x+i] - m_depthmap[a->y][a->x])<=THRESHOLD)
				{
//					mexPrintf("node: i: %d j: %d\n",a->y,a->x+i);
					node* new_node = new node(a->x+i,a->y);
//					a->sons.push_back(new_node);
					m_bsearched[a->y][a->x+i] = true;
					m_queue.push_back(new_node);
				}
		if(a->y+i>=0 && a->y+i<m_h && i!=0)
			if(!m_bsearched[a->y+i][a->x])
				if(abs(m_depthmap[a->y+i][a->x] - m_depthmap[a->y][a->x])<=THRESHOLD)
				{
//					mexPrintf("node: i: %d j: %d\n",a->y+i,a->x);
					node* new_node = new node(a->x,a->y+i);
//					a->sons.push_back(new_node);
					m_bsearched[a->y+i][a->x] = true;
					m_queue.push_back(new_node);
				}
	}
}

void DepthSegTree::Build()
{
	m_queue.push_back(m_root);
	while(m_size<m_queue.size())
	{
		node* cur = m_queue[m_size];
		m_size++;
		SearchNode(cur);
	}
}

int DepthSegTree::GetCoordinates(int**&coordinates)
{
	coordinates = cyCreateMat<int>(m_size,2);
//	mexPrintf("m_size = %d queue_size = %d\n", m_size, m_queue.size());
	
	for(int i=0; i<m_size; i++)
	{
		coordinates[i][0] = m_queue[i]->y+1;
		coordinates[i][1] = m_queue[i]->x+1;
//		mexPrintf("%d coor: %d %d\n", i, m_queue[i]->y, m_queue[i]->x);
	}
	
	return m_size;
}

bool** DepthSegTree::GetMask()
{
	return m_bsearched;
}

DepthSegTree::~DepthSegTree(void)
{
	for(int i=0; i<m_queue.size(); i++)
		delete m_queue[i];
	cyDeleteMat<bool>(m_bsearched);
} 
