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
    if(m_pVHead == nullptr)//is Empty Graph
    {
        m_pVHead=new Vertex(vertexKey);//Graph's head is newVertex
        m_vSize++;//increase Graph's size
        return;
    }

    Vertex* currVertex = m_pVHead;//Vertex node to move in Graph
    while(currVertex->GetNext())
        currVertex=currVertex->GetNext();//move in Graph
    currVertex->SetNext(new Vertex(vertexKey));//insert vertex in Graph
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
    
    vector<int> distance(m_vSize,IN_FINITY);//array to check distance
    vector<int> prePath(m_vSize,-1);//array for previous path
    vector<int> path;//array for shortest path
    set<int> s;//Set for nodes that haven't visited
    Edge* currEdge;
    Vertex* currVertex=m_pVHead;
    while(currVertex)//Repeat to put all vertex in the set
    {
        if(currVertex->GetKey()!=startVertexKey) s.insert(currVertex->GetKey());//except starting vertex
        currVertex=currVertex->GetNext();
    }
    currVertex=FindVertex(startVertexKey);//current vertex is starting vertex
    distance[startVertexKey]=0;//starting vertex's distance is zero
    while(!s.empty())//Repeat until set is empty
    {
        currEdge=currVertex->GetHeadOfEdge();//current edge is current vertex's first path
        while(currEdge)//Repeat until it finds all path
        {
            if(currEdge->GetWeight()<0)//if edge's weight is negative,it ignore this path
            {
                currEdge=currEdge->GetNext();
               continue;
            }
            if(distance[currEdge->GetKey()]>(distance[currVertex->GetKey()]+currEdge->GetWeight()))//If you find the best route, update the route
            {
                prePath[currEdge->GetKey()]=currVertex->GetKey();//update previous path
                distance[currEdge->GetKey()]=distance[currVertex->GetKey()]+currEdge->GetWeight();//update distance
            }
            currEdge=currEdge->GetNext();
        }
        auto min=s.begin();
        for(auto it=s.begin();it!=s.end();it++)//find shortest edge and move the vertex
        {
            if(distance[*it]<distance[*min]) min=it;
        }        
        currVertex=FindVertex(*min);
        s.erase(min);//it erase visited vertex in the set
    }
    if(distance[endVertexKey]==IN_FINITY)//If there's no path
    {
        path.clear();
        return path;//it return empty vector
    }
    int k=FindVertex(endVertexKey)->GetKey();//k is previous path's key
    while(k!=startVertexKey)//Repeat until k is startVertexKey
    {
        path.insert(path.begin(),k);//insert path in vector
        k=prePath[k];
    }
    path.insert(path.begin(),startVertexKey);//inset starting vertex in the path
    prePath.clear();
    return path;//it return path
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
    distance[startVertexKey]=0;//initialize starting vertex
    for(int x=0;x<n;x++)//Repeat the number of Vertex
    {
        currVertex=FindVertex(startVertexKey);
        for(int y=0;y<n;y++)
        {
            if(distance[y]!=IN_FINITY)//if vertex of y has already been updated,
            {
                currEdge=currVertex->GetHeadOfEdge();
                while(currEdge)//Explore all the paths of vertex
                {
                    k=currEdge->GetKey();
                    if(distance[k]>distance[y]+currEdge->GetWeight())//If it find the best route, update that route
                    {
                        if(x==n-1)//If the route is updated on N th repeat, 
                        {
                            path.push_back(-1);//mark for error beacause there's a negative cycle
                            return path;//and end funtion
                        }
                        distance[k]=distance[y]+currEdge->GetWeight();//update path's distance
                        prePath[k]=y;//update previous path
                    }
                    currEdge=currEdge->GetNext();
                }
            }
            currVertex=currVertex->GetNext();
        }
    }
    if(distance[endVertexKey]==IN_FINITY)//If there's no path
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
    path.insert(path.begin(),startVertexKey);//insert starting  Vertex
    prePath.clear();
    return path;//return path
}

std::vector<vector<int>> Graph::FindShortestPathFloyd()
{
    vector<vector<int>> matrix(m_vSize,vector<int>(m_vSize,IN_FINITY));//matrix for shortest distance
    Vertex* currVertex = m_pVHead;
    Edge* currEdge;
    for(int i=0;i<m_vSize;i++)//To Repeat initializing matrix
    {
        currEdge= currVertex->GetHeadOfEdge();
        for(int j=0;j<m_vSize;j++)
        {
            if(currEdge)
            {
                if(currEdge->GetKey()==j)
                {
                    matrix[i][j]=currEdge->GetWeight();
                    if(currEdge->GetNext()!=nullptr)currEdge=currEdge->GetNext();
                }
            }
            if(i==j) matrix[i][j] =0;
        }
        currVertex=currVertex->GetNext();
    }

    for(int x=0;x<m_vSize;x++)//x is vertex of passing 
    {
        for(int y=0;y<m_vSize;y++)//y for source vertex
        {
            for(int z=0; z<m_vSize;z++)//x for destination vertex 
            {
                if(matrix[y][x]==IN_FINITY || matrix[x][z]==IN_FINITY) continue;
                if(matrix[y][z] > matrix[y][x]+matrix[x][z]) matrix[y][z]=matrix[y][x]+matrix[x][z];//update better path
            }
        }
    }
    return matrix;//it return matrix
}

 bool Graph::IsNegativeEdge()//function to check that negative edge exist
 {
    Vertex* currVertex = m_pVHead;
    Edge* currEdge;
    while(currVertex)//tour in graph
    {
        currEdge=currVertex->GetHeadOfEdge();
        while(currEdge)
        {
            if(currEdge->GetWeight()<0) return true;//if negative edge exist, it return true
            currEdge=currEdge->GetNext();
        }
        currVertex=currVertex->GetNext();
    }
    return false;//If negative edge does not exist, it return false
 }

void Graph::Clear()
{
    Vertex* deleteVertex;
    Vertex* currVertex=m_pVHead;
    Edge* currEdge;
    Edge* deleteEdge;
    for(int i=0;i<m_vSize;i++)//tour in graph
    {
        currEdge=currVertex->GetHeadOfEdge();
        while(currEdge)//Repeat until it delete Edge of the vertex
        {
            deleteEdge=currEdge;
            currEdge=currEdge->GetNext();
            delete deleteEdge;
            deleteEdge=nullptr;
        }
        deleteVertex=currVertex;
        currVertex=currVertex->GetNext();
        delete deleteVertex;//Delete all edges of the vertex and then delete vertex
        deleteVertex=nullptr;
    }
}