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

#pragma package(smart_init)

USEUNIT("test1.cpp");
//---------------------------------------------------------------------------
#pragma argsused

#endif // defined(__BORLANDC__)
//---------------------------------------------------------------------------


using namespace Meta;


//template<typename F>
//void f(F*)
//{
//	klsdf11111111111;
//}
//template<typename F>
//void t(F* f)
//{
//	//double const* s = f;
//}


void test()
{
	//typedef Meta::Detail::TypeTraits<double const>::TypeWithConst		TWOC;
	//typedef Meta::Detail::TypeWithConst_Helper<double>::Result	TWOC;
	//typedef TypeWithConst<double>::Result TWOC;
	//t((TWOC*)0);
	
	//typedef Meta::Detail::SameConstAs<S const, int>::Result	R;
	//typedef Meta::Detail::TypeTraits<S const>::ConstTag			CT;
	//typedef Meta::Detail::ConstMaker<CT, int>::Result			R;

	//t((R*)0);
	
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


    std::cout << std::endl << "==================================== Treater4Data: <<< BEG" << std::endl;
	TreaterFrame<S, Treater4Data>  treater4Data(s1, Treater4Data("s1"));

    Meta::treatData<S>(treater4Data);
    std::cout << std::endl << "==================================== Treater4Data: END >>>" << std::endl;

    print("modified data", s1);

    sumMembers(s1);
	summator(s1);

    sumMembersClassic(s1);
	

    A a;
    printA("struct A:", a);
	std::string const d2s = data2Str(a, '^');
	std::cout << "-------------" << std::endl;
	std::cout << "data2Str: a='" << d2s << "'" << std::endl;

	std::string const sVal = "2718^2.718";
	bool success = str2Data(sVal, '^', a);
	std::cout << "str2Data success=" << success << std::endl;
	
    printA("struct A new value:", a);

	std::cout << "-------------" << std::endl;
	int n = 0;
	bool succ2 = str2Data("-4321", 0, n);
	std::cout << "succ2=" << succ2 << " n=" << n << std::endl;

	std::cout << std::endl << "==================================== ExecMethod: <<< BEG" << std::endl;
	B b;
	CommandTreaterMan commandTreaterMan;
	std::string const bId = "b";
	commandTreaterMan.addData(&b, bId);

	std::string const namePath = std::string("a") + ExecMethod::DELIM_MEMBER + std::string("treatX");
	std::string const args     = std::string("12345") + ExecMethod::DELIM_ARG + data2Str(X(), ExecMethod::DELIM_ARG);
	std::string const cmd      = namePath + ExecMethod::DELIM_FULL_NAME + args;

	bool const commandTreaterManRes = commandTreaterMan.treatCommand(bId, "ExecMethod", cmd);

	std::cout << "commandTreaterMan result=" << commandTreaterManRes << std::endl;
    std::cout << std::endl << "==================================== ExecMethod: END >>>" << std::endl;
}


struct DefsForA
{
    METADATA(A)
    {
        MEMBER(d) << Attr1("user.d");
        MEMBER(i) << Attr1("user.i");
    }
    END_METADATA
};

int main(int /*argc*/, char* /*argv*/[])
{
    Attribute<A>::addAttributes(static_cast<DefsForA*>(NULL));

    std::cout << "!!!!!!!!!!!!" << std::endl;

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
 
