#ifndef AttribTypes_92378947b2378681168f_H
#define AttribTypes_92378947b2378681168f_H

#include <string>
#include "metadata.h"


namespace Meta
{


struct Comment
{
    std::string comment;

    explicit Comment(std::string const& s) : comment(s)
    {}
};

class TreatAsString
{
public:
    explicit TreatAsString(bool val_ = true) : val(val_)
    {}

    operator bool() const { return val; }
private:
    bool val;
};

class INeedTreat
{
public:
    enum TreatFlag { TF_YES, TF_NO, TF_BREAK };

    virtual TreatFlag treatFlag(unsigned index) const = 0;
};

class NeedTreat : public INeedTreat
{
public:
    explicit NeedTreat(boost::shared_ptr<INeedTreat> const& arrTreat) : m_arrTreat(arrTreat)
    {
        META_LIB_ASSERT(m_arrTreat, "pSize shoud be not NULL!");
    }

    virtual TreatFlag treatFlag(unsigned index) const { return m_arrTreat->treatFlag(index); }
private:
    boost::shared_ptr<INeedTreat> m_arrTreat;
};

class ArraySize : public INeedTreat
{
public:
    explicit ArraySize(unsigned size_ = 0) : size(size_) {}

    virtual TreatFlag treatFlag(unsigned index) const { return index < size ? TF_YES : TF_BREAK; }

    unsigned size;
};

class FloatPrec
{
public:
    static int const NOT_SPECIFIED_PREC = -1;

    explicit FloatPrec(int prec = NOT_SPECIFIED_PREC) : m_prec(prec)
    {}

    int precision() const { return m_prec; }
private:
    int m_prec;
};


} /// namespace Meta


#endif AttribTypes_92378947b2378681168f_H
