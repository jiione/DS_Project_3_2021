#define _CRT_SECURE_NO_WARNINGS
#ifndef SOLUTION_H
#define SOLUTION_H

#include "Result.h"
#include "Graph.h"
#include <fstream>

class Manager
{
private:
    // the filepath for the result log
    const char* RESULT_LOG_PATH = "log.txt";
    // the filepath for the error log
    const char* ERROR_LOG_PATH="log.txt";

    // the file stream for the result log
    std::ofstream fout;
    // the file stream for the error log
    std::ofstream ferr;
    // graph instance to manage the vertics.
    Graph m_graph;

    vector<string> report;

public:
    ~Manager();
    void Run(const char* filepath);
    void PrintError(Result result);
    void PrintError(char*act,Result result);
    void PrintSuccess(char* act);

private:
    Result Load(const char* filepath);
    Result LoadReport(const char* filepath);
    Result Print();
    Result FindPathBfs(int startVertexKey, int endVertexKey);//find shortest path using bfs algorithm except negetive edge
    Result FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey);//find shortest path using dijkstra algorithm except negative edge
    Result FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey);
    Result FindShortestPathBellmanFord(int startVertexKey, int endVertexKey);//find shortest path  using bellmanford alforithm including negative edge
    Result FindShortestPathFloyd();//fuction of find all vertex's shortest path using floyd algorithm
    Result RabinKarpCompare(const char* CompareString, const char* ComparedString);//compare two string using rabinkarp algorithm
};

#endif
