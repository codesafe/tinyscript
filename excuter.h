#ifndef EXCUTER
#define EXCUTER

#include "predef.h"

struct _variable;
class funcnode;
class node;


class excuter
{
private:
    unordered_map<std::string, funcnode*> functionMap;
    unordered_map<std::string, assignnode*> globalVariables;

public:

	void addglobalvariable(node* n)
	{
		assignnode* assign = dynamic_cast<assignnode*>(n);
		if (assign == nullptr)
		{
			throw std::runtime_error("global variable must be assigned");
		}

        if (globalVariables.find(assign->variableName) == globalVariables.end())
        {
            globalVariables[assign->variableName] = assign;
        }
        else
        {
            throw std::runtime_error("duplicate global variable " + assign->variableName);
        }

	}

	void addfunction(funcnode* func)
	{
		functionMap[func->functionName] = func;
	}

    void run()
    {
        if (functionMap.find("main") != functionMap.end())
        {
            runFunction("main", {});
        }
        else
        {
            throw std::runtime_error("'main' function not found");
        }
    }

    void runFunction(const std::string& functionName, const std::vector<_variable>& args)
    {
        auto it = functionMap.find(functionName);
        if (it == functionMap.end())
        {
            throw std::runtime_error("Undefined function: " + functionName);
        }

        funcnode* func = it->second;
        std::unordered_map<std::string, _variable> localVariables;

        // 인자들을 로컬 변수로 설정
        for (size_t i = 0; i < func->args.size(); ++i)
        {
            variablenode* argNode = dynamic_cast<variablenode*>(func->args[i]);
            if (argNode)
            {
                localVariables[argNode->name] = args[i];
            }
        }

        for (node* n : func->body)
        {
            execute(n, localVariables);
        }
    }

    void execute(node* n, std::unordered_map<std::string, _variable>& scopeVariables)
    {
        if (assignnode* assignmentNode = dynamic_cast<assignnode*>(n))
        {
            _variable value = evaluate(assignmentNode->expression, scopeVariables);
            scopeVariables[assignmentNode->variableName] = value;
        }
        else if (printnode* printNode = dynamic_cast<printnode*>(n))
        {
            _variable value = evaluate(printNode->expression, scopeVariables);
            value.print();
            //std::cout << value << std::endl;
		}
		else if (ifnode* ifNode = dynamic_cast<ifnode*>(n))
		{
			_variable value = evaluate(ifNode->condition, scopeVariables);

            // if true
			if (value.valueint)
			{
				execute(ifNode->matchbranch, scopeVariables);
			}
            else
            {
                execute(ifNode->elsebranch, scopeVariables);
            }
		}
		else if (whilenode* whileNode = dynamic_cast<whilenode*>(n))
		{
			while (evaluate(whileNode->condition, scopeVariables).valueint)
			{
				execute(whileNode->block, scopeVariables);
			}
		}
        else if (funccallnode* callNode = dynamic_cast<funccallnode*>(n))
        {
            std::vector<_variable> args;
            for (node* arg : callNode->args)
            {
                args.push_back(evaluate(arg, scopeVariables));
            }
            runFunction(callNode->functionName, args);
        }
		else if (blocknode* blockNode = dynamic_cast<blocknode*>(n))
		{
			for (node* statement : blockNode->statements)
			{
				execute(statement, scopeVariables);
			}
		}
        else
        {
            throw std::runtime_error("Unknown AST node in execute");
        }
    }

    _variable evaluate(node* n, std::unordered_map<std::string, _variable>& scopeVariables)
    {
        if (valuenode* v = dynamic_cast<valuenode*>(n))
        {
            return v->value;
        }

        if (variablenode* variableNode = dynamic_cast<variablenode*>(n))
        {
            if (scopeVariables.find(variableNode->name) != scopeVariables.end())
            {
                return scopeVariables[variableNode->name];
            }

			// 전역 변수 찾기
			if (globalVariables.find(variableNode->name) != globalVariables.end())
			{
				return evaluate(globalVariables[variableNode->name]->expression, scopeVariables);
			}

            throw std::runtime_error("Undefined variable: " + variableNode->name);
        }

        if (opernode* binOpNode = dynamic_cast<opernode*>(n))
        {
            _variable leftValue = evaluate(binOpNode->left, scopeVariables);
            _variable rightValue = evaluate(binOpNode->right, scopeVariables);

            if (binOpNode->op == TokenType::Plus)
            {
                return leftValue + rightValue;
            }
            else if (binOpNode->op == TokenType::Minus)
            {
                return leftValue - rightValue;
            }
            else if (binOpNode->op == TokenType::Multipe)
            {
                return leftValue * rightValue;
            }
            else if (binOpNode->op == TokenType::Divide)
            {
                return leftValue / rightValue;
            }
            else if (binOpNode->op == TokenType::And)
            {
                return leftValue & rightValue;
            }
            else if (binOpNode->op == TokenType::Or)
            {
                return leftValue | rightValue;
            }
            else if (binOpNode->op == TokenType::GreaterThan)
            {
                _variable result;
                result.type = varEnum::VAR_INT;
                result.valueint = (leftValue.valueint > rightValue.valueint) ? 1 : 0;
                return result;
            }
            else if (binOpNode->op == TokenType::GreaterThanEqual)
            {
                _variable result;
                result.type = varEnum::VAR_INT;
                result.valueint = (leftValue.valueint >= rightValue.valueint) ? 1 : 0;
                return result;
            }
            else if (binOpNode->op == TokenType::LessThan)
            {
                _variable result;
                result.type = varEnum::VAR_INT;
                result.valueint = (leftValue.valueint < rightValue.valueint) ? 1 : 0;
                return result;
            }
            else if (binOpNode->op == TokenType::LessThanEqual)
            {
                _variable result;
                result.type = varEnum::VAR_INT;
                result.valueint = (leftValue.valueint <= rightValue.valueint) ? 1 : 0;
                return result;
            }
            else if (binOpNode->op == TokenType::Equal)
            {
                _variable result;
                result.type = varEnum::VAR_INT;
                result.valueint = (leftValue.valueint == rightValue.valueint) ? 1 : 0;
                return result;
            }
            else if (binOpNode->op == TokenType::NotEqual)
            {
                _variable result;
                result.type = varEnum::VAR_INT;
                result.valueint = (leftValue.valueint != rightValue.valueint) ? 1 : 0;
                return result;
            }
            else if (binOpNode->op == TokenType::CompareOr)
            {
                _variable result;
                result.type = varEnum::VAR_INT;
                result.valueint = (leftValue.valueint || rightValue.valueint) ? 1 : 0;
                return result;
            }
            else if (binOpNode->op == TokenType::CompareAnd)
            {
                _variable result;
                result.type = varEnum::VAR_INT;
                result.valueint = (leftValue.valueint && rightValue.valueint) ? 1 : 0;
                return result;
            }
        }

        throw std::runtime_error("Error in evaluate");
    }
};




#endif