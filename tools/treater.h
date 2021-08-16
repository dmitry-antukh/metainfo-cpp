#ifndef Treater_62749135a2786233766c_H
#define Treater_62749135a2786233766c_H

#include <iostream>
#include <iomanip>
#include "metadata.h"
#include "AttribTypes.h"


namespace Meta
{

struct NonDataMember {};
struct MethodTypeTag : public NonDataMember {};
struct BaseClassTag  : public NonDataMember {};


struct SimpleTypeTag {};
struct StructuredTypeTag {};


namespace Detail
{
	template<typename T, bool IS_ENUM>
	struct TypeTraits_Impl
	{
		typedef StructuredTypeTag 	TypeKind;
	};

	template<typename T>
	struct TypeTraits_Impl<T, true>
	{
		typedef SimpleTypeTag 		TypeKind;
	};

} /// namespace Detail

template<typename T>
struct TypeTraits 
{ 
	typedef typename Detail::TypeTraits_Impl<T, std::is_enum<T>::value>::TypeKind		TypeKind;
};


template<> struct TypeTraits<int>             
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<unsigned int>           
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<long>                   
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<unsigned long>          
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<short>                  
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<unsigned short>         
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<char>                   
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<unsigned char>          
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<signed char>            
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<bool>
{
	typedef SimpleTypeTag 		TypeKind;
};

template<> struct TypeTraits<double>                 
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<float>                  
{ 
	typedef SimpleTypeTag 	TypeKind; 
};

template<> struct TypeTraits<std::string>            
{ 
	typedef SimpleTypeTag 	TypeKind; 
};


template<typename C>
struct TypeRef_Helper
{
    typedef C											TypeWithoutRef;
};
template<typename C>
struct TypeRef_Helper<C&>
{
    typedef C											TypeWithoutRef;
};

template<typename C>
struct PureType
{
	typedef typename TypeRef_Helper<C>::TypeWithoutRef								TypeWithoutRef;
	typedef typename Meta::Detail::TypeTraits<TypeWithoutRef>::TypeWithoutConst		Result;
};

struct AnyDummy
{
	AnyDummy() {}

	template<typename T>
	AnyDummy(T const&) 
	{}
};

//struct EmptyData {};

template<typename T>
struct UsedT
{
    bool    useValue;
    T       value;

    UsedT() : useValue(false), value() {}
    UsedT(T const& t) : useValue(true), value(t) {}
    explicit UsedT(bool useValue_, T const& t) : useValue(useValue_), value(t) {}

    operator bool() const { return useValue; }
    T const&    operator*() const   { return value; }
    T&          operator*()         { return value; }
};

template<typename T>
UsedT<T> make_used_t(bool usedValue, T const& t) { return UsedT<T>(usedValue, t); }

template<typename T, typename Result, typename Data>
class TreaterFrame_
{
public:
	explicit TreaterFrame_(Data& data, Result const& result) :
		m_data(data),
		m_result(result)
	{}

    template<typename M, typename... Params>
    void operator() (unsigned memberIndex, M (T::*pM)(Params...))
    {
        m_result(MethodTypeTag(), m_data, memberIndex, pM);
    }

    template<typename M>
    void operator() (unsigned memberIndex, M (T::*pM))
    {
        typedef typename TypeTraits<M>::TypeKind								TK;		
		typedef typename Meta::Detail::ConstMaker<ConstTagOfData, M>::Result	MSameConst;

		UsedT<Result> const r = m_result(m_data, memberIndex, pM);

		if(!r)
			return;

		MSameConst& m = (m_data.*pM);

        Impl<M>::treatData(m, *r, TK());
    }

    template<typename M, unsigned N>
    void operator() (unsigned memberIndex, M (T::*pM)[N])
    {
        typedef typename TypeTraits<M>::TypeKind								TK;
		typedef typename Meta::Detail::ConstMaker<ConstTagOfData, M>::Result	MSameConst;

		NeedTreat const* arrTreat = Meta::Attribute<T>::template query<NeedTreat>(memberIndex);

        for(unsigned i = 0; i < N; ++i)
        {
			if (arrTreat)
			{
				NeedTreat::TreatFlag const tf = arrTreat->treatFlag(i);
				if (tf == NeedTreat::TF_BREAK)
					break;
				if (tf == NeedTreat::TF_NO)
					continue;
			}
            UsedT<Result> const r = m_result(m_data, memberIndex, pM, i);

			if(!r)
				continue;

			MSameConst& m = (m_data.*pM)[i];
			
			Impl<M>::treatData(m, *r, TK());
        }		
    }
	
    template<typename M, unsigned N1, unsigned N2>
    void operator() (unsigned memberIndex, M (T::*pM)[N1][N2])
    {
        typedef typename TypeTraits<M>::TypeKind                                TK;
        typedef typename Meta::Detail::ConstMaker<ConstTagOfData, M>::Result    MSameConst;

        for(unsigned i1 = 0; i1 < N1; ++i1)
            for(unsigned i2 = 0; i2 < N2; ++i2)
            {
                UsedT<Result> const r = m_result(m_data, memberIndex, pM, i1, i2);

                if(!r)
                    continue;

                MSameConst& m = (m_data.*pM)[i1][i2];

                Impl<M>::treatData(m, *r, TK());
            }
    }

    //template<typename BaseT>
    //void operator() (unsigned memberIndex, Meta::BaseClass<T, BaseT> const& baseClass)
    //{
    //    m_result(BaseClassTag(), m_data, baseClass);
    //}
	template<typename BaseClassT>
	void operator() (unsigned /*memberIndex*/, Meta::BaseClass<T, BaseClassT> const& /*baseClass*/)
	{
		typedef typename Meta::Detail::ConstMaker<ConstTagOfData, BaseClassT>::Result	BaseClassData;

		TreaterFrame_<BaseClassT, Result, BaseClassData> treater(m_data, m_result);

		Meta::treatData<BaseClassT>(treater);
	}
private:
    /// non-copyable
    explicit TreaterFrame_(TreaterFrame_ const&);
    TreaterFrame_& operator= (TreaterFrame_ const&);
private:
	typedef typename Meta::Detail::TypeTraits<Data>::ConstTag			ConstTagOfData;

	template<typename MT>
	struct Impl
	{
		static void treatData(MT& m, Result const& result, StructuredTypeTag)
		{
		    TreaterFrame_<MT, Result, MT> treater(m, result);

		    Meta::treatData<MT>(treater);
		}

		static void treatData(MT const& m, Result const& result, StructuredTypeTag)
		{
		    TreaterFrame_<MT, Result, MT const> treater(m, result);
		
		    Meta::treatData<MT const>(treater);
		}

		static void treatData(MT& /*m*/, Result const& /*result*/, SimpleTypeTag)
		{
		}

		static void treatData(MT const& /*m*/, Result const& /*result*/, SimpleTypeTag)
		{
		}
	};	
	
private:
	Data& 			m_data;
	Result 			m_result;
};


template<typename Data, typename Result>
class TreaterFrame : public TreaterFrame_<typename Meta::Detail::TypeTraits<Data>::TypeWithoutConst, Result, Data>
{
public:
	explicit TreaterFrame(Data& data, Result const& result) : 
		TreaterFrame_<typename Meta::Detail::TypeTraits<Data>::TypeWithoutConst, Result, Data>(data, result)
	{}
};


} // namespace Meta


#endif // Treater_62749135a2786233766c_H
