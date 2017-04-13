#include <iostream>
#include "marshal.hpp"

//#define DUMP
//#define COMPILE
#error "please define DUMP or COMPILE"
//#define TEST

int main(int argc, char* argv[])
{
    #if defined(DUMP)
    Marshal marshal;
    if(argc < 2)
    {
        std::cout << "Drag and drop CommonEvents.rxdata to dump the content" << std::endl;
    }
    else
    {
        if(marshal.read(argv[1]))
        {
            std::cout << "Failed to read " << argv[1] << std::endl;
        }
        else
        {
            std::cout << "Dumping " << argv[1] << " to dump.cevent..." << std::endl;
            if(!marshal.dumpCommonEvents("dump.cevent")) std::cout << "Dump failed" << std::endl;
            else std::cout << "Success" << std::endl;
        }
    }
    #elif defined(COMPILE)
    Marshal marshal;
    if(argc < 2)
    {
        std::cout << "Drag and drop your file to compile" << std::endl;
    }
    else
    {
        std::cout << "Compiling " << argv[1] << " to CommonEvents.rxdata..." << std::endl;
        if(marshal.compile(argv[1])) std::cout << "Success" << std::endl;
    }
    #elif defined(TEST)
    Marshal marshal;
    marshal.read("Map.rxdata");
    marshal.dump("dump.txt");
    #endif

    system("PAUSE");
    return 0;
}
