#ifndef PrintData_177c62be401710ca0560_H
#define PrintData_177c62be401710ca0560_H

#include <typeinfo>

#include <tools/metadata.h>
#include <tools/treater.h>
#include "struct.h"

#include <iterator>
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
inline bool fromStr(std::string const& s, T* res)
{
    std::istringstream ss(s.c_str());
    if(res) ss >> (*res);
    else
    {
        T f=T();
        ss >> f;
    }
    if(!ss.eof()) std::ws(ss);
    return !ss.bad() && !ss.fail() && ss.eof();
}

template<typename T>
inline T const fromStr(std::string const& s)
{
    T val=T();
    if(!fromStr(s, &val)) 
        throw std::runtime_error(std::string("Cannot convert string '") + s.c_str() + "'");
        
    return val;
}

template<typename StrType>
#if !defined(__BORLANDC__)
inline 
#endif // !defined(__BORLANDC__) BCB fails with Internal Compiler Error
StrType trimStr(StrType const& s)
{
    static StrType const BL = " \t\r\n";
    typename StrType::size_type p1=s.find_first_not_of(BL);
    
    if(p1==StrType::npos) return "";
    
    typename StrType::size_type p2=s.find_last_not_of(BL);
    
    return s.substr(p1, p2-p1+1);
}

template<typename StringType, typename OutputIterator>
inline
void parseStr(StringType const& source, StringType const& delimiters, 
              OutputIterator iter)
{
    typename StringType::size_type i=0;
    while(i<source.length())
    {
        typename StringType::size_type p=source.find_first_not_of(delimiters, i);
        if(p==StringType::npos) return;

        i=source.find_first_of(delimiters, p);

        *iter=source.substr(p, i-p);
        ++iter;
    }
}

inline bool getStrParts(std::string expr, char delim, std::string* s1, std::string* s2, bool trimResuls = true)
{
    std::string::size_type p=expr.find(delim);
    if(std::string::npos == p) return false;

    if(s1)
    {
        *s1=expr.substr(0, p);

        if(trimResuls)
            *s1 = trimStr(*s1);
    }
    if(s2)
    {
        *s2=expr.substr(p+1);

        if(trimResuls)
            *s2 = trimStr(*s2);
    }
    return true;
}

class PrintStruct
{
public:
	template<typename T>
	static PrintStruct make(std::string const& s)
	{
		if(!s.empty())
			std::cout << "Data name = '" << s << "'";

        Attr1 const* userNameT = 
            Meta::Attribute<T>::template query<Attr1>();

        if(NULL != userNameT)
            std::cout << " *** user name of struct =" << userNameT->s << " ***";
		std::cout << std::endl;

		return PrintStruct(s);
	}
	
    template<typename T, typename M, typename Par1>
    void operator() (Meta::MethodTypeTag const&, T const&, unsigned memberIndex, M (T::* /*pM*/)(Par1))
    {
        std::cout << "method = " << m_s << "::" << getMemberName<T>(memberIndex) << "(" << typeid(Par1).name() << ")" << std::endl;		
        std::cout << "TypeWithoutRef= " << typeid(TypeRef_Helper<Par1>::TypeWithoutRef).name() << std::endl;
    }
    template<typename T, typename M, typename Par1, typename Par2>
    void operator() (Meta::MethodTypeTag const&, T const&, unsigned memberIndex, M (T::* /*pM*/)(Par1, Par2))
    {
        std::cout << "method = " << m_s << "::" << getMemberName<T>(memberIndex) << "(" << 
			typeid(Par1).name() <<", " << 
			typeid(Par2).name() << ")" << std::endl;
        std::cout << "Type2WithoutRef= " << typeid(TypeRef_Helper<Par2>::TypeWithoutRef).name() << std::endl;
    }

    template<typename T>
    void operator() (Meta::MethodTypeTag const&, T const&, unsigned memberIndex, Meta::AnyDummy)
    {
        std::cout << "method = " << m_s << "::" << getMemberName<T>(memberIndex) << "(???)" << std::endl;
    }

