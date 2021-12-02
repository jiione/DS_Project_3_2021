#include "Manager.h"
#include <cstring>

Manager::~Manager()
{
    if (fout.is_open())
        fout.close();

    if (ferr.is_open())
        ferr.close();
}

void Manager::Run(const char* filepath)
{
    std::ifstream fin;
    fout.open(RESULT_LOG_PATH);
    // ferr.open(ERROR_LOG_PATH);
    fin.open(filepath);
    if(!fin) //if can't open file
    {
        PrintError(LoadFileNotExist);//print error code
        return;//and return
    }
    char cmd [100];
    while(!fin.eof())//read command.txt to line by line
    {
        fin.getline(cmd,100);
        char* tmp = strtok(cmd," ");
        if(tmp==NULL)
        {
            continue;
        }
        if(strcmp(tmp,"LOAD")==0)
        {
            PrintError(Load("mapdata.txt"));
        }
        else if(strcmp(tmp,"LOADREPORT")==0)
        {

        }
        else if(strcmp(tmp,"PRINT")==0)
        {

        }
        else if(strcmp(tmp,"BFS")==0)
        {

        }
        else if(strcmp(tmp,"DIJKSTRA")==0)
        {

        }
        else if(strcmp(tmp,"BELLMANFORD")==0)
        {

        }
        else if(strcmp(tmp,"FLOYD")==0)
        {

        }
        else if(strcmp(tmp,"RABINKAPP")==0)
        {

        }
    }

    // TODO: implement
}

void Manager::PrintError(Result result)
{
    fout << "Error code: " << result << std::endl;
}

void Manager::PrintSuccess(char* act)
{
    fout << "========== " << act << " ==========" << endl;
    fout << Success << endl;
    fout << "============================" << endl << endl;
}

/// <summary>
/// make a graph
/// </summary>
///
/// <param name="filepath">
/// the filepath to read to make the graph
/// </param>
///
/// <returns>
/// Result::Success if load is successful.
/// Result::LoadFileNotExist if file is not exist.
/// </returns>
Result Manager::Load(const char* filepath)
{
    ifstream fdata;
    fdata.open(filepath);//open map's data
    if(!fdata) return LoadFileNotExist;//if false to open text of map's data, return LoadFileNotExist

    char d_line[100];
    int Vnum=0;//declare integer for key of vertex
    while(!fdata.eof())
    {
        Vertex* newVertex= new Vertex(Vnum++); //create new vertex
        int Enum=0;//declare integer for key of edge

        fdata.getline(d_line,100);
        char* d_tmp=strtok(d_line,"/");
        while(d_tmp==NULL)//Loop for link edge and vertex
        {
            d_tmp=strtok(NULL," ");
            if(strcmp(d_tmp,"0")!=0)//if Weight is not zero
            { 
                Edge* newEdge = new Edge(Enum,atoi(d_tmp));//if Weight is not zero, declare new Edge
                newVertex ->AddEdge(Enum,atoi(d_tmp));//link edge and vertex
            }
            Enum++;
        }
        // link Vertex and another Vertex
    }
}
/// <summary>
/// print out the graph as matrix form
/// </summary>
///
/// <returns>
/// Result::Success if the printing is successful
/// Result::GraphNotExist if there is no graph
/// </returns>
Result Manager::Print()
{
    // TODO: implement
}
/// <summary>
/// find the path from startVertexKey to endVertexKey with DFS 
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindPathBfs(int startVertexKey, int endVertexKey)
{
    // TODO: implement
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
    // TODO: implement
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::NegativeCycleDetected if exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
    // TODO: implement
}

Result Manager::RabinKarpCompare(const char* CompareString,const char* ComparedString)
{
    // TODO: implement
}
