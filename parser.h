#ifndef PARSER
#define PARSER

#include "predef.h"

class node;
class excuter;

class parser
{
public:
	parser();
	~parser();

    void parse(excuter& exe);
    node* parseBlock();
    void addtoken(TokenType type, string text);

private:

    void checkandpass(TokenType type);

    node* term();
    node* expression();
    node* statement();

    node* createassignment();
    node* createfunction();
    node* createfunctioncall();
    node* createprint();
    node* createif();
    node* createwhile();

    vector<Token> tokens;
    int parselineno;
};


#endif

