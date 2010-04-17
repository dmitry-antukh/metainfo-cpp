#ifndef Metadata_060909_H
#define Metadata_060909_H

#include <assert.h>
#include <typeinfo>
#include <vector>
#include <string>


#if !defined(META_LIB_ASSERT)
/// you must define META_LIB_ASSERT(expr, errorDescription), for example 
/// #define META_LIB_ASSERT(expr, errorDescription) assert(expr)
#	error META_LIB_ASSERT(expr, errorDescription) not defined 
#endif // !defined(META_LIB_ASSERT)


#if defined(METADATA)
#   error METADATA already defined
#else // !defined(METADATA)
#   define METADATA(TypeName) 										                    \
    	template<typename TreaterType, typename AttrTreater>								    \
	    static void defineMetaInfo(TreaterType& treater, AttrTreater const& attrTreater, TypeName*) \
	    {															                    \
		    typedef TypeName		TypeT;
#endif // !defined(METADATA)


#if defined(ATTRIBUTES)
#   error ATTRIBUTES already defined
#else // !defined(ATTRIBUTES)
#   define ATTRIBUTES  attrTreater
#endif // !defined(ATTRIBUTES)


#if defined(MEMBER)
#   error MEMBER already defined
#else // !defined(MEMBER)
#   define MEMBER(memberName)										\
	    treater(&TypeT::memberName) << ::Meta::Name(#memberName)
#endif // !defined(MEMBER)


#if defined(END_METADATA)
#   error END_METADATA already defined
#else
#   define END_METADATA }
#endif


#if defined(BASE_CLASS)
#   error BASE_CLASS already defined
#else // !defined(BASE_CLASS)
#   define BASE_CLASS(BaseClassName) 										    \
		typedef BaseClassName 				BaseClassName_BccWA;				\
        treater(Meta::BaseClass<TypeT, BaseClassName_BccWA>())
#endif // !defined(BASE_CLASS)



namespace Meta
{

///////////////////////////////////////////////////////////////////////////////
//////////////////////////  Member info  //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template<typename, typename T>                           class MemberInfo;

/*template<typename TreaterType>
class IMember
{
public:
    //virtual void treat(TreaterType& treater, T* t) const = 0;
    virtual void treat(TreaterType& treater, unsigned index) const = 0;

    virtual ~IMember() {}
};*/

namespace Detail
{

/*
template<typename TreaterType, typename PMemberType>
class Member : public IMember<TreaterType>
{
public:
	explicit Member(PMemberType pmember) : m_pMember(pmember)
	{
		//T* p = NULL;
		/// check; //(p->*pmember);
	}

	virtual void treat(TreaterType& treater, unsigned index) const
	{
		treater(m_pMember, index);
	}
private:
	PMemberType m_pMember;
};
 */


class DummyReceiver
{
public:
	template<typename T>
	DummyReceiver const& operator << (T const& t) const
	{
        #if defined(__BORLANDC__)
		t; /// to prevent bcc-warning
		#endif // defined(__BORLANDC__)
		
		return *this;
	}	
};


template<typename TreaterType>
class TreaterHelper : public DummyReceiver
{
public:
	explicit TreaterHelper(TreaterType& treater) : 
		m_treater(treater), m_index(0)
	{}

    template<typename PMemberType>
    TreaterHelper<TreaterType>& operator() (PMemberType pMember)
    {
        m_treater(pMember, m_index++);

		return *this;
    }
	
	/*
	template<typename T>
	TreaterHelper<TreaterType>& operator << (T const& t) const
	{
        #if defined(__BORLANDC__)
		t; /// to prevent bcc-warning
		#endif // defined(__BORLANDC__)
		
		return *this;
	}
	*/	
private:
	TreaterType& 	m_treater;
    unsigned		m_index;
};


} // namespace Detail


template<typename T, typename TreaterType>
void treatData(TreaterType& treater)
{
	Detail::TreaterHelper<TreaterType> helper(treater);
	
	Detail::DummyReceiver attrTreater;
	
	defineMetaInfo(helper, attrTreater, static_cast<T*>(NULL));
}


template<typename ClassT, typename BaseClassT>
class BaseClass
{
public:
	BaseClassT& operator()(ClassT& obj) const
	{ 
		BaseClassT* baseObj = &obj;
		return *baseObj;
	}
	BaseClassT const& operator()(ClassT const& obj) const
	{ 
		BaseClassT const* baseObj = &obj;
		return *baseObj;
	}
};


struct NoAttr
{
    NoAttr() {}

    template<typename T> NoAttr(T) {}
};

class Name
{
public:
	explicit Name(std::string const& name) : m_name(name) {}
	
	std::string get() const { return m_name; }
private:
	std::string 	m_name;
};


template<typename, typename> class MemberList;


/// initMemberList helper
template<typename AddMember, typename AttrList, typename T>
void defineMetaInfo(AddMember& addMember, AttrList const& attrList, T*)
{
    T::defineMetaInfo(addMember, attrList, static_cast<T*>(0));
}

template<typename AddMember, typename AttrList, typename T>
void defineMetaInfo(AddMember& addMember, AttrList const& attrList, T const*)
{
    defineMetaInfo(addMember, attrList, static_cast<T*>(0));
}


template<typename T>
class Attribute
{
public:
	template<typename Attr>
	static Attr const& get()
	{
		Attr const* attr = Impl<Attr>::inst().m_attr;

		META_LIB_ASSERT(NULL != attr, "attribute not defined");

		return *attr;
	}

	template<typename Attr>
	static Attr const* query()
	{
		Attr const* attr = Impl<Attr>::inst().m_attr;
		
		return attr;
	}

	template<typename Attr>
	static Attr const& get(unsigned index)
	{
		typename Impl<Attr>::AttrList const&  a_attr = Impl<Attr>::inst().ma_attr;

		META_LIB_ASSERT(index < a_attr.size() && NULL != a_attr[index], "attribute not defined");

		return *a_attr[index];
	}

	template<typename Attr>
	static Attr const* query(unsigned index)
	{
		typename Impl<Attr>::AttrList const&  a_attr = Impl<Attr>::inst().ma_attr;
		
		return (index < a_attr.size()) ? a_attr[index] : NULL;
	}
private:
    
	template<typename Attr>
	class Impl
	{
    public:
        typedef Attr                        Attr_BccWA;
		typedef std::vector<Attr_BccWA*>    AttrList; 
		
#if defined(__BORLANDC__)
		static Impl& inst() 
#else // !defined(__BORLANDC__)
        static Impl<Attr_BccWA>& inst() 
#endif // !defined(__BORLANDC__)
		{
			static Impl<Attr_BccWA> s_inst;
			
            if(!Attribute<T>::m_inited)
            {
                Attribute<T>::m_inited = true;
                Attribute<T>::init();
            }
			
			return s_inst;
		}

		explicit Impl() : m_attr(NULL)
        {
        }

		~Impl()
		{
			delete m_attr;

			for(unsigned i = 0; i < ma_attr.size(); ++i)
				delete ma_attr[i];
		}
		
		void add(Attr_BccWA const& attr, unsigned index)
		{
			META_LIB_ASSERT(index >= ma_attr.size(), "attribute already defined");
			
			ma_attr.resize(index + 1, NULL);
			ma_attr[index] = new Attr_BccWA(attr);

			//std::cout << "type=" << typeid(T).name() << " attribute=" << typeid(Attr).name() <<
				//" set for index=" << index << std::endl;
		}
		
        void setAttribute(Attr_BccWA const& attr)
        {
			META_LIB_ASSERT(NULL == m_attr, "attribute already defined");
			
            m_attr = new Attr_BccWA(attr);
        }

		Attr_BccWA*	m_attr;
		AttrList 	ma_attr;
        bool        m_inited;
	};
        
    class AttrListHelper
    {
    public:
	    template<typename Attr>
	    AttrListHelper const& operator << (Attr const& attr) const
	    {
		    typedef typename Attribute<T>::template Impl<Attr>  AttrImpl;

		    AttrImpl::inst().setAttribute(attr);

		    return *this;
	    }
    };

    class MemberListHelper
    {
    public:
        MemberListHelper() : m_memberIndex(-1) {}

        template<typename PMemberType>
        MemberListHelper& operator() (PMemberType pMember)
        {
			#if defined(__BORLANDC__)
            pMember; /// to prevent bcc-warning
			#endif // defined(__BORLANDC__)
			
            ++m_memberIndex;
			
		    return *this;
        }

	    template<typename Attr>
	    MemberListHelper& operator << (Attr const& attr)
	    {
		    typedef typename Attribute<T>::template Impl<Attr>  AttrImpl;

		    META_LIB_ASSERT(m_memberIndex >= 0, "cannot add attribute if there's no members");

		    AttrImpl::inst().add(attr, m_memberIndex);

		    return *this;
	    }
    private:
        int      m_memberIndex;
    };

    
    inline static void init()
    {
        MemberListHelper    memberListHelper;
        AttrListHelper      attrListHelper;

        defineMetaInfo(memberListHelper, attrListHelper, static_cast<T*>(NULL));
    }


private:	
	///// private copyable
	//Attribute(Attribute<T> const&) {}
	//Attribute<T>& operator= (Attribute<T> const&) {}
		
    static bool m_inited;
private:
	template<typename, typename> friend class MemberList;
	template<typename, typename> friend class AttrList;
    template<typename> friend class Impl;
};

template<typename T>
bool Attribute<T>::m_inited = false;


/*
template<typename TreaterType, typename T>
class MemberList
{
public:
    template<typename PMemberType>
    MemberList<TreaterType, T>& operator() (PMemberType pMember)
    {
        IMember<TreaterType>* mt =
            new Detail::Member<TreaterType, PMemberType>(pMember);

        ma_member.push_back(mt);

		return *this;
    }

	template<typename Attr>
	MemberList<TreaterType, T>& operator << (Attr const& attr)
	{
        #if defined(__BORLANDC__)
		attr; /// to prevent bcc-warning
		#endif // defined(__BORLANDC__)
		
		return *this;
	}
private:
    explicit MemberList() {}

    ~MemberList() 
    {
        for(unsigned i=0; i<ma_member.size(); ++i)
        {
            delete ma_member[i];
        }
    }

    std::vector< IMember<TreaterType>* >      ma_member;

    friend class MemberInfo<TreaterType, T>;
};

template<typename TreaterType, typename T>
class MemberInfo //: boost::noncopyable
{
public:
    static MemberInfo<TreaterType, T>& inst()
    {
        static MemberInfo<TreaterType, T> s_inst;

        return s_inst;
    }

    void treatData(TreaterType& treater) const
    {
        for(unsigned i = 0; i < m_memberList.ma_member.size(); ++i)
        {
            m_memberList.ma_member[i]->treat(treater, i);
        }
    }
    
private:
    MemberInfo() 
    {
		Detail::DummyReceiver 	attrTreater;
        defineMetaInfo(m_memberList, attrTreater, static_cast<T*>(NULL));
    }
    //noncopyable
    MemberInfo(MemberInfo<TreaterType, T> const&);
    MemberInfo<TreaterType, T>& operator = (MemberInfo<TreaterType, T> const&);
    
    MemberList<TreaterType, T>        m_memberList;
};
*/



//template<typename Attr, typename TreaterType, typename T>
//inline void treatData(live_ptr<TreaterType> treater, T* data)
//{
//    MemberInfo<T, Attr>::inst().treatData(treater, data);
//
//    //std::cout << "treatData: treater type=" << typeid(TreaterType).name() 
//      //        << "           data type=" << typeid(T).name() << std::endl << std::endl;
//}
//
//template<typename Attr, typename TreaterType, typename T>
//inline void treatData(live_ptr<TreaterType> treater, T const& data)
//{
//    MemberInfo<T, Attr>::inst().treatData(treater, data);
//
//    //std::cout << "treatData: treater type=" << typeid(TreaterType).name() 
//    //        << "           data type=" << typeid(T).name() << std::endl << std::endl;
//}

} // namespace Meta


#endif // Metadata_060909_H
