#ifndef TAC_H
#define TAC_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include "symbolTable.h"

symbolTable extern SYMBOL_TABLE;
int extern printNr;

struct byteCode
{
    byteCode() {}
    byteCode(int index, std::string instruction) : index(index), instruction(instruction) {}
    int index;
    std::string instruction;
};

class tac
{
public:
    std::string op, lhs, rhs, result;
    std::vector<byteCode *> bytecodes;
    bool lhsSet;
    virtual void dump()
    {
        std::cout << result << ":=" << lhs << op << rhs << std::endl;
        std::string tacVal = "( " + lhs + " " + op + " " + rhs + " )";
        variable* var = new variable(result,tacVal);
        SYMBOL_TABLE.putTemps(result,var);
    }
    virtual std::string getTacStream()
    {
            std::string tacString;
            tacString = result + ":=" + lhs + " " + op + " " + rhs;
            std::cout << result << ":=" << lhs << op << rhs << std::endl;
            return tacString;
    }
    std::string decideOp()
    {
        if (op == "AddOP")
        {
            return "iadd ";
        }
        else if (op == "SubOP")
        {
            return "isub ";
        }
        else if (op == "MultOP")
        {
            return "imul ";
        }
        else if (op == "DivOP")
        {
            return "idiv ";
        }
        else if (op == "AndOP")
        {
            return "iand ";
        }
        else if (op == "OrOP")
        {
            return "ior ";
        }
        else if (op == "NotOP")
        {
            return "inot ";
        }
        else if (op == "LesserOp")
        {
            return "ilt ";
        }
        else if (op == "GreaterOp")
        {
            return "igt ";
        }
        else if (op == "EqualsOp")
        {
            return "ieq ";
        }
        else
        {
            return "";
        }
    }
    std::string decideIn()
    {
        if (result == "print")
        {
            return "print";
        }
    }
    
    std::string generate_code()
    {
        if (lhs.length() > 0)
        {
            byteCode *newByteCode = new byteCode(0, "iload " + lhs);
            bytecodes.push_back(newByteCode);
        }
        if (rhs.length() > 0)
        {
            byteCode *newByteCode = new byteCode(0, "iload " + rhs);
            bytecodes.push_back(newByteCode);
        }
        if (op.length() > 0)
        {
            byteCode *newByteCode = new byteCode(0, (decideOp() + rhs));
            bytecodes.push_back(newByteCode);
        }
        if (rhs.length() > 0)
        {
            byteCode *newByteCode = new byteCode(0, "iload " + rhs);
            bytecodes.push_back(newByteCode);
        }
    }
};

class expression : public tac
{
public:
    expression(std::string _op, std::string _y, std::string _z, std::string _result)
    {
        op = _op;
        lhs = _y;
        rhs = _z;
        result = _result;
        if (_result == "print")
        {
            result = result + std::to_string(printNr);
            printNr++;
        }
    }
};

class parameter : public tac
{
public:
    parameter(std::string _p)
    {
        op = "param";
        lhs = "";
        rhs = _p;
        result = "";
    }
    void dump() override
    {
        std::cout << op << " " << rhs << std::endl;
    }
    std::string getTacStream()
    {
            std::string tacString;
            tacString = op + " " + rhs;
            return tacString;
    }
};
class methodCallTac : public tac
{
public:
    methodCallTac(std::string _f, std::string _n, std::string _result)
    {
        op = "call";
        lhs = _f;
        rhs = _n;
        result = _result;
    }
    void dump() override
    {
        std::cout << result << ":=" << op << " " << lhs << " " << rhs << std::endl;
        std::string tacVal = "( " + op + " " + lhs + " " + rhs + " )";
        variable* var = new variable(result,tacVal);
        SYMBOL_TABLE.putTemps(result,var);
    }
    std::string getTacStream()
    {
            std::string tacString;
            tacString = result + ":=" + op + " " + lhs + " " + rhs;
            return tacString;
    }
};

class jump : public tac
{
public:
    jump(std::string _label)
    {
        op = "goto";
        result = _label;
    }
};

class condJump : public tac
{
public:
    condJump(std::string _op, std::string _x, std::string _result)
    {
        op = _op;
        lhs = _x;
        result = _result;
    }
};

#endif
