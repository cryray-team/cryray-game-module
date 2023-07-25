#pragma once

#define STRINGIFY(s) STRINGIFYIMPL(s)
#define STRINGIFYIMPL(s) #s

// Create a dummy type returning a string
#define CREATE_STRING_DECL(string) \
 struct StringDecl##string \
 { \
 	constexpr static const char* str() {return STRINGIFY(string);} \
 }

#define GET_STRING_DECL(string) StringDecl##string

// helper function
constexpr unsigned c_strlen(char const* str, unsigned count = 0)
{
	return ('\0' == str[0]) ? count : c_strlen(str + 1, count + 1);
}

// Recursive function to dump typenamed string to runtime string
template< char ch, char... chars>
struct TStringOut
{
	static void out(string64& OutStr, int& InOutAccum)
	{
		OutStr[InOutAccum] = ch;
		++InOutAccum;
		TStringOut<chars...>::out(OutStr, InOutAccum);
	}
};

template< char ch >
struct TStringOut<ch>
{
	static void out(string64& OutStr, int& InOutAccum)
	{
		OutStr[InOutAccum] = ch;
		++InOutAccum;
	}
};

// destination "template string" type
template < char... arrayChar >
struct StringDeclType
{
	static void out(string64& OutString)
	{
		int Accum = 0;
		TStringOut<arrayChar...>::out(OutString, Accum);
	}
};

// struct to explode a `char const*` to an `exploded_string` type
template < typename StringProvider, unsigned StrLen, char... arrayChar >
struct ConvertStringImpl
{
	using result =
		typename ConvertStringImpl < StringProvider, StrLen - 1,
		StringProvider::str()[StrLen - 1],
		arrayChar... > ::result;
};

template < typename StringProvider, char... arrayChar >
struct ConvertStringImpl < StringProvider, 0, arrayChar... >
{
	using result = StringDeclType < arrayChar... >;
};

// syntactical sugar
template < typename StringProvider >
using ConvertStringToTypename =
typename ConvertStringImpl < StringProvider,
	c_strlen(StringProvider::str()) > ::result;