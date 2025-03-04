

#include "predef.h"
#include "ast.h"
#include "parser.h"
#include "excuter.h"

////////////////////////////////////////////////////////////////////////////////////


parser par;
excuter exe;


////////////////////////////////////////////////////////////////////////////////////


bool scripttokenize(string line)
{
    printf("=====================================\n");
    printf("parse : %s\n", line.c_str());
    printf("=====================================\n");


    vector<string> words;
    string token;
    bool inString = false;

    //const char seperators[] = { "=/;{},*+-\"()" };

    for (size_t i = 0; i < line.length(); ++i)
    {
        char c = line[i];

        if (isspace(c) && !inString)
        {
            if (!token.empty())
            {
                words.push_back(token);
                token.clear();
            }
        }
        //else if (c == '\"')
        //{
        //    if (inString)
        //    {
        //        //token += c;
        //        words.push_back(token);
        //        token.clear();
        //        inString = false;
        //    }
        //    else
        //    {
        //        if (!token.empty())
        //        {
        //            words.push_back(token);
        //            token.clear();
        //        }
        //        //token += c;
        //        inString = true;
        //    }
        //}
        else if (c == '=' || c == ';' || c == '{' || c == '}' || c == ','
            || c == '+' || c == '-' || c == '*' || c == '/' || c == '('
            || c == ')' || c == '\"')
        {
            if (!token.empty())
            {
                words.push_back(token);
                token.clear();
            }
            words.push_back(string(1, c));

            if (c == '\"')
				inString = !inString;
        }
        else
        {
            token += c;
        }
    }

    if (!token.empty())
        words.push_back(token);

    for (string t : words)
    {
        Token token;

        if (isdigit(t[0]) || t[0] == '-')
        {
            bool hasDecimalPoint = false;
            for (size_t i = 0; i < t.length(); ++i) 
            {
                if (isdigit(t[i])) 
                {
                    continue;
                }
                else if (t[i] == '.' && !hasDecimalPoint) 
                {
                    hasDecimalPoint = true;
                }
                else if (i == 0 && (t[i] == '-' || t[i] == '+')) {
                    continue;
                }
                else 
                    break;
            }

            token.type = hasDecimalPoint ? TokenType::FloatNumber : TokenType::Number;
        }
        else if (isalpha(t[0]))
            token.type = TokenType::String;
        else
        {
            if (t == "==")
                token.type = TokenType::Equal;
            else if (t == "!=")
                token.type = TokenType::NotEqual;
            else if (t == ">")
                token.type = TokenType::GreaterThan;
            else if (t == ">=")
                token.type = TokenType::GreaterThanEqual;
            else if (t == "<")
                token.type = TokenType::LessThan;
            else if (t == "<=")
                token.type = TokenType::LessThanEqual;
            else if (t == "||")
                token.type = TokenType::CompareOr;
            else if (t == "&&")
                token.type = TokenType::CompareAnd;
            else
            {
                if (t[0] == '=')
                    token.type = TokenType::Assign;
                else if (t[0] == '{')
                    token.type = TokenType::LBrace;
                else if (t[0] == '}')
                    token.type = TokenType::RBrace;
                else if (t[0] == ';')
                    token.type = TokenType::Semicolon;
                else if (t[0] == '(')
                    token.type = TokenType::LeftParen;
                else if (t[0] == ')')
                    token.type = TokenType::RightParen;
                else if (t[0] == '+')
                    token.type = TokenType::Plus;
                else if (t[0] == '-')
                    token.type = TokenType::Minus;
                else if (t[0] == '*')
                    token.type = TokenType::Multipe;
                else if (t[0] == '/')
                    token.type = TokenType::Divide;
                else if (t[0] == '\"')
                    token.type = TokenType::DoubleQuote;
                else if (t[0] == '|')
                    token.type = TokenType::Or;
                else if (t[0] == '&')
                    token.type = TokenType::And;
                else if (t[0] == '!')
                    token.type = TokenType::Not;
                else
                {
                    token.type = TokenType::String;
                }
            }
        }

        token.text = t;
		par.addtoken(token.type, token.text);

        printf("token : %s\n", token.text.c_str());
    }

    return true;
}

string trimStart(const string& str)
{
    size_t start = 0;
    while (start < str.length() && (str[start] == SPACE || str[start] == TAB))
    {
        start++;
    }
    return str.substr(start);
}

vector<string> readFileLines(const string& filename)
{
    vector<string> lines;
    ifstream file(filename);
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            line = trimStart(line);

            // 주석으로 시작하는 라인은 제외
            if (!line.empty() && line[0] != '/')
            {
                lines.push_back(line);
            }
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file: " << filename << endl;
    }

    return lines;
}



////////////////////////////////////////////////////////////////////////////////////

int main()
{
    vector<string> lines = readFileLines("testscript.txt");

    for (int i = 0; i < lines.size(); i++)
        scripttokenize(lines[i]);
	par.addtoken(TokenType::EndOfFile, "EOF");
    par.parse(exe);

    exe.run();

    return 0;
}