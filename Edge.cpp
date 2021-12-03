#include "Edge.h"

Edge::Edge(int key, int weight)
{
    m_key=key;
    m_weight=weight;
    m_pNext=nullptr;
}

void Edge::SetNext(Edge* pNext){this->m_pNext=pNext;}

int Edge::GetKey() const{return m_key;}

int Edge::GetWeight() const{return m_weight;}

Edge* Edge::GetNext() const{return m_pNext;}