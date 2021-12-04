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
    Result result;//Variables to show the result screen
    ifstream fin;//
    fout.open(RESULT_LOG_PATH);
    ferr.open(ERROR_LOG_PATH);
    fin.open(filepath);//open text of command 
    if(!fin) //if can't open file
    {
        fout << "======= SYSTEM =======" << endl;
        fout<< "CommandFileNotExist"<<endl;
        fout << "==========================" << endl << endl;//print error code
        return;//and return
    }
    char cmd [100];
    while(!fin.eof())//Repeat until it finish reading the file
    {
        fin.getline(cmd,100);//read command.txt to line by line
        char* tmp = strtok(cmd," ");
        if(tmp==NULL)//If the command reduces the number of blanks, process an exception
        {
            continue;
        }
        if(strcmp(tmp,"LOAD")==0)//if command  is LOAD
        {
            tmp= strtok(NULL," ");//tmp is factor of LOAD command
            result = Load(tmp);//Calling funciton of Load
            if(result==Success)//if result of Load is Success,
            {
                PrintSuccess("LOAD");//print success code
                PrintError(result);
            }
            else if(result==LoadFileNotExist) PrintError("LoadFileNotExist",result);//if file is not existed, print error code
        }
        else if(strcmp(tmp,"LOADREPORT")==0)//if command is LOADREPORT
        {
            tmp=strtok(NULL," ");//tmp is factor of LOAD command
            result = LoadReport(tmp);//Calling function of LoadReport
            if(result==Success)//if result of Load is Success,
            {
               PrintSuccess("LOADREPORT");//Print Success code
               PrintError(Success);
            }
            else if(result==FaildtoUpdatePath) PrintError("FaildtoUpdatePath",FaildtoUpdatePath);//if file is not existed, print error code
        }
        else if(strcmp(tmp,"PRINT")==0)//if command is PRINT
        {
            if(Print()==GraphNotExist) PrintError("GraphNotExist",GraphNotExist);//Calling function of Print and if result is GraphNotExist, print error code
        }
        else if(strcmp(tmp,"BFS")==0)//if command is BFS
        {
            if(strtok(NULL," ")!=NULL)//if invalid factor,it print error code
            {
                PrintError("InvalidVertexKey",InvalidVertexKey);
                continue;//and ignore command
            }
            result=FindPathBfs(0,m_graph.Size()-1);//calling function of FindPahthBfs
            if(result==InvalidAlgorithm) PrintError(InvalidAlgorithm);//if result is InvalidAlgorithm, it print error code
            else if(result==GraphNotExist) PrintError("GraphNotExist",GraphNotExist);//if result is GraphNotExist, it print error code
            else if(result==VertexKeyNotExist) PrintError("VertexKeyNotExist",VertexKeyNotExist);//if result is VertexKeyNotExist, it print error code
        }
        else if(strcmp(tmp,"DIJKSTRA")==0)//if command is DIJKSTRA
        {
            if(strtok(NULL," ")!=NULL)//if invalid factor,it print error code
            {
                PrintError("InvalidVertexKey",InvalidVertexKey);
                continue;//and ignore command
            }
            result = FindShortestPathDijkstraUsingSet(0,m_graph.Size()-1);//Calling function of Dikstra algorithm
            if(result==InvalidAlgorithm) PrintError(InvalidAlgorithm);//if result is InvalidAlgorithm, it print error code
            else if(result==GraphNotExist) PrintError("GraphNotExist",GraphNotExist); //if result is GraphNotExist, it print error code
            else if(result==VertexKeyNotExist) PrintError("VertexKeyNotExist",VertexKeyNotExist);//if result is VertexKeyNotExist, it print error code
        }
        else if(strcmp(tmp,"BELLMANFORD")==0)
        {
            if(strtok(NULL," ")!=NULL)
            {
                PrintError("InvalidVertexKey",InvalidVertexKey);
                continue;
            }
            result = FindShortestPathBellmanFord(0,m_graph.Size()-1);
            if(result==NegativeCycleDetected) PrintError("NegativeCycleDetected",NegativeCycleDetected);
            else if(result==GraphNotExist) PrintError("GraphNotExist",GraphNotExist);
            else if(result==VertexKeyNotExist) PrintError("VertexKeyNotExist",VertexKeyNotExist);
        }
        else if(strcmp(tmp,"FLOYD")==0)
        {
            result = FindShortestPathFloyd();
            if(result==NegativeCycleDetected) PrintError("NegativeCycleDetected",NegativeCycleDetected);
            else if(result==GraphNotExist) PrintError("GraphNotExist",GraphNotExist);
        }
        else if(strcmp(tmp,"RABINKAPP")==0)
        {
            tmp=strtok(NULL,"\0");
            if(strlen(tmp)>=10)
            {
                PrintError("InvalidOptionNumber",InvalidOptionNumber);
                continue;
            }
            const char* c;
            bool existKey =false;
            fout << "========== RABINKARP ==========" << endl;
            for(int i=0;i<report.size();i++)
            {
                c=report[i].c_str();
                if(RabinKarpCompare(c,tmp)==Success) existKey = true; 
            }
            if(existKey==false) fout<<"NO DUPLICATE TITLE EXISTS"<<endl;
             fout << "============================" << endl << endl;
        }
        else
        {
            fout << "======= ASTAR =======" << endl;
            fout<< "NonDefinedCommand"<<endl;
            fout << "==========================" << endl << endl;
        }
    }
    m_graph.Clear();
}

