#ifndef TQ_H
#define TQ_H

#include<string>
#include<memory>
#include<vector>
#include<queue>
#include<fstream>

namespace TQ{
    //save occurs content
    class Result{
    public:
        Result(const std::string& content,size_t line_number):
            line_content(content),
            line_number(line_number)
        {}
        friend class QueryBase;
        friend class TextQuery;
        friend class Query;
        bool operator<(const Result& r)const;
    private:
        std::string line_content{""};
        size_t line_number;
    };

    //abstract query base class
    class QueryBase{
    public:
        void occursInFile(std::ifstream& in,
                          const std::string& word);
        virtual QueryBase& operator&(const QueryBase& q) = 0;
        virtual QueryBase& operator|(const QueryBase& q) = 0;
        std::vector<Result> result_list;
    };

    //single word occurs
    class TextQuery:public QueryBase{
    public:
        virtual QueryBase& operator&(const QueryBase& q)override;
        virtual QueryBase& operator|(const QueryBase& q)override;
    };

    //helpful query class
    class Query{
    public:
        Query() = default;
        void parseExpr(const std::string& expr);
        void evalCommands(std::ifstream& in);
    private:
        std::queue<std::string> commands;
    };
}

#endif

