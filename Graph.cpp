#include "Graph.h"
#include "Stack.h"
#include "Queue.h"
#include "MinHeap.h"
#include "vector"
#include <set>

#define BFS_FIRST_PATH

Graph::Graph()
{
    m_pVHead=nullptr;
    m_vSize=0;
}
Graph::~Graph()
{
    // TODO: implement
}

int Graph::Size() const{return m_vSize;}

void Graph::AddVertex(int vertexKey)
{
    Vertex* newVertex = new Vertex(vertexKey);//create new Vertex
    if(m_pVHead == nullptr)//is Empty Graph
    {
        m_pVHead=newVertex;//Graph's head is newVertex
        m_vSize++;//increase Graph's size
        return;
    }

    Vertex* currVertex = m_pVHead;//Vertex node to move in Graph
    while(currVertex->GetNext()) currVertex=currVertex->GetNext();//move in Graph
    currVertex->SetNext(newVertex);//insert vertex in Graph
    m_vSize++;//increase Graph's size
}

Vertex* Graph::FindVertex(int key)
{
    if(m_vSize==0) return nullptr;//if Graph is empty return NULL

    Vertex* currVertex= m_pVHead;
    while(currVertex)
    {
        if(currVertex->GetKey()==key) break;
        currVertex= currVertex->GetNext();
    }
    return currVertex;
}

void Graph::Print(std::ofstream& fout)//Print Data of Map
{
    Vertex* currVertex= m_pVHead;//Vertex to move point
    fout << "========== PRINT ==========" << std::endl;
    for(int i=0; i<m_vSize;i++)//Loop moving to all vertexes of Graph
    {
        Edge* currEdge=currVertex->GetHeadOfEdge();//Edge to move point
        for(int j=0; j<m_vSize;j++)//Loop to print Graph's row
        {
            if(currEdge==nullptr) fout<< 0 <<" ";
            else if(j==currEdge->GetKey())//if path is existed
            {
                fout<<currEdge->GetWeight()<<" ";//print the path's weight
                currEdge=currEdge->GetNext();
            }
            else fout<< 0 <<" ";//otherwise print zero
        }
        fout << std::endl;
        currVertex=currVertex->GetNext();
    }
    fout << "============================" << std::endl << std::endl;
}

std::vector<int> Graph::FindPathBfs(int startVertexKey, int endVertexKey)
{
    bool visit[m_vSize]={unvisited, };//array to check visited vertex
    vector<Vertex*> prePath(m_vSize,nullptr);//array for previous path
    vector<int> path;//array for shortest path
    Queue<Vertex*> q;//queue for BFS

    visit[startVertexKey]=visited;//mark starting vertex
    Vertex* currVertex=FindVertex(startVertexKey);//currVertex is Vertex for moving a path
    Vertex* preVertex;
    Edge* currEdge;//currEdge is Edge for moving a path
    q.push(currVertex);//push starting Vertex in queue
    while(!q.empty())//Repeat until the queue is empty.
    {
        preVertex=currVertex;
        currVertex = q.val[q.front];//currVertex is front
        q.pop();//pop front in queue
        currEdge=currVertex->GetHeadOfEdge();//initialize currEdge to first edge
        while(currEdge)//Repeat untill the currEdge is NULL
        {
            if(currEdge->GetWeight()<0)
            {
                path.insert(path.begin(),-1);//mark InvalidAlgorithm error
                currEdge=currEdge->GetNext();
                continue;
            }
            if(visit[currEdge->GetKey()]==unvisited)//if currEdge is not visited path
            {
                prePath[currEdge->GetKey()]=currVertex;//mark previous path
                if(currEdge->GetKey()==endVertexKey) 
                {
                    q.clear();
                    break;
                }
                q.push(FindVertex(currEdge->GetKey()));//push currEdge in queue
                visit[currEdge->GetKey()]=visited;//mark visited path
            }
            currEdge=currEdge->GetNext();//move to the next path
        }

    }
    Vertex* tmpVertex=prePath[endVertexKey];//tmpVertex is previous path
    int k=FindVertex(endVertexKey)->GetKey();//k is previous path's key
    while(k!=startVertexKey)//Repeat until k is startVertexKey
    {
        path.insert(path.begin(),k);//insert path in vector
        tmpVertex=prePath[k];//tmpVertex is moving on the previous path
        k=tmpVertex->GetKey();
    }
    path.insert(path.begin(),startVertexKey);
    prePath.clear();
    return path;
}