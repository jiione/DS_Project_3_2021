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
                q.push(FindVertex(currEdge->GetKey()));//push currEdge in queue
                visit[currEdge->GetKey()]=visited;//mark visited path
                if(currEdge->GetKey()==endVertexKey) 
                {
                    q.clear();
                    break;
                }
            }
            currEdge=currEdge->GetNext();//move to the next path
        }
    }
    if(visit[endVertexKey]==unvisited)
    {
        path.clear();
        return path;
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

std::vector<int> Graph::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
    vector<int> distance(m_vSize,IN_FINITY);
    vector<int> prePath(m_vSize,-1);
    vector<int> path;
    set<int> s;
    Edge* currEdge;
    Vertex* currVertex=m_pVHead;
    while(currVertex)
    {
        if(currVertex->GetKey()!=startVertexKey) s.insert(currVertex->GetKey());
        currVertex=currVertex->GetNext();
    }
    currVertex=FindVertex(startVertexKey);
    distance[startVertexKey]=0;
    while(!s.empty())
    {
        currEdge=currVertex->GetHeadOfEdge();
        while(currEdge)
        {
            if(currEdge->GetWeight()<0)
            {
                path.insert(path.begin(),-1);//mark InvalidAlgorithm error
                currEdge=currEdge->GetNext();
                continue;
            }
            if(distance[currEdge->GetKey()]>(distance[currVertex->GetKey()]+currEdge->GetWeight()))
            {
                prePath[currEdge->GetKey()]=currVertex->GetKey();
                distance[currEdge->GetKey()]=distance[currVertex->GetKey()]+currEdge->GetWeight();
            }
            currEdge=currEdge->GetNext();
        }
        auto min=s.begin();
        for(auto it=s.begin();it!=s.end();it++)
        {
            if(distance[*it]<distance[*min]) min=it;
        }        
        currVertex=FindVertex(*min);
        s.erase(min);
    }
    if(distance[endVertexKey]==IN_FINITY)
    {
        path.clear();
        return path;
    }
    int k=FindVertex(endVertexKey)->GetKey();//k is previous path's key
    while(k!=startVertexKey)//Repeat until k is startVertexKey
    {
        path.insert(path.begin(),k);//insert path in vector
        k=prePath[k];
    }
    path.insert(path.begin(),startVertexKey);
    prePath.clear();
    return path;
}

std::vector<int> Graph::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
    int n=m_vSize;
    vector<int> distance(n,IN_FINITY);//distance initialize infinity
    vector<int> path;
    vector<int> prePath(n,-1);//array of previous path
    Vertex* currVertex;
    Edge* currEdge;
    int k;
    distance[startVertexKey]=0;
    for(int x=0;x<n;x++)
    {
        currVertex=FindVertex(startVertexKey);
        for(int y=0;y<n;y++)
        {
            if(distance[y]!=IN_FINITY)
            {
                currEdge=currVertex->GetHeadOfEdge();
                while(currEdge)
                {
                    k=currEdge->GetKey();
                    if(distance[k]>distance[y]+currEdge->GetWeight())
                    {
                        if(x==n-1)
                        {
                            path.push_back(-1);
                            return path;
                        }
                        distance[k]=distance[y]+currEdge->GetWeight();
                        prePath[k]=y;
                    }
                    currEdge=currEdge->GetNext();
                }
            }
            currVertex=currVertex->GetNext();
        }
    }
    if(distance[endVertexKey]==IN_FINITY)
    {
        path.clear();
        return path;
    }
    k=FindVertex(endVertexKey)->GetKey();//k is previous path's key
    while(k!=startVertexKey)//Repeat until k is startVertexKey
    {
        path.insert(path.begin(),k);//insert path in vector
        k=prePath[k];
    }
    path.insert(path.begin(),startVertexKey);
    prePath.clear();
    return path;
}

std::vector<vector<int>> Graph::FindShortestPathFloyd()
{
    vector<vector<int>> matrix(m_vSize,vector<int>(m_vSize,IN_FINITY));
    Vertex* currVertex = m_pVHead;
    Edge* currEdge;
    for(int i=0;i<m_vSize;i++)
    {
        currEdge= currVertex->GetHeadOfEdge();
        for(int j=0;j<m_vSize;j++)
        {
            if(currEdge->GetKey()==j)
            {
                matrix[i][j]=currEdge->GetWeight();
                if(currEdge->GetNext()!=nullptr)currEdge=currEdge->GetNext();
            }
            if(i==j) matrix[i][j] =0;
        }
        currVertex=currVertex->GetNext();
    }

    for(int x=0;x<m_vSize;x++)
    {
        for(int y=0;y<m_vSize;y++)
        {
            for(int z=0; z<m_vSize;z++)
            {
                if(matrix[y][z] > matrix[y][x]+matrix[x][z]) matrix[y][z]=matrix[y][x]+matrix[x][z];
            }
        }
    }
    return matrix;
}

void Graph::Clear()
{
    Vertex* deleteVertex;
    Vertex* currVertex=m_pVHead;
    Edge* currEdge;
    Edge* deleteEdge;
    for(int i=0;i<m_vSize;i++)
    {
        currEdge=currVertex->GetHeadOfEdge();
        for(int j=0;j<currVertex->Size();j++)
        {
            deleteEdge=currEdge;
            currEdge=currEdge->GetNext();
            delete deleteEdge;
            deleteEdge=nullptr;
        }
        deleteVertex=currVertex;
        currVertex=currVertex->GetNext();
        delete deleteVertex;
        deleteVertex=nullptr;
    }
}