#include "main.h"

#include "envs.h"
#include "tokens.h"
#include "util.h"
#include "constants.h"
#include "executor.h"

static vector<Token> Lexer(string text) {
    vector<Token> tokens = vector<Token>{};
    
    int len = text.length();
   

    for (int pos = 0; pos < len; pos++) {

        char c = text[pos];
        stringstream val;
        val << c;

        if (Utils::IsWhiteSpace(c))
            continue;

        if (Utils::InList(LOGIC_DEF_CHARS, c)) {
            
            
            for (int i = pos + 1; i < len; i++) {
                if (not Utils::InList(LOGIC_DEF_CHARS, text[i])) break;
                val << text[i];
                pos = i;
            }
            if (Utils::InList(LOGIC_DEFS, val.str())) {
                tokens.push_back(Token(val.str(), LOGIC));
                
                continue;
            }
            
        }

        if (Utils::InList(OPERATORS, c)) {
           
            for (int i = pos + 1; i < len; i++) {
                if (not Utils::InList(OPERATORS, text[i])) break;
                val << text[i];
                pos = i;
            }

            if (Utils::InList(DEFS, val.str())) {
                tokens.push_back(Token(val.str(), DEF));
                continue;
            }
            

            tokens.push_back(Token(val.str(), OPERATOR));

            continue;
        }

        

        if (Utils::InList(TEXT_CHARS, c)) {
               
            for (int i = pos + 1; i < len; i++) {

                if (not Utils::InList(TEXT_CHARS, text[i]) and not isdigit(text[i])) break;
                pos = i;

                val << text[i];
            }
        
            if (val.str() == "true" or val.str() == "false") {
                
                
                tokens.push_back(Token(std::to_string((val.str() == "true")), BOOL));
                continue;
            }
                
                

            if (Utils::InList(KEYWORDS, val.str())) {
                tokens.push_back(Token(val.str(), KEYWORD));
                continue;
            }

            tokens.push_back(Token(val.str(), TEXT));


            continue;
        }
        //
        if (isdigit(c)) {
            bool period = false;
            for (int i = pos + 1; i < len; i++) {
                char ch = text[i];
                if (not isdigit(ch) and ch != '.') break;   
                if (ch == '.') {
                    if (period) break;                  
                    period = true;                       
                }
                val << ch;                               
                pos = i;                                 
            }
            tokens.push_back(Token(val.str(), NUMBER));

            continue;
        }

        if (c == '(' or c == ')') {
            val = stringstream();
            if (c == '(') val << "L";
            else val << "R";
            tokens.push_back(Token(val.str(), PARENTHESIS));
            continue;
        }
        if (c == '{' or c == '}') {
            tokens.push_back(Token(val.str(), BRACKET));
            continue;
        }

        if (c == ';') {
            tokens.push_back(Token(val.str(), ENDL));
            continue;
        }
        //77
        if (c == '"' or c == '\'') {
            val = stringstream();
            for (int i = pos + 1; i < len; i++) {
                char ch = text[i];
                pos = i;
                
                if (c == ch) break;
                val << ch;
            }
            tokens.push_back(Token(val.str(), STRING));
            continue;
        }

        if (c == ',') {
            tokens.push_back(Token(val.str(), COMMA));
            continue;

        }

        if (c == '[' or c == ']') {
            tokens.push_back(Token(val.str(), TABLEDEC));
            continue;    
        }

        if (c == '.') {
            tokens.push_back(Token(".", PERIOD));
            continue;
        }

        stringstream s;
        s << "Unexpected character: '" << c << "'";
        throw std::runtime_error(s.str());
    }


    return tokens;
}
//777777

void AddDefaultFunction(Function f, string tag, int  minargs, int maxargs) {
    globalEnvironment.defaultMethods.emplace(tag, DefaultMethod(f, maxargs, minargs));
}


void readFile(std::ifstream file) {
    try {
        stringstream text;
        string line;
        if (file.is_open())
        {
            while (std::getline(file, line))
            {
                text << line;
            }
            file.close();
        }

        vector<Token> codeTokens = Lexer(text.str());


        AddDefaultFunction(Print, "print", 1,1);
        AddDefaultFunction(RawPrint, "rawprint", 1,1);
        AddDefaultFunction(Input, "input", 0,1);
        AddDefaultFunction(Random, "random", 2,2);
        AddDefaultFunction(MakeString, "tostring", 1, 1);
        AddDefaultFunction(MakeNumber, "tonumber", 1, 1);

        Executor::Execute(codeTokens, globalEnvironment);
    }
    catch (std::runtime_error err) {
        std::cout << "Error: " << err.what() << '\n';
    }
    catch (...) {
        std::cout << "Error\n";
    }
    
}

int main(int argc, char** argv)
{
    
    //readFile(std::ifstream("C:/InterpreterCode/coolio.txt"));
   if (argc >= 3 and string(argv[1]) == "run") {
        string filename = argv[2];
        
        readFile(std::ifstream(filename));
        return 0;
    } else if (argc == 2 and string(argv[1]) == "-version") {

        std::cout << "MarshallScript version " << version << '\n';

        return 0;
    }
    std::cerr << "Usage:\n Running a file: MarshallScript run <file>\n Version: MarshallScript -version\n";
   
    return 1;
    //return 0;
    
}