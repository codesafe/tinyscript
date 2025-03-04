#ifndef AST
#define AST

#include "predef.h"


struct _variable
{
    varEnum type;
    int valueint;
    float valuefloat;
    string valuestring;

    _variable()
    {
        type = VAR_NONE;
        valueint = 0;
        valuefloat = 0;
        valuestring = "";
    }

    _variable operator+(const _variable& other) const
    {
        _variable ret;
        if (type == varEnum::VAR_STRING || other.type == varEnum::VAR_STRING)
        {
            // 둘중 하나라도 string이면 string +
            std::string a, b;
            if (type == varEnum::VAR_INT)
                a = std::to_string(valueint);
            else if (type == varEnum::VAR_FLOAT)
                a = std::to_string(valuefloat);
            else if (type == varEnum::VAR_STRING)
                a = valuestring;

            if (other.type == varEnum::VAR_INT)
                b = std::to_string(other.valueint);
            else if (other.type == varEnum::VAR_FLOAT)
                b = std::to_string(other.valuefloat);
            else if (other.type == varEnum::VAR_STRING)
                b = other.valuestring;

            ret.type = varEnum::VAR_STRING;
            ret.valuestring = a + b;
        }
        else if (type == varEnum::VAR_FLOAT || other.type == varEnum::VAR_FLOAT)
        {
            // 둘중 하나라도 float이면 float +
            float a = type == varEnum::VAR_INT ? (float)valueint : valuefloat;
            float b = other.type == varEnum::VAR_INT ? (float)other.valueint : other.valuefloat;
            ret.type = varEnum::VAR_FLOAT;
            ret.valuefloat = a + b;
        }
        else
        {
            // int + int
            ret.type = varEnum::VAR_INT;
            ret.valueint = valueint + other.valueint;
        }

        return ret;
    }

    _variable operator-(const _variable& other) const
    {
        _variable ret;
        if (type == varEnum::VAR_STRING || other.type == varEnum::VAR_STRING)
        {
            // 둘중 하나라도 string이면 Error
            std::cout << "minus operator ?? Error" << std::endl;
        }
        else if (type == varEnum::VAR_FLOAT || other.type == varEnum::VAR_FLOAT)
        {
            // 둘중 하나라도 float이면 float -
            float a = type == varEnum::VAR_INT ? (float)valueint : valuefloat;
            float b = other.type == varEnum::VAR_INT ? (float)other.valueint : other.valuefloat;
            ret.type = varEnum::VAR_FLOAT;
            ret.valuefloat = a - b;
        }
        else
        {
            // int - int
            ret.type = varEnum::VAR_INT;
            ret.valueint = valueint - other.valueint;
        }

        return ret;
    }

    _variable operator*(const _variable& other) const
    {
        _variable ret;
        if (type == varEnum::VAR_STRING || other.type == varEnum::VAR_STRING)
        {
            // 둘중 하나라도 string이면 Error
            std::cout << "minus operator ?? Error" << std::endl;
        }
        else if (type == varEnum::VAR_FLOAT || other.type == varEnum::VAR_FLOAT)
        {
            // 둘중 하나라도 float이면 float -
            float a = type == varEnum::VAR_INT ? (float)valueint : valuefloat;
            float b = other.type == varEnum::VAR_INT ? (float)other.valueint : other.valuefloat;
            ret.type = varEnum::VAR_FLOAT;
            ret.valuefloat = a * b;
        }
        else
        {
            // int - int
            ret.type = varEnum::VAR_INT;
            ret.valueint = valueint * other.valueint;
        }
        return ret;
    }

    _variable operator/(const _variable& other) const
    {
        _variable ret;
        if (type == varEnum::VAR_STRING || other.type == varEnum::VAR_STRING)
        {
            // 둘중 하나라도 string이면 Error
            std::cout << "minus operator ?? Error" << std::endl;
        }
        else if (type == varEnum::VAR_FLOAT || other.type == varEnum::VAR_FLOAT)
        {
            // 둘중 하나라도 float이면 float -
            float a = type == varEnum::VAR_INT ? (float)valueint : valuefloat;
            float b = other.type == varEnum::VAR_INT ? (float)other.valueint : other.valuefloat;
            ret.type = varEnum::VAR_FLOAT;
            ret.valuefloat = a / b;
        }
        else
        {
            // int - int
            ret.type = varEnum::VAR_INT;
            ret.valueint = valueint / other.valueint;
        }
        return ret;
    }

