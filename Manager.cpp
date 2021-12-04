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
        if(tmp[0]=='/'&&tmp[1]=='/')//it ignore comment
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
        else if(strcmp(tmp,"BELLMANFORD")==0)//if command is BELLMANFORD
        {
            if(strtok(NULL," ")!=NULL)//if invaild factor, it print error code
            {
                PrintError("InvalidVertexKey",InvalidVertexKey);
                continue;//and ignore command
            }
            result = FindShortestPathBellmanFord(0,m_graph.Size()-1);//Calling function of BellmanFord algorithm
            if(result==NegativeCycleDetected) PrintError("NegativeCycleDetected",NegativeCycleDetected);//When a negative cycle occurs, it print error code 
            else if(result==GraphNotExist) PrintError("GraphNotExist",GraphNotExist);//If the graph does not exist, it print error code
            else if(result==VertexKeyNotExist) PrintError("VertexKeyNotExist",VertexKeyNotExist);//If target is not exist, it print error code
        }
        else if(strcmp(tmp,"FLOYD")==0)//If command is FLOYD
        {
            result = FindShortestPathFloyd();//Calling function of Floyd algorithm
            if(result==NegativeCycleDetected) PrintError("NegativeCycleDetected",NegativeCycleDetected);//When a negative cycle occurs, it print error code
            else if(result==GraphNotExist) PrintError("GraphNotExist",GraphNotExist);//If the graph does not exist, it print error code
        }
        else if(strcmp(tmp,"RABINKARP")==0)//If command is RABINKARP
        {
            tmp=strtok(NULL,"\0");//recive factor including blank
            if(strlen(tmp)>=10)//if factors are too long, it print error code
            {
                PrintError("InvalidOptionNumber",InvalidOptionNumber);
                continue;//and ignore command
            }
            const char* c;
            bool existKey =false;//signal to exist correct sentence
            fout << "========== RABINKARP ==========" << endl;
            for(int i=0;i<report.size();i++)//Repeat until it finish reading the report data
            {
                c=report[i].c_str();//string to const char*
                if(RabinKarpCompare(c,tmp)==Success) existKey = true; //Calling function of RabinKarp algorithm and if result is success, existKey is true
            }
            if(existKey==false) fout<<"NO DUPLICATE TITLE EXISTS"<<endl;//if correct sentence does not exist, it print error code
             fout << "============================" << endl << endl;
        }
        else
        {
            fout << "=======" <<tmp<< "=======" << endl;//if it receive invalid command, it print error code
            fout<< "NonDefinedCommand"<<endl;
            fout << "==========================" << endl << endl;
        }
    }
    m_graph.Clear();//for memory free
}

void Manager::PrintError(Result result)//function to print error code
{
    fout << "============================" << std::endl;
    fout << "Error code: " << result << std::endl;
    fout << "============================" << std::endl << std::endl;
}

void Manager::PrintError(char* act,Result result)//function to print error code
{
    fout << "======= " << act << " =======" << std::endl;
    fout << "Error code: " << result << std::endl;
    fout << "============================" << std::endl << std::endl;
}

void Manager::PrintSuccess(char* act)//function to print success code
{
    fout << "======== " << act << " ========" << std::endl;
    fout << "Success" << std::endl;
    fout << "============================" << std::endl << std::endl;
}