    template<typename T, typename M>
    PrintStruct const operator() (T const& data, unsigned memberIndex, M (T::*pM))
    {
		std::string const s = m_s + "." + getMemberName<T>(memberIndex);

        typedef typename TypeTraits<M>::TypeKind TK;

		PrintValue<M>::treatData((data.*pM), s,  TK());        	

		return PrintStruct(s);
    }
    template<typename T, typename M, unsigned N>
    PrintStruct const operator() (T const& data, unsigned memberIndex, M (T::*pM)[N], unsigned i)
    {
		std::string const s = m_s + "." + getMemberName<T>(memberIndex) +  "[" + toStr(i) + "]";

        typedef typename TypeTraits<M>::TypeKind TK;

        PrintValue<M>::treatData((data.*pM)[i], s,  TK());        	

		return PrintStruct(s);
    }
private:
	explicit PrintStruct(std::string const& s) :
        m_s(s)
    {
    }

	template<typename T>
	std::string getMemberName(unsigned memberIndex) const
	{
		Meta::Name const& n = Meta::Attribute<T>::template get<Meta::Name>(memberIndex);
        Attr1 const* attr1  = Meta::Attribute<T>::template query<Attr1>(memberIndex);
        std::string name = (NULL != attr1) ? attr1->s : n.get();
		return name;
	}

    std::string             m_s;
private:	
	template<typename MT>
	struct PrintValue
	{
		static void treatData(MT const&, std::string const& s, Meta::StructuredTypeTag)
		{
		    std::cout << "struct " << s <<
		        " (" << typeid(MT).name() << "):" << std::endl;
		}

		static void treatData(MT const& m, std::string const& s, Meta::SimpleTypeTag)
		{
		    std::cout << s << " = " << m <<
		        " (" << typeid(MT).name() << ")" << std::endl;
		}
	};
};

class Treater4Data
{
public:
	explicit Treater4Data(std::string const& s = "") : m_s(s)
    {}

    template<typename T, typename M, typename Par1>
    void operator() (Meta::MethodTypeTag const&, T& data, unsigned memberIndex, M (T::*pM)(Par1))
    {			
		char const par1 = 1;
		
		M res = (data.*pM)(par1);

        std::cout << "method : " << m_s << "::" << getMemberName<T>(memberIndex) << "(" <<  int(par1) << ") = " << res << std::endl;
    }

    template<typename T, typename M>
    Treater4Data const operator() (T const&, unsigned memberIndex, M (T::* /*pM*/))
    {
		return Treater4Data(m_s + "." + getMemberName<T>(memberIndex));
    }
    template<typename T, typename M, unsigned N>
    Treater4Data const operator() (T const&, unsigned memberIndex, M (T::* /*pM*/)[N], unsigned i)
    {
		return Treater4Data(m_s + "." + getMemberName<T>(memberIndex) +  "[" + toStr(i) + "]");
    }
private:
	template<typename T>
	std::string getMemberName(unsigned memberIndex) const
	{
		Meta::Name const& n = Meta::Attribute<T>::template get<Meta::Name>(memberIndex);
        Attr1 const* attr1  = Meta::Attribute<T>::template query<Attr1>(memberIndex);
        std::string name = (NULL != attr1) ? attr1->s : n.get();
		return name;
	}

    std::string             m_s;
};

template<typename T>
struct Data2Str_Simple_Type_Helper
{	
	static bool conv(std::string&  , char      , T const&     , Meta::StructuredTypeTag) 
	{
		return false;
	}

	static bool conv(std::string& s, char delim, T const& data, Meta::SimpleTypeTag)
	{
		std::string sVal = toStr(data);
		if(!s.empty())
			s += delim;
		s += sVal;
		return true;
	}	
};

class Data2Str
{
public:
	explicit Data2Str(char delim, std::string& s) : m_delim(delim), m_s(&s)
    {}

    template<typename T, typename M>
    Data2Str const operator() (T const& t, unsigned memberIndex, M (T::*pM))
    {
		typedef typename TypeTraits<M>::TypeKind								TK;

		memberIndex; // @info: to prevent compiler warning

		Data2Str_Simple_Type_Helper<M>::conv(*m_s, m_delim, (t.*pM), TK());
		return Data2Str(m_delim, *m_s);
    }
    template<typename T, typename M, unsigned N>
    Data2Str const operator() (T const& t, unsigned memberIndex, M (T::* pM)[N], unsigned i)
    {
		typedef typename TypeTraits<M>::TypeKind								TK;

		memberIndex; // @info: to prevent compiler warning
		
		Data2Str_Simple_Type_Helper<M>::conv(*m_s, m_delim, (t.*pM)[i], TK());

		return Data2Str(m_delim, *m_s);
    }

