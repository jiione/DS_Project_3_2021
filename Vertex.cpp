#include "Vertex.h"
Vertex::Vertex(int key)
{
    m_key=key;
    m_size=0;
    m_pEHead=nullptr;
    Vertex* m_pNext=nullptr;
    company="";
}
void Vertex::SetCompany(string str){this->company=str;}
void Vertex::SetNext(Vertex* pNext) {this->m_pNext=pNext;}

int Vertex::GetKey() const {return m_key;}

Vertex* Vertex::GetNext() const {return m_pNext;}

int Vertex::Size() const {return m_size;}

Edge* Vertex::GetHeadOfEdge() const {return m_pEHead;}

string Vertex::GetCompany() const {return company;}

void Vertex::AddEdge(int edgeKey, int weight)
{
    if(m_pEHead==nullptr)
    {
        m_pEHead=new Edge(edgeKey,weight);
        m_size++;
        return;
    }

    Edge* currEdge = m_pEHead;
    while(currEdge->GetNext()) currEdge=currEdge->GetNext();
    currEdge->SetNext(new Edge(edgeKey,weight));
}

