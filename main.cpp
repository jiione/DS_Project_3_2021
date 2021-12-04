#include "Manager.h"

int main(int argc, char* argv[])
{
    const char* commandFilepath = "command.txt";//commandFilepath is name of the text file containing the command
    if (argc > 1)
        commandFilepath = argv[1];

    Manager m;
    m.Run(commandFilepath);//call function of Run

    return 0;
}