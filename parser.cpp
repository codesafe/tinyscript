
#include "parser.h"
#include "ast.h"
#include "excuter.h"    

parser::parser()
{
    parselineno = 0;
}

parser::~parser()
{
}

void parser::addtoken(TokenType type, string text)
{
    tokens.push_back(Token{ type, text });
}

void parser::checkandpass(TokenType type)
{
    if (tokens[parselineno].type != type)
    {
        printf("error in line %d\n", parselineno);
        _ASSERT(0);
        exit(0);
    }

    parselineno++;
}

// print node
node* parser::createprint()
{
    checkandpass(TokenType::String);
    checkandpass(TokenType::LeftParen);

    node* expr = expression();
    checkandpass(TokenType::RightParen);
    checkandpass(TokenType::Semicolon);
    return new printnode(expr);
}


node* parser::parseBlock()
{
    checkandpass(TokenType::LBrace);    // {

    vector<node*> statements;
    while (tokens[parselineno].type != TokenType::RBrace)
    {
        statements.push_back(statement());
    }
    checkandpass(TokenType::RBrace);    // }
    return new blocknode(statements);
}

// if command node
node* parser::createif()
{
    checkandpass(TokenType::String);
    checkandpass(TokenType::LeftParen);
    node* expr = expression();
    checkandpass(TokenType::RightParen);

	// match block 贸府
    node* matchblock = parseBlock();
    node* elseblock = nullptr;

	// else block
	if (tokens[parselineno].text == "else")
	{
		checkandpass(TokenType::String);
        elseblock = parseBlock();
	}

    return new ifnode(expr, matchblock, elseblock);
}

// while node
node* parser::createwhile()
{
	checkandpass(TokenType::String);
	checkandpass(TokenType::LeftParen);
	node* expr = expression();
	checkandpass(TokenType::RightParen);
	node* block = parseBlock();
	return new whilenode(expr, block);
}

node* parser::term()
{
	// 何龋 贸府
    bool numberpositive = true;
    if (tokens[parselineno].text == "-" && 
        (tokens[parselineno+1].type == TokenType::Number || 
         tokens[parselineno+1].type == TokenType::FloatNumber))
    {
        checkandpass(TokenType::Number);
        numberpositive = false;
    }

    if (tokens[parselineno].type == TokenType::Number)
    {
        int val = std::stoi(tokens[parselineno].text);
        val *= numberpositive ? 1 : -1;
        parselineno++;
        return new valuenode(val);
    }
    else if (tokens[parselineno].type == TokenType::FloatNumber)
    {
        float val = std::stof(tokens[parselineno].text);
        val *= numberpositive ? 1.0f : -1.0f;
        parselineno++;
        return new valuenode(val);
    }
    else if (tokens[parselineno].type == TokenType::DoubleQuote)
    {
        string val;
        checkandpass(TokenType::DoubleQuote);

        // string
        while (tokens[parselineno].type != TokenType::DoubleQuote)
        {
            val += tokens[parselineno].text;
            parselineno++;
        }

        checkandpass(TokenType::DoubleQuote);
        return new valuenode(val);
    }
    else if (tokens[parselineno].type == TokenType::LeftParen)
    {
        parselineno++;
        node* n = expression();
        checkandpass(TokenType::RightParen);
        return n;
    }
    else if (tokens[parselineno].type == TokenType::String)
    {
        //string val = tokens[parselineno].text;
        //parselineno++;
        //return new valuenode(val);

        string val = tokens[parselineno].text;
        parselineno++;
        return new variablenode(val);
    }
    else
    {
        // error
        throw std::runtime_error("Syntax error in term");
    }
}

