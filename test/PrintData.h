#ifndef Treater2_090208_H
#define Treater2_090208_H

#include <typeinfo>

#include <tools/metadata.h>
#include "struct.h"
#include "treater.h"


#include <sstream>
#include <stdexcept>

template<typename T>
inline bool toStr(T const& val, std::string* s)
{
    std::stringstream ss;
    ss << val;

    bool res=!ss.bad() && !ss.fail();

    if(s && res) *s=ss.str();
    return res;
}

template<typename T>
inline std::string toStr(T const& val)
{
    std::string s;
    if(!toStr(val, &s)) 
        throw std::runtime_error("Cannot convert value to string");
        
    return s;
}


template<typename T>
class PrintStruct
{
public:
	explicit PrintStruct(T const& data, std::string const& s = "") :
        m_data(data),
        m_s(s)
    {
        Attr1 const* userNameT = 
            Meta::Attribute<T_BcbWa>::template query<Attr1>();
        if(NULL != userNameT)
            std::cout << "*** user name of struct =" << userNameT->s << " ***" << std::endl;
    }

    template<typename M, typename Par1>
    void operator() (M (T::*pM)(Par1), unsigned index)
    {
        ::Meta::Name name =
            Meta::Attribute<T_BcbWa>::template get<Meta::Name>(index);
            
        Attr1 const& nameT = 
            Meta::Attribute<T_BcbWa>::template get<Attr1>();
            
        std::cout << "method = " << nameT.s << "::" << name.get() << std::endl;
    }

    template<typename M>
    void operator() (M (T::*pM), unsigned index)
    {
		Attr1 name = Meta::Attribute<T_BcbWa>::template get<Attr1>(index);
                
        typedef typename TypeTraits<M>::TypeKind TK;

        Impl<M>::treatData(m_data.*pM, m_s + "." + name.s, TK());
    }
    template<typename M, unsigned N>
    void operator() (M (T::*pM)[N], unsigned index)
    {
		Attr1 name = Meta::Attribute<T_BcbWa>::template get<Attr1>(index);
        
        typedef typename TypeTraits<M>::TypeKind TK;

        for(unsigned i = 0; i < N; ++i)
        {
			Impl<M>::treatData(
            	(m_data.*pM)[i],
				m_s + "." + name.s + "[" + toStr(i) + "]",
				TK());
        }
    }
private:
    /// non-copyable
    explicit PrintStruct(PrintStruct const&);
    PrintStruct& operator= (PrintStruct const&);

    T const&				m_data;
    std::string             m_s;
private:
    typedef T       T_BcbWa;    /// bcb workaround

	template<typename MT>
	struct Impl
	{
		static void treatData(MT const& m, std::string const& s, StructuredTypeTag)
		{
		    std::cout << "Treat struct " << s <<
		        " (" << typeid(MT).name() << ")" << std::endl;

		    PrintStruct<MT>  ps(m, s);

		    Meta::MemberInfo<PrintStruct<MT>, MT>::inst().treatData(ps);
		}

		static void treatData(MT const& m, std::string const& s, SimpleTypeTag)
		{
		    std::cout << s << " = " << m <<
		        " (" << typeid(MT).name() << ")" << std::endl << std::endl;
		}
	};
};


#endif // Treater2_090208_H
