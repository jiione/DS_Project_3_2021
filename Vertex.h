#ifndef VERTEX_H
#define VERTEX_H

#ifndef NULL
#define NULL	0
#endif

#include "Edge.h"

class Vertex
{
private:
    // the key of this vertex
    int m_key;
    // the number of the edges from this vertex to others
    int m_size;
    // the head pointer for the linked list of the edges
    Edge* m_pEHead;
    // the next pointer for the linked list of the vertics
    Vertex* m_pNext;

public:
    Vertex();
    Vertex(int key);
    ~Vertex();

    /// <summary>
    /// set the next pointer of this vertex
    /// </summary>
    void SetNext(Vertex* pNext);
    /// <summary>
    /// get the key of this vertex
    /// </summary>
    ///
    /// <returns>
    /// the key of this vertex
    /// </returns>
    int GetKey() const;
    /// <summary>
    /// get the next pointer of this vertex
    /// </summary>
    ///
    /// <returns>
    /// the next pointer of this vertex
    /// </returns>
    Vertex* GetNext() const;

    /// <summary>
    /// get the number of the edges
    /// </summary>
    ///
    /// <returns>
    /// the number of the edges
    /// </returns>
    int Size() const;

    /// <summary>
    /// add edge with edgeNum at the end of the linked list for the edges
    /// </summary>
    ///
    /// <param name="edgeKey">
    /// the key of the vertex for the edge
    /// </param>
    /// <param name="weight">
    /// the weight of the edge
    /// </param>
    void AddEdge(int edgeKey, int weight);
    /// <summary>
    /// get the head pointer of the edge
    /// </summary>
    ///
    /// <returns>
    /// the ehad pointer of the edge
    /// </returns>
    Edge* GetHeadOfEdge() const;
    /// <summary>
    /// memory free for edges
    /// </summary>
    void Clear();
};

#endif