    _variable operator&(const _variable& other) const
    {
        _variable ret;
        if (type == varEnum::VAR_STRING || other.type == varEnum::VAR_STRING)
        {
            // 둘중 하나라도 string이면 Error
            std::cout << "minus operator ?? Error" << std::endl;
        }
        else if (type == varEnum::VAR_FLOAT || other.type == varEnum::VAR_FLOAT)
        {
            // 둘중 하나라도 float이면 Error
            std::cout << "minus operator ?? Error" << std::endl;
        }
        else
        {
            // int - int
            ret.type = varEnum::VAR_INT;
            ret.valueint = valueint & other.valueint;
        }
        return ret;
    }

    _variable operator|(const _variable& other) const
    {
        _variable ret;
        if (type == varEnum::VAR_STRING || other.type == varEnum::VAR_STRING)
        {
            // 둘중 하나라도 string이면 Error
            std::cout << "minus operator ?? Error" << std::endl;
        }
        else if (type == varEnum::VAR_FLOAT || other.type == varEnum::VAR_FLOAT)
        {
            // 둘중 하나라도 float이면 Error
            std::cout << "minus operator ?? Error" << std::endl;
        }
        else
        {
            // int - int
            ret.type = varEnum::VAR_INT;
            ret.valueint = valueint | other.valueint;
        }
        return ret;
    }

    void print()
    {
        switch (type)
        {
        case VAR_NONE:
            std::cout << "none ?? Error" << std::endl;
            break;
        case VAR_VOID:
            std::cout << "void??" << std::endl;
            break;
        case VAR_INT:
            std::cout << valueint << std::endl;
            break;
        case VAR_FLOAT:
            std::cout << valuefloat << std::endl;
            break;
        case VAR_STRING:
            std::cout << valuestring << std::endl;
            break;
        }
    }

};


////////////////////////////////////////////////////////////////////////////////////

// AST node


class node
{
public:
    virtual ~node() = default;
};


class valuenode : public node
{
public:
    _variable value;
    valuenode(int val)
    {
        value.type = varEnum::VAR_INT;
        value.valueint = val;
    }

    valuenode(float val)
    {
        value.type = varEnum::VAR_FLOAT;
        value.valuefloat = val;
    }

    valuenode(std::string val)
    {
        value.type = varEnum::VAR_STRING;
        value.valuestring = val;
    }
};

class variablenode : public node
{
public:
    string name;
    variablenode(const std::string& varName) : name(varName) {}
};

// function node
class funcnode : public node
{
public:
    string functionName;
    vector<node*> args;
    vector<node*> body;

    funcnode(string& name, vector<node*>& args, vector<node*>& body)
    {
        functionName = name;
        this->args = args;
        this->body = body;
    }
};

// function call node
class funccallnode : public node
{
public:
    string functionName;
    vector<node*> args;

    funccallnode(string& name, vector<node*>& args)
    {
        functionName = name;
        this->args = args;
    }
};


// 연산 node
class opernode : public node
{
public:
    TokenType op;
    node* left;
    node* right;

    opernode(TokenType oper, node* l, node* r)
    {
        op = oper;
        left = l;
        right = r;
    }
};

// assign node
class assignnode : public node
{
public:
    string variableName;
    node* expression;

    assignnode(const std::string& var, node* expr)
    {
        variableName = var;
        expression = expr;
    }
};

// print node
class printnode : public node
{
public:
    node* expression;
    printnode(node* expr) : expression(expr) {}
};

// if node
class ifnode : public node
{
public:
	node* condition;
    node* matchbranch;  // block node
    node* elsebranch;   // block node

	ifnode(node* cond, node* matchBr, node* elseBr)
		: condition(cond), matchbranch(matchBr), elsebranch(elseBr) 
    { 
    }

	~ifnode()
	{
		delete condition;
		delete matchbranch;
		if (elsebranch)
			delete elsebranch;
	}
};

class blocknode : public node
{
public:
	vector<node*> statements;
	blocknode(vector<node*>& stmts) : statements(stmts) {}
	~blocknode()
	{
		for (node* n : statements)
			delete n;
	}
};

class whilenode : public node
{
public:
	node* condition;
	node* block;
	whilenode(node* cond, node* blk)
		: condition(cond), block(blk)
	{
	}
	~whilenode()
	{
		delete condition;
		delete block;
	}
};


#endif