	Data2Str const& operator() (
		Meta::AnyDummy const&, 
		Meta::AnyDummy const& = Meta::AnyDummy(), 
		Meta::AnyDummy const& = Meta::AnyDummy(), 
		Meta::AnyDummy const& = Meta::AnyDummy()) 
	{ 
		return *this; 
	}	

private:
	char					m_delim;
    std::string*            m_s;
};

template<typename T>
struct Data2Str_Struct_Helper
{	
	static bool conv(std::string& s, char delim, T const& t, Meta::StructuredTypeTag) 
	{
		Data2Str d2s(delim, s);
		Meta::TreaterFrame<T const, Data2Str> treater(t, d2s);
		Meta::treatData<T>(treater);
		return true;
	}

	static bool conv(std::string&  , char      , T const&  , Meta::SimpleTypeTag)
	{
		return false;
	}	
};

template<typename T>
std::string data2Str(T const& t, char delim)
{
    typedef typename TypeTraits<T>::TypeKind								TK;
	std::string s;
	if(Data2Str_Simple_Type_Helper<T>::conv(s, delim, t, TK()))
		return s;

	if(!Data2Str_Struct_Helper<T>::conv(s, delim, t, TK()))
		throw std::runtime_error("very strange: !Data2Str_Struct_Helper<T>::conv(s, delim, t, TK()) - type T should be not simple!");

	return s;
}


template<typename T>
struct Str2Data_Simple_Type_Helper
{	
	enum ConvertResult { CR_FAIL, CR_CANNOT_CONVERT_STRUCT, CR_OK };

	static ConvertResult conv(std::string const&  , unsigned       , char      , T&     , Meta::StructuredTypeTag) 
	{
		return CR_CANNOT_CONVERT_STRUCT;
	}

	static ConvertResult conv(std::string const& s, unsigned& strPos, char delim, T& data, Meta::SimpleTypeTag)
	{
		std::string::size_type p = s.find_first_of(delim, strPos);
		if(p == std::string::npos)
			p = s.size();

		std::string sVal = s.substr(strPos, p - strPos);	

		if(p < s.size())
			++p;	
		strPos = p;

		return fromStr(sVal, &data) ? CR_OK : CR_FAIL;
	}	
};

class Str2Data
{
public:
	explicit Str2Data(std::string const& s, char delim, unsigned& strPos, bool&	success) : 
		m_s(s), m_delim(delim), m_strPos(&strPos), m_success(&success)
    {}

    template<typename T, typename M>
    Meta::UsedT<Str2Data> operator() (T& t, unsigned memberIndex, M (T::*pM))
    {
        typedef typename TypeTraits<M>::TypeKind								TK;

		memberIndex; // @info: to prevent compiler warning

		if(Str2Data_Simple_Type_Helper<M>::conv(m_s, *m_strPos, m_delim, (t.*pM), TK()) == Str2Data_Simple_Type_Helper<M>::CR_FAIL)
			*m_success = false;

		return Meta::make_used_t(*m_success, Str2Data(m_s, m_delim, *m_strPos, *m_success));
    }
    template<typename T, typename M, unsigned N>
    Meta::UsedT<Str2Data> operator() (T& t, unsigned memberIndex, M (T::* pM)[N], unsigned i)
    {
        typedef typename TypeTraits<M>::TypeKind								TK;

		memberIndex; // @info: to prevent compiler warning

		if(Str2Data_Simple_Type_Helper<M>::conv(m_s, *m_strPos, m_delim, (t.*pM)[i], TK()) == Str2Data_Simple_Type_Helper::CR_FAIL)
			*m_success = false;

		return Meta::make_used_t(*m_success, Str2Data(m_s, m_delim, *m_strPos, *m_success));
    }

	Str2Data const& operator() (
		Meta::AnyDummy const&, 
		Meta::AnyDummy const& = Meta::AnyDummy(), 
		Meta::AnyDummy const& = Meta::AnyDummy(), 
		Meta::AnyDummy const& = Meta::AnyDummy()) 
	{ 
		return *this; 
	}

private:
    std::string             m_s;
	char					m_delim;
	unsigned*				m_strPos;
	bool*					m_success;
};

