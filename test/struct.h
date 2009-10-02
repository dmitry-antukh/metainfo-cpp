#ifndef Struct_090208_H
#define Struct_090208_H

#include <Tools/metadata.h>


struct Attr1
{
    explicit Attr1(std::string const& s_) : s(s_) {}
    std::string s;
};

struct S0
{
    int i0;
    
    S0() : i0(125) {}
	
	void add(int n) { i0 += n; }

	METADATA(S0)
	{
	    ATTRIBUTES << Attr1("S0");
	    
		MEMBER(i0) << Attr1("i0");
		MEMBER(add);
	}
	END_METADATA
};

struct S 
{
    int         i;
    double      d;
    std::string s;
    
    int         a_i[3];
    
    S0 s0;

    S() : i(31415), d(2.718281828), s("hello world") 
    {
        a_i[0]=111;
        a_i[1]=22;
        a_i[2]=3;
    }

	METADATA(S)
	{
	    ATTRIBUTES << Attr1("S");
	    
		MEMBER(i)  	<< Attr1("i");
		MEMBER(d)	<< Attr1("d");
		MEMBER(s)	<< Attr1("s");
		MEMBER(s0)	<< Attr1("s0");
		MEMBER(a_i)	<< Attr1("a_i");
	} 
	END_METADATA
};



#endif // Struct_090208_H