Result Manager::Load(const char* filepath)
{
    ifstream fdata;
    fdata.open(filepath);//open map's data
    if(!fdata) return LoadFileNotExist;//if false to open text of map's data, return LoadFileNotExist

    char d_line[100];
    char* d_tmp;
    int Vnum=0;//declare integer for key of vertex
    while(!fdata.eof())
    {
        fdata.getline(d_line,100);
        if(d_line==NULL) continue;
        m_graph.AddVertex(Vnum);
        Vertex* tmpVertex= m_graph.FindVertex(Vnum++); //tmpVertex's key is Vnum
        int Enum=0;//declare integer for key of edge
        if(d_line==NULL) continue;
        d_tmp=strtok(d_line,"/");
        tmpVertex->SetCompany(d_tmp);
        d_tmp=strtok(NULL," ");
        while(d_tmp!=NULL)//Loop for link edge and vertex
        {
            if(strcmp(d_tmp,"0")!=0)//if Weight is not zero
            { 
                //Edge* newEdge = new Edge(Enum,atoi(d_tmp));//if Weight is not zero, declare new Edge
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
    flep.open(filepath);//open text report data 
    if(!flep) return FaildtoUpdatePath;//if file does not exist, it return FaildtoUpdataPath

    string str;
    while(!flep.eof())//read reportdata's text line by line
    {
        getline(flep,str);//read report data as string
        if(str.empty()) continue;//if string is empty, it ignore
        report.push_back(str);//add string in the vector
    }
    return Success;
}

Result Manager::Print()//function of printing graph
{
    if(m_graph.Size()==0) return GraphNotExist;//if Graph is not exist, return Error code
    m_graph.Print(fout);//Call Print function of Graph
    return Success;
}

Result Manager::FindPathBfs(int startVertexKey, int endVertexKey)
{
    if(m_graph.Size()==0) return GraphNotExist;//if graph does not exist, it return GraphNotExist
    vector<int> v=m_graph.FindPathBfs(startVertexKey,endVertexKey);//Calling function of finding BFS's path
    if(v.size()==0) return VertexKeyNotExist;//If it can't find your destination
    int sum=0;//The weighted value of all paths
    auto it=v.begin();
    if(m_graph.IsNegativeEdge()) fout << "===== InvalidAlgorithm =====" << endl;//if Negative edge exist
    else fout << "======= BFS =======" << endl;
    fout <<"shortest path: "<<*it<<" ";
    Vertex* currVertex=m_graph.FindVertex(startVertexKey);
    Edge* currEdge;
    while(currVertex->GetKey()!=endVertexKey)//to print shortest path and calculate sum
    {
        it++;
        fout<<*it<<" ";//print shortest path
        currEdge=currVertex->GetHeadOfEdge();
        while(currEdge->GetKey()!=*it) currEdge=currEdge->GetNext();//Repeat current path is shortest path 
        sum+=currEdge->GetWeight();//calculate sum
        currVertex=m_graph.FindVertex(currEdge->GetKey());//move vertex to current edge
    }
    fout<<endl;
    fout<<"path length: "<<sum<<endl;//print sum
    fout<<"Course: ";
    for(int i=0; i<v.size();i++)//repeat to print course
    {
        if(v[i]<0) break;
        fout<<m_graph.FindVertex(v[i])->GetCompany()<<" ";
    }
    fout<<endl;
    fout << "============================" << std::endl << std::endl;
    if(m_graph.IsNegativeEdge()) return InvalidAlgorithm;//If Negative edge exist return InvalidAlgorithm
    v.clear();
    return Success;
}

Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)//fuction to find shortest path using dijkstra algorithm
{
    if(m_graph.Size()==0) return GraphNotExist;//if graph does not exist, it return GraphNotExist
    vector<int> v=m_graph.FindShortestPathDijkstraUsingSet(startVertexKey,endVertexKey);//Calling function of finding path using dijkstra algorithm
    if(v.size()==0) return VertexKeyNotExist;//If it can't find your destination
    int sum=0;//The weighted value of all paths
    auto it=v.begin();
    if(m_graph.IsNegativeEdge()) fout << "===== InvalidAlgorithm =====" << endl;//if Negative edge exist
    else fout << "====== DIJKSTRA ======" << endl;
    fout <<"shortest path: "<<*it<<" ";
    Vertex* currVertex=m_graph.FindVertex(startVertexKey);
    Edge* currEdge;
    while(currVertex->GetKey()!=endVertexKey)//to print shortest path and calculate sum
    {
        it++;
        fout<<*it<<" ";
        currEdge=currVertex->GetHeadOfEdge();
        while(currEdge->GetKey()!=*it) currEdge=currEdge->GetNext();//Repeat current path is shortest path 
        sum+=currEdge->GetWeight();
        currVertex=m_graph.FindVertex(currEdge->GetKey());//move vertex to current edge
    }

    fout<<endl;
    fout<<"path length: "<<sum<<endl;//print sum
    fout<<"Course: ";
    for(int i=0; i<v.size();i++)//repeat to print course
    {
        if(v[i]<0) break;
        fout<<m_graph.FindVertex(v[i])->GetCompany()<<" ";
    }
    fout<<endl;
    fout << "============================" << std::endl << std::endl;
    if(m_graph.IsNegativeEdge()) return InvalidAlgorithm;//If Negative edge exist return InvalidAlgorithm
    v.clear();
    return Success;
}

Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)//fuction to find shortest path using bellmanford algorithm
{
    if(m_graph.Size()==0) return GraphNotExist;
    vector<int> v=m_graph.FindShortestPathBellmanFord(startVertexKey,endVertexKey);//Calling function of finding path using bellmanford algorithm
    if(v.size()==0) return VertexKeyNotExist;//If it can't find your destination
    if(v[0]==-1) return NegativeCycleDetected;//If a negative cycle occur, it return NegativeCycleDetected
    int sum=0;//The weighted value of all paths
    auto it=v.begin();
    fout << "========== BELLMANFORD ==========" << endl;
    fout <<"shortest path: "<<*it<<" ";
    Vertex* currVertex=m_graph.FindVertex(startVertexKey);
    Edge* currEdge;
    while(currVertex->GetKey()!=endVertexKey)//to print shortest path and calculate sum
    {
        it++;
        fout<<*it<<" ";
        currEdge=currVertex->GetHeadOfEdge();
        while(currEdge->GetKey()!=*it) currEdge=currEdge->GetNext();//Repeat current path is shortest path 
        sum+=currEdge->GetWeight();
        currVertex=m_graph.FindVertex(currEdge->GetKey());//move vertex to current edge
    }

    fout<<endl;
    fout<<"path length: "<<sum<<endl;//print sum
    fout<<"Course: ";
    for(int i=0; i<v.size();i++)//repeat to print course
    {
        if(v[i]<0) break;
        fout<<m_graph.FindVertex(v[i])->GetCompany()<<" ";
    }
    fout<<endl;
    fout << "============================" << std::endl << std::endl;
    v.clear();
    return Success;
}

Result Manager::FindShortestPathFloyd()//fuction of find all vertex's shortest path using floyd algorithm
{
    if(m_graph.Size()==0) return GraphNotExist;
    vector<vector<int>> v = m_graph.FindShortestPathFloyd();//Calling function of finding path using floyd algorithm
    for(int i=0; i<v.size();i++)//Repeated to check whether negative cycles exist
    {
        if(v[i][i]!=0) return NegativeCycleDetected;//If negative cycles exist, it return NegativeCycleDetected
    }
    fout << "========== FLOYD ==========" << endl;
    for(int i=0;i<m_graph.Size();i++)//print matrix including all shortest path 
    {
        for(int j=0; j<m_graph.Size();j++)
        {
            if(v[i][j]==IN_FINITY) fout<<"INF ";
            else fout<<v[i][j]<<" ";
        }
        fout<<endl;
    }
    fout << "============================" << std::endl << std::endl;
    v.clear();
    return Success;//it return success
}

Result Manager::RabinKarpCompare(const char* CompareString,const char* ComparedString)//function to compare two strings
{
    string line = CompareString;
    string key= ComparedString;
    int lSum=0;//Sum of comparing string's hash
    int kSum=0;//Sum of compared string's hash
    int mul=1;
    for(int i=0; i<key.length();i++)
    {
        if('A'<=key[i]&&key[i]<='Z') key[i]+=32;//Convert upper case letters to lower case letters
        kSum+=key[i]*mul;
        mul*=2;
    }
    for(int i=0; i<=line.length()-key.length();i++)//Repeat to find same string using rabinkarp algorithm
    {
        lSum=0;
        mul=1;
        for(int j=0;j<key.length();j++)
        {
            if('A'<=line[i+j]&&line[i+j]<='Z') line[i+j]+=32;//Convert upper case letters to lower case letters
            lSum+=line[i+j]*mul;
            mul*=2;
        }
        if(lSum==kSum)//If the two strings are the same,
        {
            fout<<CompareString<<endl;//it print Compare String
            return Success;//and it return success
        }
    } 
    return NonExistWord;//f the same string doesn't exist, it return NonExistWord
}