template<typename T>
struct Str2Data_Struct_Helper
{
	static bool conv(std::string const&  s, unsigned& strPos, char delim, T&     t, Meta::StructuredTypeTag) 
	{
		bool		success		= true;
		Str2Data s2d(s, delim, strPos, success);
		Meta::TreaterFrame<T, Str2Data> treater(t, s2d);
		Meta::treatData<T>(treater);
		return success;
	}

	static bool conv(std::string const&   , unsigned&       , char      ,  T&     , Meta::SimpleTypeTag)
	{
		return false;
	}	
};

template<typename T>
bool str2Data(std::string const& s, unsigned& strPos, char delim, T& t)
{
    typedef typename TypeTraits<T>::TypeKind								TK;

	typename Str2Data_Simple_Type_Helper<T>::ConvertResult const convRes = Str2Data_Simple_Type_Helper<T>::conv(s, strPos, delim, t, TK());

	if(convRes == Str2Data_Simple_Type_Helper<T>::CR_FAIL)
		return false;
	else if(convRes == Str2Data_Simple_Type_Helper<T>::CR_OK)
		return true;

	return Str2Data_Struct_Helper<T>::conv(s, strPos, delim, t, TK());
}


template<typename T>
bool str2Data(std::string const& s, char delim, T& t)
{
	unsigned strPos = 0;
	bool success = str2Data(s, strPos, delim, t);

	if(success)
		success = (strPos == s.size());

	return success;
}

typedef std::vector<std::string> StrList;

class ExecMethod
{
public:
    static char const DELIM_FULL_NAME   = ':';
    static char const DELIM_MEMBER      = '.';
    static char const DELIM_ARG         = '\n';

	explicit ExecMethod(StrList const& a_name, std::string const& args) : ma_name(a_name), m_args(args)
    {}

    template<typename T, typename M, typename Par1>
    void operator() (Meta::MethodTypeTag const&, T& data, unsigned memberIndex, M (T::*pM)(Par1))
    {			
        std::string const name = getMemberName<T>(memberIndex);
        if(ma_name.size() != 1)
            return;
        if(ma_name[0] != name)
            return;		
                    
        typedef typename PureType<Par1>::Result									Par1_Pure;

        Par1_Pure par1;
		if(!str2Data(m_args, DELIM_ARG, par1))
			throw std::runtime_error("!str2Data(m_args, DELIM_ARG, par1");
        
		(data.*pM)(par1);

        std::cout << "ExecMethod : " << getMemberName<T>(memberIndex) << "(" <<  data2Str(par1, ',') << ")" << std::endl;
    }

    template<typename T, typename M, typename Par1, typename Par2>
    void operator() (Meta::MethodTypeTag const&, T& data, unsigned memberIndex, M (T::*pM)(Par1, Par2))
    {			
        std::string const name = getMemberName<T>(memberIndex);
        if(ma_name.size() != 1)
            return;
        if(ma_name[0] != name)
            return;		
                    
        typedef typename PureType<Par1>::Result									Par1_Pure;
        typedef typename PureType<Par2>::Result									Par2_Pure;

		unsigned strPos = 0;
        Par1_Pure par1;
		if(!str2Data(m_args, strPos, DELIM_ARG, par1))
			throw std::runtime_error("!str2Data(m_args, DELIM_ARG, par1");
        Par2_Pure par2;
		if(!str2Data(m_args, strPos, DELIM_ARG, par2))
			throw std::runtime_error("!str2Data(m_args, DELIM_ARG, par2");
		if(strPos != m_args.size())
			throw std::runtime_error("strPos != m_args.size()");
        
		(data.*pM)(par1, par2);

        std::cout << "ExecMethod : " << getMemberName<T>(memberIndex) << "(" << data2Str(par1, ',') << " ; " <<   data2Str(par2, ',')  << ")" << std::endl;
    }

