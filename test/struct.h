#ifndef Struct_c0c820ed8709b3b05c16_H
#define Struct_c0c820ed8709b3b05c16_H

#include <tools/metadata.h>

		
#define FILL_ARR(a)      for(unsigned i = 0; i < sizeof(a)/sizeof(a[0]); ++i) a[i] = char(i);
#define SUM_ARR(a, res)  for(unsigned i = 0; i < sizeof(a)/sizeof(a[0]); ++i) res += a[i];


struct Attr1
{
    explicit Attr1(std::string const& s_) : s(s_) {}
    std::string s;
};

struct DummyStruct1 {};
struct DummyStruct2 {};
struct DummyStruct3 {};
struct DummyStruct4 {};
struct DummyStruct5 {};

template<typename T>
struct S0
{
    T i0;

	int n1;
	int n2;
	int n3;
	int n4;
	int n5;
    
    char c3[3];
    char c4[4];
    char c5[5];
    char c6[5];
    char c7[5];
    char c8[5];
    
    
    double d1;
    double d2;
    double d3;
    double d4;
    double d5;

    
    S0();
	
	T add(T const& n) { i0 = T(i0 + n); return i0; }
    
    void add2sum(double& res) const; 

	METADATA(S0)
	{
	    ATTRIBUTES << Attr1("My-S0") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
	    
		MEMBER(i0) << Attr1("i0") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4();
		MEMBER(n1) << Attr1("n1") << DummyStruct1() << DummyStruct2() << DummyStruct3() <<                   DummyStruct5();
		MEMBER(n2) << Attr1("n2") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
		MEMBER(n3) << Attr1("n3") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
		MEMBER(n4) << Attr1("n4") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
		MEMBER(n5) << Attr1("n5") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        
        MEMBER(c3) << Attr1("c3") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        MEMBER(c4) << Attr1("c4") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        MEMBER(c5) << Attr1("c5") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        MEMBER(c6) << Attr1("c6") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        MEMBER(c7) << Attr1("c7") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        MEMBER(c8) << Attr1("c8") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        
        MEMBER(d1) << Attr1("d1") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        MEMBER(d2) << Attr1("d2") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        MEMBER(d3) << Attr1("d3") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        MEMBER(d4) << Attr1("d4") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
        MEMBER(d5) << Attr1("d5") << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();

		MEMBER(add) << DummyStruct1() << DummyStruct2() << DummyStruct3() << DummyStruct4() << DummyStruct5();
	}
	END_METADATA
};

template<typename T>
S0<T>::S0() : i0(1000),
		n1(0), n2(0), n3(0), n4(0), n5(0),
		d1(0), d2(0), d3(0), d4(0), d5(0)
{
    FILL_ARR(c3);
    FILL_ARR(c4);
    FILL_ARR(c5);
    FILL_ARR(c6);
    FILL_ARR(c7);
    FILL_ARR(c8);
}
template<typename T>
void S0<T>::add2sum(double& res) const
{
    res += (i0 + n1 + n2 + n3 + n4 + n5 + d1 + d2 + d3 + d4 + d5);
    SUM_ARR(c3, res);
    SUM_ARR(c4, res);
    SUM_ARR(c5, res);
    SUM_ARR(c6, res);
    SUM_ARR(c7, res);
    SUM_ARR(c8, res);
}


template<typename T, typename C>
struct S1 : public S0<T>
{
    C   c;

    S1() : c() {}
    
    
    void add2sum(double& res) const 
    {
        S0<T>::add2sum(res);
        res += c;
    }
    
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
	int 				ii;
    double              d;
    std::string         s;
    
    int                 a_i[3];
    
    S1<int, double>     s1;
	
    S1<float, int>     s2;
	
    S1<short, unsigned char>     s3;

    S() : i(31415), ii(0), d(2.718281828), s("hello world") 
    {
        a_i[0]=111;
        a_i[1]=22;
        a_i[2]=3;

        s1.c = 3.14159265;
    }
    
    INLINE void add2sum(double& res) const;

	METADATA(S)
	{
	    ATTRIBUTES << Attr1("My-S");
	    
		MEMBER(i)  	<< Attr1("i");
		MEMBER(ii);
		MEMBER(d)	<< Attr1("d");
		MEMBER(s)	<< Attr1("s");

		MEMBER(a_i)	<< Attr1("a_i");

		MEMBER(s1)	<< Attr1("s1");
		MEMBER(s2)	<< Attr1("s2");
		MEMBER(s3)	<< Attr1("s3");
	} 
	END_METADATA
};

INLINE void S::add2sum(double& res) const 
{
    res += (i + d);
    
    SUM_ARR(a_i, res);
    
    s1.add2sum(res);
    s2.add2sum(res);
    s3.add2sum(res);
}

struct X
{	
	int		xi;
	double	xd;
	
	explicit X() : xi(57721), xd(0.57721)
	{}
	
    METADATA(X)
    {
        MEMBER(xi);
        MEMBER(xd);
    }
    END_METADATA
};

struct A
{
	int i;
	double d;

	void addN(int n) { i += n; }

	void treatX(int k, X const& x) { std::cout << "A::treatX(k=" << k << "X: xi=" << x.xi << " xd=" << x.xd << " )" << std::endl; }
	
	explicit A() : i(31415), d(3.14159)
	{}

    METADATA(A)
    {
        MEMBER(i);
        MEMBER(d);
        MEMBER(addN);
        MEMBER(treatX);
    }
    END_METADATA
};


struct B
{
	A a;

	METADATA(B)
	{
		MEMBER(a);
	}
	END_METADATA
};

#undef  FILL_ARR
#undef  SUM_ARR

#endif // Struct_c0c820ed8709b3b05c16_H
