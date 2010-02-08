#ifndef Struct_080909_H
#define Struct_080909_H

#include <tools/metadata.h>


struct Attr1
{
    explicit Attr1(std::string const& s_) : s(s_) {}
    std::string s;
};

template<typename T>
struct S0
{
    T i0;
    
    S0() : i0(125) {}
	
	void add(T const& n) { i0 += n; }

	METADATA(S0)
	{
	    ATTRIBUTES << Attr1("My-S0");
	    
		MEMBER(i0) << Attr1("i0");
		MEMBER(add);
	}
	END_METADATA
};

template<typename T, typename C>
struct S1 : public S0<T>
{
    C   c;

    S1() : c() {}
    
    METADATA(S1)
    {
        BASE_CLASS(S0<T>);

        ATTRIBUTES << Attr1("My-S1");

        MEMBER(c) << Attr1("c");
    }
    END_METADATA
};

struct S 
{
    int                 i;
    double              d;
    std::string         s;
    
    int                 a_i[3];
    
    S1<int, double>     s1;

    S() : i(31415), d(2.718281828), s("hello world") 
    {
        a_i[0]=111;
        a_i[1]=22;
        a_i[2]=3;

        s1.c = 3.14159265;
    }

	METADATA(S)
	{
	    ATTRIBUTES << Attr1("My-S");
	    
		MEMBER(i)  	<< Attr1("i");
		MEMBER(d)	<< Attr1("d");
		MEMBER(s)	<< Attr1("s");
		MEMBER(s1)	<< Attr1("s1");
		MEMBER(a_i)	<< Attr1("a_i");
	} 
	END_METADATA
};



#endif // Struct_080909_H