    template<typename T, typename M, typename Par1, typename Par2, typename Par3>
    void operator() (Meta::MethodTypeTag const&, T& data, unsigned memberIndex, M (T::*pM)(Par1, Par2, Par3))
    {			
        std::string const name = getMemberName<T>(memberIndex);
        if(ma_name.size() != 1)
            return;
        if(ma_name[0] != name)
            return;		
                    
		unsigned strPos = 0;
		
        typename PureType<Par1>::Result 	par1;
		if(!str2Data(m_args, strPos, DELIM_ARG, par1))
			throw std::runtime_error("!str2Data(m_args, DELIM_ARG, par1");

        typename PureType<Par2>::Result 	par2;
		if(!str2Data(m_args, strPos, DELIM_ARG, par2))
			throw std::runtime_error("!str2Data(m_args, DELIM_ARG, par2");

        typename PureType<Par3>::Result par3;
		if(!str2Data(m_args, strPos, DELIM_ARG, par3))
			throw std::runtime_error("!str2Data(m_args, DELIM_ARG, par3");

		if(strPos != m_args.size())
			throw std::runtime_error("strPos != m_args.size()");
        
		(data.*pM)(par1, par2, par3);

        std::cout << "ExecMethod : " << getMemberName<T>(memberIndex) << "(" << 
			data2Str(par1, ',') << " ; " <<
			data2Str(par2, ',') << " ; " <<   
			data2Str(par3, ',')  << ")" << std::endl;
    }
	

    template<typename T, typename M>
    Meta::UsedT<ExecMethod> operator() (T const&, unsigned memberIndex, M (T::* /*pM*/))
    {
		if(ma_name.empty())
			return dummy();
		
        std::string const name = getMemberName<T>(memberIndex);

		if(ma_name[0] != name)
			return dummy();

		StrList a_name1(ma_name.begin() + 1, ma_name.end());

		return ExecMethod(a_name1, m_args);
    }
    template<typename T, typename M, unsigned N>
    Meta::UsedT<ExecMethod> operator() (T const&, unsigned memberIndex, M (T::* /*pM*/)[N], unsigned i)
    {
		if(ma_name.empty())
			return dummy();
		
        std::string const name = getMemberName<T>(memberIndex, i);

		if(ma_name[0] != name)
			return dummy();

		StrList a_name1(ma_name.begin() + 1, ma_name.end());

		return ExecMethod(a_name1, m_args);
    }
private:
	template<typename T>
	static std::string getMemberName(unsigned memberIndex, int indexInArray = -1)
	{
		std::string name = Meta::Attribute<T>::template get<Meta::Name>(memberIndex).get();
        if(indexInArray >= 0)        
            name = name + "[" + toStr(indexInArray) + "]";
        return name;
	}

	static Meta::UsedT<ExecMethod> dummy() { return Meta::make_used_t(false, ExecMethod(StrList(), std::string())); }
    
private:
    StrList             ma_name;
    std::string         m_args;
    //std::string             m_cmd;
};


class Summator
{
public:
	Summator(double& res) : m_sum(&res)
	{}

	template<typename T, typename M>
	Summator const& operator() (T const& t, unsigned /*memberIndex*/, M (T::*pM))
	{
		treat(t.*pM);
		return *this;
	}
	template<typename T, typename M, unsigned N>
	Summator const& operator() (T const& t, unsigned /*memberIndex*/, M (T::*pM)[N], unsigned i)
	{
		M const& m = (t.*pM)[i];
		treat(m);
		return *this;
	}
			
	Summator const& operator() (
		Meta::AnyDummy const&, 
		Meta::AnyDummy const& = Meta::AnyDummy(), 
		Meta::AnyDummy const& = Meta::AnyDummy(), 
		Meta::AnyDummy const& = Meta::AnyDummy()) 
	{ 
		return *this; 
	}	
	
	void treat(double d) { *m_sum += d; }
	void treat(Meta::AnyDummy const&) {}
	
private:
	double* m_sum;
};

template<typename T>
class SumMembers
{
public:
	explicit SumMembers(T const& data, double& res) :
        m_data(data),
        m_res(res)
    {
    }

	void operator()(...)	{}

    template<typename M, typename Par1>
    void operator() (unsigned /*memberIndex*/, M (T::* /*pM*/)(Par1))
    {
    }

