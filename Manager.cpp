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
    Result result;
    std::ifstream fin;
    fout.open(RESULT_LOG_PATH);
    //ferr.open(ERROR_LOG_PATH);
    fin.open(filepath);
    if(!fin) //if can't open file
    {
        PrintError(CommandFileNotExist);//print error code
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
            tmp= strtok(NULL," ");
            result = Load(tmp);
            if(result==Success)
            {
                PrintSuccess("LOAD");
                PrintError(result);
            }
            else if(result==LoadFileNotExist) PrintError("LoadFileNotExist",result);//Call function of Load and print result
        }
        else if(strcmp(tmp,"LOADREPORT")==0)
        {
            tmp=strtok(NULL," ");
            result = Load(tmp);
            if(result==Success)
            {
               PrintSuccess("LOAD");//Print Success code
               PrintError(Success);
            }
            PrintError(LoadReport(tmp));//Call function of LoadReport and print result
        }
        else if(strcmp(tmp,"PRINT")==0)
        {
            if(Print()==GraphNotExist) PrintError("GraphNotExist",GraphNotExist);
        }
        else if(strcmp(tmp,"BFS")==0)
        {
            if(strtok(NULL," ")!=NULL)
            {
                PrintError("VertexKeyNotExist",VertexKeyNotExist);
                continue;
            }
            if(FindPathBfs(0,m_graph.Size()-1)==InvalidAlgorithm) PrintError(InvalidAlgorithm);
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
    fout << "============================" << std::endl;
    fout << "Error code: " << result << std::endl;
    fout << "============================" << std::endl << std::endl;
}

void Manager::PrintError(char* act,Result result)
{
    fout << "========== " << act << " ==========" << std::endl;
    fout << "Error code: " << result << std::endl;
    fout << "============================" << std::endl << std::endl;
}

void Manager::PrintSuccess(char* act)
{
    fout << "========== " << act << " ==========" << std::endl;
    fout << "Success" << std::endl;
    fout << "============================" << std::endl << std::endl;
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
    std::ifstream fdata;
    fdata.open(filepath);//open map's data
    if(!fdata) return LoadFileNotExist;//if false to open text of map's data, return LoadFileNotExist

    char d_line[100];
    int Vnum=0;//declare integer for key of vertex
    while(!fdata.eof())
    {
        m_graph.AddVertex(Vnum);
        Vertex* tmpVertex= m_graph.FindVertex(Vnum++); //tmpVertex's key is Vnum
        int Enum=0;//declare integer for key of edge
        fdata.getline(d_line,100);
        char* d_tmp=strtok(d_line,"/");
        tmpVertex->SetCompany(d_tmp);
        d_tmp=strtok(NULL," ");
        while(d_tmp!=NULL)//Loop for link edge and vertex
        {
            if(strcmp(d_tmp,"0")!=0)//if Weight is not zero
            { 
                Edge* newEdge = new Edge(Enum,atoi(d_tmp));//if Weight is not zero, declare new Edge
                tmpVertex ->AddEdge(Enum,atoi(d_tmp));//link edge and vertex
            }
            Enum++;
            d_tmp=strtok(NULL," ");
        }
    }
    return Success;//retuen Success
}

Result Manager::LoadReport(const char* filepath)
{
    
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
    if(m_graph.Size()==0) return GraphNotExist;//if Graph is not exist, return Error code
    m_graph.Print(fout);//Call Print function of Graph
    return Success;
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
    vector<int> v=m_graph.FindPathBfs(startVertexKey,endVertexKey);
    int sum=0;
    auto it=v.begin();
    fout << "========== BFS ==========" << endl;
    fout <<"shortest path: "<<*it<<" ";
    Vertex* currVertex=m_graph.FindVertex(startVertexKey);
    Edge* currEdge;
    while(currVertex->GetKey()!=endVertexKey)
    {
        it++;
        fout<<*it<<" ";
        currEdge=currVertex->GetHeadOfEdge();
        while(currEdge->GetKey()!=*it) currEdge=currEdge->GetNext();
        sum+=currEdge->GetWeight();
        currVertex=m_graph.FindVertex(currEdge->GetKey());
    }

    fout<<endl;
    fout<<"path length: "<<sum<<endl;
    fout<<"Course: ";
    for(int i=0; i<v.size()-2;i++)
    {
        fout<<m_graph.FindVertex(v[i])->GetCompany()<<" ";
    }
    fout<<endl;
    fout << "============================" << std::endl << std::endl;
    if(v[v.size()-1]==-1) return InvalidAlgorithm;
    return Success;
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
