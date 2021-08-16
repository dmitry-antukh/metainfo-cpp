#ifndef Metadata_f4c2a193c01aeba365ae_H
#define Metadata_f4c2a193c01aeba365ae_H

#include <vector>
#include <string>
//#include <typeinfo> /// @info: for debug


#if !defined(META_LIB_ASSERT)
/// you must define META_LIB_ASSERT(expr, errorDescription), for example
/// #define META_LIB_ASSERT(expr, errorDescription) assert(expr)
#	error META_LIB_ASSERT(expr, errorDescription) not defined
#endif // !defined(META_LIB_ASSERT)


#if defined(METADATA)
#   error METADATA already defined
#else // !defined(METADATA)
#   define METADATA(TypeName) 										                                 \
    	template<typename TreaterType, typename AttrTreater>								         \
	    inline static void processMetaData(TreaterType& treater, AttrTreater const* attrTreater, TypeName*) \
	    {															                                 \
            typedef TypeName        TypeT_For_Metadata;                                              \
            int memberMetaIndex_impl_ = -1;     memberMetaIndex_impl_;
#endif // !defined(METADATA)                    ^ may be unused


#if defined(ATTRIBUTES)
#   error ATTRIBUTES already defined
#else // !defined(ATTRIBUTES)
#   define ATTRIBUTES  if(NULL != attrTreater) *attrTreater
#endif // !defined(ATTRIBUTES)


#if defined(MEMBER)
#   error MEMBER already defined
#else // !defined(MEMBER)
#   define MEMBER(memberName)	                                                                        \
        if(NULL == attrTreater) treater(++memberMetaIndex_impl_, &TypeT_For_Metadata::memberName);      \
        else                    (*attrTreater)(::Meta::Name(#memberName, ::Meta::Name::OT_MEMBER), ++memberMetaIndex_impl_)
#endif // !defined(MEMBER)


#if defined(END_METADATA)
#   error END_METADATA already defined
#else
#   define END_METADATA }
#endif


#if defined(BASE_CLASS)
#   error BASE_CLASS already defined
#else // !defined(BASE_CLASS)
#   define BASE_CLASS(BaseClassName) 										                \
        if(NULL == attrTreater) treater(++memberMetaIndex_impl_, Meta::BaseClass<TypeT_For_Metadata, BaseClassName>());    \
        else                    (*attrTreater)(::Meta::Name(#BaseClassName, ::Meta::Name::OT_BASE_CLASS), ++memberMetaIndex_impl_)
#endif // !defined(BASE_CLASS)

#if defined(__BORLANDC__)
     #define INLINE inline
#else
     #define INLINE inline
#endif // !defined(__BORLANDC__)

namespace Meta
{


class Name
{
public:
    enum ObjectType { OT_MEMBER, OT_BASE_CLASS };

public:
    explicit Name(std::string const& name, ObjectType objectType) :
            m_name(name), m_objectType(objectType)
    {}

    std::string get() const { return m_name; }

    ObjectType  getObjectType() const { return m_objectType; }

private:
    std::string     m_name;
    ObjectType      m_objectType;
};


namespace Detail
{

class DummyReceiver
{
public:
    explicit DummyReceiver() {}

    DummyReceiver const& operator()(Name const&, unsigned) const { return *this; }

	template<typename T>
    INLINE void operator()(unsigned, T const&) const;

	template<typename T>
	INLINE DummyReceiver const& operator << (T const& t) const;
};

template<typename T>
INLINE void DummyReceiver::operator()(unsigned, T const&) const {}

template<typename T>
INLINE DummyReceiver const& DummyReceiver::operator << (T const& /* t */) const
{
    return *this;
}

struct TypeWithConstTag {};
struct TypeWithoutConstTag {};

template<typename C> struct TypeTraits_Helper;
template<typename C>
struct TypeTraits_Helper<C*>
{
    typedef C											TypeWithoutConst;
    typedef TypeWithoutConstTag							ConstTag;
};
template<typename C>
struct TypeTraits_Helper<C const*>
{
    typedef C											TypeWithoutConst;
    typedef TypeWithConstTag							ConstTag;
};

template<typename T>
struct TypeTraits
{
    typedef typename TypeTraits_Helper<T*>::TypeWithoutConst		TypeWithoutConst;
    typedef T const													TypeWithConst;
    typedef typename TypeTraits_Helper<T*>::ConstTag				ConstTag;
};


template<typename ConstTag, typename X>
struct ConstMaker;

template<typename X>
struct ConstMaker<TypeWithConstTag, X>
{
	typedef X const										Result;
};

template<typename X>
struct ConstMaker<TypeWithoutConstTag, X>
{
	typedef typename TypeTraits<X>::TypeWithoutConst	Result;
};


} // namespace Detail


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

class AttributeBase
{
protected:
    static INLINE unsigned getIndexOfName(std::vector<void*> const& a_name, Name const& name);
};

INLINE unsigned AttributeBase::getIndexOfName(std::vector<void*> const& a_name, Name const& name)
{
    unsigned res = 1; /// 0-index in attribute-array is reserved for attibutes-of-class
    for(; res < a_name.size(); ++res)
    {
		Name* n = static_cast<Name*>(a_name[res]);
		META_LIB_ASSERT(n != NULL, "n shoud be not NULL!");

        if(n->get() == name.get())
        {
            META_LIB_ASSERT(n->getObjectType() == name.getObjectType(),
                    std::string("object type different for:") + name.get());
            break;
        }
    }

    META_LIB_ASSERT(res < a_name.size(), std::string("member:") + name.get() + " is not registered");
    return (res - 1); /// we subsract 1 because index of member is less on one than index of attribute 
	                  /// (0-index in attribute-array is reserved for attibutes-of-class)
}

template<typename T>
class Attribute : public AttributeBase
{
public:
	template<typename Attr>
	static Attr const& get()
	{
		static typename Impl<Attr>::AttrList const&  a_attr = Impl<Attr>::inst().ma_attr;

		META_LIB_ASSERT(0 < a_attr.size() && NULL != a_attr[0], "attribute not defined");

		return *reinterpret_cast<Attr const*>(a_attr[0]);
	}

	template<typename Attr>
	static Attr const* query()
	{
		static typename Impl<Attr>::AttrList const&  a_attr = Impl<Attr>::inst().ma_attr;

		return (0 < a_attr.size()) ? reinterpret_cast<Attr const*>(a_attr[0]) : NULL;
	}

	template<typename Attr>
	static Attr const& get(unsigned index)
	{
		static typename Impl<Attr>::AttrList const&  a_attr = Impl<Attr>::inst().ma_attr;

        ++index;
		META_LIB_ASSERT(index < a_attr.size() && NULL != a_attr[index], "attribute not defined");

		return *reinterpret_cast<Attr const*>(a_attr[index]);
	}

	template<typename Attr>
	static Attr const* query(unsigned index)
	{
		static typename Impl<Attr>::AttrList const&  a_attr = Impl<Attr>::inst().ma_attr;

        ++index;
		return (index < a_attr.size()) ? reinterpret_cast<Attr const*>(a_attr[index]) : NULL;
	}

	template<typename AdditionalDefT>
	static void addAttributes(AdditionalDefT* dummy = NULL);
private:

	template<typename Attr>
	class Impl
	{
    public:
        typedef Attr                        Attr_BccWA;
		typedef std::vector<void*>          AttrList;

#if defined(__BORLANDC__)
		static Impl& inst();
#else // !defined(__BORLANDC__)
        static Impl<Attr>& inst();
#endif // !defined(__BORLANDC__)

		explicit Impl();
		~Impl();

		void setAttribute(Attr_BccWA const& attr, int index);

		AttrList 	ma_attr;
	};

    class AttrListHelper
    {
    public:
        explicit AttrListHelper(int memberIndex, bool inited);

        AttrListHelper operator()(Name const& name, unsigned memberIndex) const;

	    template<typename Attr>
	    AttrListHelper const& operator << (Attr const& attr) const;
    private:
        int     m_memberIndex;
        bool    m_inited;
    };

    template<typename AdditionalDefT>
    struct AdditionalInit
    {
    	static bool ms_inited;
    };

    static void init();

private:
	///// private copyable
	//Attribute(Attribute<T> const&) {}
	//Attribute<T>& operator= (Attribute<T> const&) {}

    static bool ms_inited;

private:
    template<typename> friend class Impl;
    friend class AttrListHelper;
};

template<typename T>
	template<typename Attr>
#if !defined(_MSC_VER)
typename Attribute<T>::template Impl<Attr>& Attribute<T>::Impl<Attr>::inst()
#else	// defined(_MSC_VER)
typename Attribute<T>::Impl<Attr>& Attribute<T>::Impl<Attr>::inst()
#endif	// defined(_MSC_VER)
{
	static Impl<Attr_BccWA> s_inst;

	if(!Attribute<T>::ms_inited)
	{
		Attribute<T>::init();
	}

	return s_inst;
}

template<typename T>
	template<typename Attr>
Attribute<T>::Impl<Attr>::Impl()
{
}

template<typename T>
	template<typename Attr>
Attribute<T>::Impl<Attr>::~Impl()
{
    for(unsigned i = 0; i < ma_attr.size(); ++i)
        delete reinterpret_cast<Attr const*>(ma_attr[i]);
}

template<typename T>
	template<typename Attr>
void Attribute<T>::Impl<Attr>::setAttribute(Attr_BccWA const& attr, int index)
{
    ++index;

    bool const attrNotDefined = (index >= int(ma_attr.size())) || (NULL == ma_attr[index]);
    META_LIB_ASSERT(attrNotDefined, "attribute already defined");

    if(int(ma_attr.size()) <= index)
        ma_attr.resize(index + 1, NULL);

    ma_attr[index] = new Attr_BccWA(attr);
}

template<typename T>
Attribute<T>::AttrListHelper::AttrListHelper(int memberIndex, bool inited) :
    m_memberIndex(memberIndex), m_inited(inited)
{
}

template<typename T>
typename Attribute<T>::AttrListHelper Attribute<T>::AttrListHelper::operator()(Name const& name, unsigned memberIndex) const
{
    typedef typename Attribute<T>::Impl<Name>	NameAttrImpl;

    unsigned const ind = m_inited ? AttributeBase::getIndexOfName(NameAttrImpl::inst().ma_attr, name) : memberIndex;

    AttrListHelper res(ind, m_inited);

    if(!m_inited)
    {
        res << name;
    }
    return res;
}

template<typename T>
    template<typename Attr>
typename Attribute<T>::AttrListHelper const& Attribute<T>::AttrListHelper::operator << (Attr const& attr) const
{
    typedef typename Attribute<T>::template Impl<Attr>  AttrImpl;

    META_LIB_ASSERT(m_memberIndex >= -1, "cannot add attribute if there's no members");

    AttrImpl::inst().setAttribute(attr, m_memberIndex);

    return *this;
}

template<typename T>
void Attribute<T>::init()
{
    if(ms_inited)
        return;

    ms_inited = true;

	Detail::DummyReceiver	dummyReceiver;
    AttrListHelper      	attrListHelper(-1, false);

    typedef typename Detail::TypeTraits<T>::TypeWithoutConst	TypeWithoutConst;
    T::processMetaData(dummyReceiver, &attrListHelper, static_cast<TypeWithoutConst*>(NULL));
}


template<typename T>
	template<typename AdditionalDefT>
void Attribute<T>::addAttributes(AdditionalDefT* /* dummy = NULL */ )
{
	if(!AdditionalInit<AdditionalDefT>::ms_inited)
	{
	    init();

        AdditionalInit<AdditionalDefT>::ms_inited = true;

		Detail::DummyReceiver	dummyReceiver;
	    AttrListHelper      	attrListHelper(-1, true);

	    typedef typename Detail::TypeTraits<T>::TypeWithoutConst	TypeWithoutConst;
	    AdditionalDefT::processMetaData(dummyReceiver, &attrListHelper, static_cast<TypeWithoutConst*>(NULL));
	}
}

template<typename T>
bool Attribute<T>::ms_inited = false;

template<typename T>
	template<typename AdditionalDefT>
bool Attribute<T>::AdditionalInit<AdditionalDefT>::ms_inited = false;


template<typename T, typename TreaterType>
inline void treatData(TreaterType& treater)
{
    typedef typename Detail::TypeTraits<T>::TypeWithoutConst	TypeWithoutConst;

    TypeWithoutConst::processMetaData(treater, static_cast<Detail::DummyReceiver*>(NULL), static_cast<TypeWithoutConst*>(NULL));
}


} // namespace Meta


#endif // Metadata_f4c2a193c01aeba365ae_H
