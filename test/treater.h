#ifndef Treater_090208_H
#define Treater_090208_H

#include <iostream>
#include <iomanip>
#include <Tools/metadata.h>


//enum TypeKind { TK_STRUCT, TK_SIMPLE };

struct SimpleTypeTag {};
struct StructuredTypeTag {};

template<typename T>
struct TypeTraits { typedef StructuredTypeTag TypeKind; };


template<> struct TypeTraits<int>             { typedef SimpleTypeTag TypeKind; };
template<> struct TypeTraits<double>          { typedef SimpleTypeTag TypeKind; };
template<> struct TypeTraits<std::string>     { typedef SimpleTypeTag TypeKind; };


#endif // Treater_090208_H
