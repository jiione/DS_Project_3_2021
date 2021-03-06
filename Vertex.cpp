#include "Vertex.h"
Vertex::Vertex()
{
   m_key=0;
    m_size=0;
    m_pEHead=nullptr;
    Vertex* m_pNext=nullptr;
    Edge* m_pEHead=nullptr;
    company=""; 
}
Vertex::Vertex(int key)
{
    m_key=key;
    m_size=0;
    m_pEHead=nullptr;
    Vertex* m_pNext=nullptr;
    Edge* m_pEHead=nullptr;
    company="";
}
Vertex::~Vertex()
{
    
    m_key=0;
    m_size=0;
    m_pEHead=nullptr;
    Vertex* m_pNext=nullptr;
    Edge* m_pEHead=nullptr;
    company.clear();
}
void Vertex::SetCompany(string str){this->company=str;}
void Vertex::SetNext(Vertex* pNext) {this->m_pNext=pNext;}

int Vertex::GetKey() const {return m_key;}

Vertex* Vertex::GetNext() const {return m_pNext;}

int Vertex::Size() const {return m_size;}

Edge* Vertex::GetHeadOfEdge() const {return m_pEHead;}

string Vertex::GetCompany() const {return company;}

void Vertex::AddEdge(int edgeKey, int weight)//funtion to add edge in the vertex using linked list
{
    if(m_pEHead==nullptr)
    {
        m_pEHead=new Edge(edgeKey,weight);
        m_size++;//increase size
        return;
    }

    Edge* currEdge = m_pEHead;
    while(currEdge->GetNext()) currEdge=currEdge->GetNext();
    currEdge->SetNext(new Edge(edgeKey,weight));
    m_size++;//increase size
}

