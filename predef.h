#ifndef PREDEF
#define PREDEF

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <string>
#include <map>
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <unordered_map>

#pragma warning(disable: 4996)

using namespace std;


typedef unsigned char ubyte;
typedef char byte;
typedef unsigned int uint;

#define TAB			0x09
#define SPACE		0x20
#define countof(a) (sizeof(a)/sizeof(a[0]))

enum varEnum
{
	VAR_NONE,	// error
	VAR_VOID,
	VAR_INT,
	VAR_FLOAT,
	VAR_STRING
};

enum TokenType
{
    Number,
	FloatNumber,
    String,

    Plus,
    Minus,
    Multipe,
    Divide,

    DoubleQuote,
    Or,             // |
    And,            // &
    Assign,         // =
    Semicolon,      // ;

    LeftParen,      // (
    RightParen,     // )
    LBrace,         // {
    RBrace,         // }
    Comma,          // ,

    GreaterThan,        // >
    GreaterThanEqual,   // >=
    LessThan,           // <
    LessThanEqual,      // <=
    Equal,              // ==
    NotEqual,           // !=
    CompareOr,          // ||
    CompareAnd,         // &&
    Not,                // !

    EndOfFile
};

struct Token
{
    TokenType type;
    string text;
};




#endif

