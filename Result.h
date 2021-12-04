#ifndef RESULT_H
#define RESULT_H

enum Result
{
    Success = 0,
    InvalidOptionNumber,
    NonExistWord,

    FaildtoUpdatePath=5,

    CommandFileNotExist = 100,
    LoadFileNotExist,

    InvalidVertexKey = 200,
    VertexKeyNotExist,
    GraphNotExist,
    InvalidAlgorithm,
    NegativeCycleDetected,

    NonDefinedCommand = 300,
};

#endif