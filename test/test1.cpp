#include "pch.h"
#if !defined(__GNUC__)
#	pragma hdrstop
#endif // !defined(__GNUC__)

#include <string>
#include <iostream>
#include <iomanip>
#include <tools/metadata.h>

#include "PrintData.h"
#include "struct.h"
#include "test.h"

using namespace Meta;

void print(std::string const& descr, S const& s)
{
    std::cout << std::endl << "====================================" << std::endl;
    std::cout << descr;
    std::cout << std::endl << "====================================" << std::endl;
    
    PrintStruct<S> treater(s);
    
    Meta::treatData<S>(treater);
}
