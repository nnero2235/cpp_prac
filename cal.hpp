#ifndef CAL_H
#define CAL_H
#include<stack>
#include<string>
#include<memory>

namespace Cal{
    class CalResult{
    public:
        static constexpr int CODE_SUCCESS{1};
        static constexpr int CODE_FAIL{0};
        CalResult(int code,double result,const std::string& msg):
            code(code),
            result(result),
            msg(msg)
        {};
        CalResult(double result):
            result(result)
        {};
        bool isSuccess()const;
        const std::string& getMsg()const;
        double getResult()const;
    private:
        int code{1};
        double result{0};
        std::string msg{"ok"};
    };

    class Elem{
    public:
        enum EType{
            NUMBER,
            ADD,
            DIV,
            SUB,
            MULTI,
            LEFT_PARENTH,
            RIGHT_PARENTH,
            UNKNOW
        };
        Elem(double value,EType type):
            value(value),
            type(type)
        {}
        std::unique_ptr<Elem> parseElem(const std::string& symbol);
        friend class Calculator;
    private:
        double value{0};
        EType type{EType::UNKNOW};
    };
    
    //calculate expr and return CalResult
    class Calculator{
    public:
        CalResult& parseAndCalculate(const std::string& expr);
    private:
        std::stack<Elem> elem_stack;
    };
}

#endif