void Manager::PrintError(Result result)
{
    fout << "============================" << std::endl;
    fout << "Error code: " << result << std::endl;
    fout << "============================" << std::endl << std::endl;
}

void Manager::PrintError(char* act,Result result)
{
    fout << "======= " << act << " =======" << std::endl;
    fout << "Error code: " << result << std::endl;
    fout << "============================" << std::endl << std::endl;
}

void Manager::PrintSuccess(char* act)
{
    fout << "======== " << act << " ========" << std::endl;
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
    ifstream fdata;
    fdata.open(filepath);//open map's data
    if(!fdata) return LoadFileNotExist;//if false to open text of map's data, return LoadFileNotExist

    char d_line[100];
    int Vnum=0;//declare integer for key of vertex
    while(!fdata.eof())
    {
        fdata.getline(d_line,100);
        if(d_line==NULL) continue;
        m_graph.AddVertex(Vnum);
        Vertex* tmpVertex= m_graph.FindVertex(Vnum++); //tmpVertex's key is Vnum
        int Enum=0;//declare integer for key of edge
        if(d_line==NULL) continue;
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
    ifstream flep;
    flep.open(filepath);
    if(!flep) return FaildtoUpdatePath;

    string str;
    while(!flep.eof())
    {
        getline(flep,str);
        if(str.empty()) continue;
        report.push_back(str);
    }
    return Success;
}

Result Manager::Print()
{
    if(m_graph.Size()==0) return GraphNotExist;//if Graph is not exist, return Error code
    m_graph.Print(fout);//Call Print function of Graph
    return Success;
}

Result Manager::FindPathBfs(int startVertexKey, int endVertexKey)
{
    if(m_graph.Size()==0) return GraphNotExist;
    vector<int> v=m_graph.FindPathBfs(startVertexKey,endVertexKey);
    if(v.size()==0) return VertexKeyNotExist;
    int sum=0;
    auto it=v.begin();
    if(v[v.size()-1]==-1) fout << "===== InvalidAlgorithm =====" << endl;
    else fout << "======= BFS =======" << endl;
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
    for(int i=0; i<v.size();i++)
    {
        if(v[i]<0) break;
        fout<<m_graph.FindVertex(v[i])->GetCompany()<<" ";
    }
    fout<<endl;
    fout << "============================" << std::endl << std::endl;
    if(v[v.size()-1]==-1) return InvalidAlgorithm;
    v.clear();
    return Success;
}

Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
    if(m_graph.Size()==0) return GraphNotExist;
    vector<int> v=m_graph.FindShortestPathDijkstraUsingSet(startVertexKey,endVertexKey);
    if(v.size()==0) return VertexKeyNotExist;
    int sum=0;
    auto it=v.begin();
    if(v[v.size()-1]==-1) fout << "===== InvalidAlgorithm =====" << endl;
    else fout << "====== DIJKSTRA ======" << endl;
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
    for(int i=0; i<v.size();i++)
    {
        if(v[i]<0) break;
        fout<<m_graph.FindVertex(v[i])->GetCompany()<<" ";
    }
    fout<<endl;
    fout << "============================" << std::endl << std::endl;
    if(v[v.size()-1]==-1) return InvalidAlgorithm;
    v.clear();
    return Success;
}

Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
    if(m_graph.Size()==0) return GraphNotExist;
    vector<int> v=m_graph.FindShortestPathBellmanFord(startVertexKey,endVertexKey);
    if(v.size()==0) return VertexKeyNotExist;
    if(v[0]==-1) return NegativeCycleDetected;
    int sum=0;
    auto it=v.begin();
    fout << "========== BELLMANFORD ==========" << endl;
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
    for(int i=0; i<v.size();i++)
    {
        if(v[i]<0) break;
        fout<<m_graph.FindVertex(v[i])->GetCompany()<<" ";
    }
    fout<<endl;
    fout << "============================" << std::endl << std::endl;
    v.clear();
    return Success;
}

Result Manager::FindShortestPathFloyd()
{
    if(m_graph.Size()==0) return GraphNotExist;
    vector<vector<int>> v = m_graph.FindShortestPathFloyd();
    for(int i=0; i<v.size();i++)
    {
        if(v[i][i]!=0) return NegativeCycleDetected;
    }
    fout << "========== FLOYD ==========" << endl;
    for(int i=0;i<m_graph.Size();i++)
    {
        for(int j=0; j<m_graph.Size();j++)
        {
            if(v[i][j]==IN_FINITY) fout<<1<<" ";
            else fout<<v[i][j]<<" ";
        }
        fout<<endl;
    }
    fout << "============================" << std::endl << std::endl;
    v.clear();
    return Success;
}

Result Manager::RabinKarpCompare(const char* CompareString,const char* ComparedString)
{
    string line = CompareString;
    string key= ComparedString;
    int lSum=0;
    int kSum=0;
    for(int i=0; i<key.length();i++)
    {
        if('A'<=key[i]&&key[i]<='Z') key[i]+=32;
        kSum+=key[i]*key[i];
    }

    for(int i=0; i<=line.length()-key.length();i++)
    {
        lSum=0;
        for(int j=0;j<key.length();j++)
        {
            if('A'<=line[i+j]&&line[i+j]<='Z') line[i+j]+=32;
            lSum+=line[i+j]*line[i+j];
        }
        if(lSum==kSum)
        {
            fout<<CompareString<<endl;
            return Success;
        }
    } 
    return NonExistWord;
}
