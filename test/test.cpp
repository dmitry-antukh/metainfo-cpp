#include "pch.h"
#if !defined(__GNUC__)
#	pragma hdrstop
#endif // !defined(__GNUC__)

#include <string>
#include <iostream>
#include <iomanip>
#include <tools/metadata.h>

#include "struct.h"
#include "test.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#if defined(__BORLANDC__)
USEUNIT("test1.cpp");
//---------------------------------------------------------------------------
#pragma argsused
#endif // defined(__BORLANDC__)
//---------------------------------------------------------------------------


using namespace Meta;


void test()
{
    S s;
    print("default ctor:", s);
    
    S s1;
    s1.d=9.876;
    s1.i=4567;
    s1.s="bye";
    s1.s1.i0=7364;
    s1.s1.c = 2.1718281828;
    s1.a_i[0]=0;
    s1.a_i[1]=10;
    s1.a_i[2]=210;
    
    print("modified data", s1);
}

int main(int /*argc*/, char* /*argv*/[])
{
    try
    {
        test();
    }
    catch(std::exception const& e)
    {
        std::cout << "exception with message=" << std::endl << e.what() << std::endl;
    }
    
    return 0;
}
 