    template<typename M>
    void operator() (unsigned /*memberIndex*/, M (T::*pM))
    {
        typedef typename TypeTraits<M>::TypeKind TK;

        treatData(TK(), m_res, m_data.*pM);		
    }
    template<typename M, unsigned N>
    void operator() (unsigned /*memberIndex*/, M (T::*pM)[N])
    {        
        typedef typename TypeTraits<M>::TypeKind TK;

        for(unsigned i = 0; i < N; ++i)
        {
			treatData(TK(), m_res, (m_data.*pM)[i]);
        }
    }
	template<typename BaseClassT>
	void operator() (unsigned /*memberIndex*/, Meta::BaseClass<T, BaseClassT> const& /*baseClass*/)
	{
		SumMembers<BaseClassT> sumMembers(m_data, m_res);
		
		Meta::treatData<BaseClassT>(sumMembers);
	}
private:
    /// non-copyable
    explicit SumMembers(SumMembers const&);
    SumMembers& operator= (SumMembers const&);

    T const&				m_data;
    double&                 m_res;
private:
    typedef T       T_BcbWa;    /// bcb workaround

	static void treatData(Meta::SimpleTypeTag const&, double& res, int 		n) { res += n; }
	static void treatData(Meta::SimpleTypeTag const&, double& res, double	d) { res += d; }
	static void treatData(Meta::SimpleTypeTag const&, double&    , Meta::AnyDummy const&) {}
	
	template<typename MT>
	static void treatData(Meta::StructuredTypeTag const&, double& res, MT const& m)
	{
		SumMembers<MT> treater(m, res);
		Meta::treatData<MT>(treater);		
	}	
};


class ICommandTreater
{
public:
	enum TreaterResult { TR_FAILED, TR_OK, TR_ID_NOT_FIT };

    virtual TreaterResult treatCommand(std::string const& dataId, std::string const& treaterId, std::string const& cmd) = 0;

    virtual ~ICommandTreater() {}
private:
};

template<typename T>
class CommandTreaterT : public ICommandTreater
{
public:
	explicit CommandTreaterT(T* data, std::string const& dataId) : 
		m_data(data), m_dataId(dataId)
	{
		if(data == NULL)
			throw std::runtime_error("data == NULL");
	}

    virtual TreaterResult treatCommand(std::string const& dataId, std::string const& treaterId, std::string const& cmd)
    {
		if(dataId != m_dataId)
			return TR_ID_NOT_FIT;

		if(treaterId == "ExecMethod")
		{
			std::string names;
			std::string args;
        
			if(!getStrParts(cmd, ExecMethod::DELIM_FULL_NAME, &names, &args, false))
				throw std::runtime_error("!LibSystem::getStrParts");
        
			if(names.empty() || args.empty())
				throw std::runtime_error("names.empty() || args.empty()");
        
			StrList a_name;
			parseStr(names, std::string(1, ExecMethod::DELIM_MEMBER), std::back_inserter(a_name));

			Meta::TreaterFrame<T, ExecMethod>  execMethod(*m_data, ExecMethod(a_name, args));

			Meta::treatData<T>(execMethod);

			return ICommandTreater::TR_OK;
		}

		return ICommandTreater::TR_FAILED;
    }

    virtual ~CommandTreaterT() {}
private:
	T*				m_data;
	std::string		m_dataId;
};

class CommandTreaterMan
{
public:
	template<typename T>
	void addData(T* t, std::string const& dataId)
	{
		ICommandTreater* ct = new CommandTreaterT<T>(t, dataId);
		ma_commandTreater.push_back(ct);
	}

    bool treatCommand(std::string const& dataId, std::string const& treaterId, std::string const& cmd)
	{
		for(unsigned i = 0; i < ma_commandTreater.size(); ++i)
		{
			ICommandTreater::TreaterResult tr = ma_commandTreater[i]->treatCommand(dataId, treaterId, cmd);
			if(tr == ICommandTreater::TR_ID_NOT_FIT)
				continue;

			return (tr == ICommandTreater::TR_OK);
		}
		return false;
	}

	~CommandTreaterMan()
	{
		for(unsigned i = 0; i < ma_commandTreater.size(); ++i)
		{
			delete ma_commandTreater[i];
		}
	}
private:
	std::vector<ICommandTreater*>	ma_commandTreater;
};


#endif // PrintData_177c62be401710ca0560_H