node* parser::statement()
{
    string tex = tokens[parselineno].text;

    if (tex == "int" || tex == "float" || tex == "string")
    {
        // parse variable
        checkandpass(TokenType::String);
        return createassignment();
    }
    else if (tex == "print")
    {
        return createprint();
    }
    else if (tex == "if")
    {
        return createif();
    }
    else if (tex == "while")
    {
        return createwhile();
    }
    else
    {
        // variable assign : a=a+10;
        if (tokens[parselineno + 1].text == "=")
        {
            // variable assignment
            return createassignment();
        }
        else
        {
            // function call
            return createfunctioncall();
        }
    }

    throw std::runtime_error("Invalid statement");
}

node* parser::expression()
{
    node* n = term();
    while (true)
    {
        if (tokens[parselineno].type == TokenType::Plus || 
            tokens[parselineno].type == TokenType::Minus ||
            tokens[parselineno].type == TokenType::Multipe || 
            tokens[parselineno].type == TokenType::Divide ||
            tokens[parselineno].type == TokenType::GreaterThan || 
            tokens[parselineno].type == TokenType::GreaterThanEqual ||
            tokens[parselineno].type == TokenType::LessThan || 
            tokens[parselineno].type == TokenType::LessThanEqual ||
            tokens[parselineno].type == TokenType::Equal || 
            tokens[parselineno].type == TokenType::NotEqual ||
			tokens[parselineno].type == TokenType::CompareOr || 
            tokens[parselineno].type == TokenType::CompareAnd)
        {
            TokenType token = tokens[parselineno].type;
            parselineno++;
            n = new opernode(token, n, term());
        }
        else
        {
            break;
        }
    }
    return n;
}

node* parser::createassignment()
{
    string varName = tokens[parselineno].text;
    checkandpass(TokenType::String);
    checkandpass(TokenType::Assign);

    node* expr = expression();
    checkandpass(TokenType::Semicolon);
    return new assignnode(varName, expr);
}


node* parser::createfunctioncall()
{
    string funcName = tokens[parselineno].text;
    checkandpass(TokenType::String);
    checkandpass(TokenType::LeftParen);
    vector<node*> args;
    while (tokens[parselineno].type != TokenType::RightParen)
    {
        args.push_back(expression());
        if (tokens[parselineno].type == TokenType::Comma)
            checkandpass(TokenType::Comma);
    }
    checkandpass(TokenType::RightParen);
    checkandpass(TokenType::Semicolon);
    return new funccallnode(funcName, args);
}


node* parser::createfunction()
{
    checkandpass(TokenType::String);
    std::string funcName = tokens[parselineno].text;
    checkandpass(TokenType::String);

    checkandpass(TokenType::LeftParen);
    vector<node*> args;
    while (tokens[parselineno].type != TokenType::RightParen)
    {
        args.push_back(expression());
        if (tokens[parselineno].type == TokenType::Comma)
            checkandpass(TokenType::Comma);
    }
    checkandpass(TokenType::RightParen);

    checkandpass(TokenType::LBrace);
    std::vector<node*> body;
    while (tokens[parselineno].type != TokenType::RBrace)
    {
        body.push_back(statement());
    }
    checkandpass(TokenType::RBrace);

    return new funcnode(funcName, args, body);
}


void parser::parse(excuter& exe)
{
    parselineno = 0;

    while (parselineno < tokens.size())
    {
        // Gloval variable or function
        if (tokens[parselineno].text == "func")
        {
            // create function
            funcnode *fnode = dynamic_cast<funcnode*>(createfunction());
            exe.addfunction(fnode);
        }
        else if (tokens[parselineno].text == "int" ||
            tokens[parselineno].text == "float" ||
            tokens[parselineno].text == "string")
        {
            // create global variable
            string tex = tokens[parselineno].text;
            if (tex == "int" || tex == "float" || tex == "string")
            {
                // parse variable
                checkandpass(TokenType::String);
                exe.addglobalvariable(createassignment());
            }
        }
        else
        {
            // 场!
            if (tokens[parselineno].type == TokenType::EndOfFile)
                break;
        }
    }
}
