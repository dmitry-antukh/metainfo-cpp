#ifndef Metadata_090208_H
#define Metadata_090208_H

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
    	template<typename AddMember, typename AttrList>								    \
	    static void defineMetaInfo(AddMember& addMember, AttrList& attrList, TypeName*) \
	    {															                    \
		    typedef TypeName		TypeT;
#endif // !defined(METADATA)


#if defined(ATTRIBUTES)
#   error ATTRIBUTES already defined
#else // !defined(ATTRIBUTES)
#   define ATTRIBUTES  attrList
#endif // !defined(ATTRIBUTES)


#if defined(MEMBER)
#   error MEMBER already defined
#else // !defined(MEMBER)
#   define MEMBER(memberName)										\
	    addMember(&TypeT::memberName) << ::Meta::Name(#memberName)
#endif // !defined(MEMBER)


#if defined(END_METADATA)
#   error END_METADATA already defined
#else
#   define END_METADATA }
#endif



namespace Meta
{


template<typename, typename T>                           class MemberInfo;


namespace Detail
{


///////////////////////////////////////////////////////////////////////////////
//////////////////////////  Member info  //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template<typename TreaterType>
class IMember
{
public:
    //virtual void treat(TreaterType& treater, T* t) const = 0;
    virtual void treat(TreaterType& treater, unsigned index) const = 0;

    virtual ~IMember() {}
};

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

} // namespace Detail


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

template<typename T>
class Attribute
{
public:
	explicit Attribute() {}

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
		Attr const* attr = Impl<Attr>::inst().a_attr;
		
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
	struct Impl
	{
		typedef std::vector<Attr*> AttrList; 
		
#if defined(__BORLANDC__)
		static Impl& inst() 
#else // !defined(__BORLANDC__)
        static Impl<Attr>& inst() 
#endif // !defined(__BORLANDC__)
		{
			static Impl<Attr> s_inst;
			return s_inst;
		}

		explicit Impl() : m_attr(NULL) {}

		~Impl()
		{
			delete m_attr;

			for(unsigned i = 0; i < ma_attr.size(); ++i)
				delete ma_attr[i];
		}
		
		void add(Attr const& attr, unsigned index)
		{
			META_LIB_ASSERT(index >= ma_attr.size(), "attribute already defined");
			
			ma_attr.resize(index + 1, NULL);
			ma_attr[index] = new Attr(attr);

			//std::cout << "type=" << typeid(T).name() << " attribute=" << typeid(Attr).name() <<
				//" set for index=" << index << std::endl;
		}
		
        void setAttribute(Attr const& attr)
        {
			META_LIB_ASSERT(NULL == m_attr, "attribute already defined");
			
            m_attr = new Attr(attr);
        }

		Attr*		m_attr;
		AttrList 	ma_attr;
	};	
private:	
	///// private copyable
	//Attribute(Attribute<T> const&) {}
	//Attribute<T>& operator= (Attribute<T> const&) {}
		
	static bool& inited()
	{
		static bool s_inited = false;
		
		return s_inited;
	}
		
private:
	template<typename, typename> friend class MemberList;
	template<typename, typename> friend class AttrList;
};

template<typename TreaterType, typename T>
class AttrList
{
public:
	template<typename Attr>
	AttrList& operator << (Attr const& attr)
	{
		typedef typename Attribute<T>::template Impl<Attr>  AttrImpl;

		if(!Attribute<T>::inited())
			AttrImpl::inst().setAttribute(attr);

		return *this;
	}
private:
    explicit AttrList() {}
	
	~AttrList() { Attribute<T>::inited() =  true; }

    template<typename, typename> friend class MemberInfo;
};


template<typename TreaterType, typename T>
class MemberList
{
public:
    template<typename PMemberType>
    MemberList& operator() (PMemberType pMember)
    {
        Detail::IMember<TreaterType>* mt =
            new Detail::Member<TreaterType, PMemberType>(pMember);

        ma_member.push_back(mt);

		return *this;
    }

	template<typename Attr>
	MemberList& operator << (Attr const& attr)
	{
		typedef typename Attribute<T>::template Impl<Attr>  AttrImpl;
		//AttrImpl::AttrList& attrImpl = AttrImpl::inst(); /// stupid gcc don't understand it

		META_LIB_ASSERT(!ma_member.empty(), "cannot add attribute if there's no members");

		if(!Attribute<T>::inited())
			AttrImpl::inst().add(attr, ma_member.size() - 1);

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

	
    std::vector< Detail::IMember<TreaterType>* >      ma_member;

    template<typename, typename> friend class MemberInfo;
};

/// initMemberList helper
template<typename AddMember, typename AttrList, typename T>
void defineMetaInfo(AddMember& addMember, AttrList& attrList, T*)
{
    T::defineMetaInfo(addMember, attrList, static_cast<T*>(0));
}

template<typename AddMember, typename AttrList, typename T>
void defineMetaInfo(AddMember& addMember, AttrList& attrList, T const*)
{
    T::defineMetaInfo(addMember, attrList, static_cast<T*>(0));
}

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
        AttrList<TreaterType, T> attrList;
        defineMetaInfo(m_memberList, attrList, static_cast<T*>(NULL));
    }
    //noncopyable
    MemberInfo(MemberInfo<TreaterType, T> const&);
    MemberInfo<TreaterType, T>& operator = (MemberInfo<TreaterType, T> const&);
    
    MemberList<TreaterType, T>        m_memberList;
};



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


#endif
