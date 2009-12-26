#ifndef Treater_080909_H
#define Treater_080909_H

#include <iostream>
#include <iomanip>
#include <tools/metadata.h>


//enum TypeKind { TK_STRUCT, TK_SIMPLE };

struct SimpleTypeTag {};
struct StructuredTypeTag {};

template<typename T>
struct TypeTraits { typedef StructuredTypeTag TypeKind; };


template<> struct TypeTraits<int>             { typedef SimpleTypeTag TypeKind; };
template<> struct TypeTraits<double>          { typedef SimpleTypeTag TypeKind; };
template<> struct TypeTraits<std::string>     { typedef SimpleTypeTag TypeKind; };


#endif // Treater_080909_H
