#include "pch.h"
#if !defined(__GNUC__)
#	pragma hdrstop
#endif // !defined(__GNUC__)

#include <string>
#include <iostream>
#include <iomanip>
#include <sys/timeb.h>
#include <time.h>


#include <tools/metadata.h>

#include "PrintData.h"
#include "struct.h"
#include "test.h"

#if defined(__BORLANDC__)
    #pragma package(smart_init)
#endif // defined(__BORLANDC__)


using namespace Meta;

void print(std::string const& descr, S const& s)
{
    std::cout << std::endl << "====================================" << std::endl;
    std::cout << descr;
    std::cout << std::endl << "====================================" << std::endl;

	TreaterFrame<S const, PrintStruct> treater(s, PrintStruct::make<S>(std::string("s")));
    Meta::treatData<S>(treater);
}

void printA(std::string const& descr, A const& s)
{
    std::cout << std::endl << "====================================" << std::endl;
    std::cout << descr;
    std::cout << std::endl << "====================================" << std::endl;

	TreaterFrame<A const, PrintStruct> treater(s, PrintStruct::make<A>(std::string("a")));
    Meta::treatData<A>(treater);
}

double getCurTime()
{
    timeb tb = {0, 0, 0, 0};
    ftime(&tb);
    
    double const res = tb.time + double(tb.millitm)/1000;
    return res;
   
}

int const NN = 1000000;
    
void sumMembers(S const& s)
{
    std::cout << std::endl << "====================================" << std::endl;
    std::cout << "SumMembers:";
    std::cout << std::endl << "====================================" << std::endl;
    
    S s1 = s;
    
    double res = 0;
    SumMembers<S> treater(s1, res);

    Meta::treatData<S>(treater);
    std::cout << " sum = " << std::setprecision(10) << res << std::endl;
    
    double t0 = getCurTime();
        
    for(int i = 0; i < NN-1; ++i)
    {    
        Meta::treatData<S>(treater);
    }
    double t1 = getCurTime();
    
    std::cout << std::endl << "====================================" << std::endl;
    std::cout << "SumMembers time=" << (t1 - t0) << " sum = " << std::setprecision(10) << res;
    std::cout << std::endl << "====================================" << std::endl;
}

void summator(S const& s)
{
    std::cout << std::endl << "====================================" << std::endl;
    std::cout << "Summator:";
    std::cout << std::endl << "====================================" << std::endl;
    
	///typedef S const SS;
	//typedef S SS;

    S const s1 = s;
    
    double res = 0;
    TreaterFrame<S const, Summator> treater(s1, res);
	
	Meta::treatData<S>(treater);
    std::cout << " sum = " << std::setprecision(10) << res << std::endl;
    
    double t0 = getCurTime();
        
    for(int i = 0; i < NN-1; ++i)
    {    
        Meta::treatData<S>(treater);
    }
    double t1 = getCurTime();
    
    std::cout << std::endl << "====================================" << std::endl;
    std::cout << "Summator time=" << (t1 - t0) << " sum = " << std::setprecision(10) << res;
    std::cout << std::endl << "====================================" << std::endl;
	
}

void sumMembersClassic(S const& s)
{
    std::cout << std::endl << "====================================" << std::endl;
    std::cout << "sumMembersClassic:";
    std::cout << std::endl << "====================================" << std::endl;
    
    S s1 = s;
    
    double res = 0;	

    s1.add2sum(res);
    std::cout << " sum = " << std::setprecision(10) << res << std::endl;

    double t0 = getCurTime();
            
    for(int i = 0; i < NN-1; ++i)
    {
        s1.add2sum(res);
    }
    double t1 = getCurTime();
    
    std::cout << std::endl << "====================================" << std::endl;
    std::cout << "sumMembersClassic time=" << (t1 - t0) << " sum = " << std::setprecision(10) << res;
    std::cout << std::endl << "====================================" << std::endl;
}
