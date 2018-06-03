#include "tq.hpp"
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace TQ;

bool Result::operator<(const Result& r)const{
    return this->line_number < r.line_number;
}

void QueryBase::occursInFile(std::ifstream& in,
                             const std::string& word){
    std::string line;
    size_t line_number = 1;
    while(!in.eof()){
        std::getline(in, line);
        if(line.find(word) != std::string::npos){
            Result r(line,line_number);
            result_list.push_back(r);
        }
        ++line_number;
    }
    in.clear();
    in.seekg(0,std::ios::beg);
}

QueryBase& TextQuery::operator&(const QueryBase& q){
    std::vector<Result> new_vector;
    std::set_intersection(this->result_list.begin(), this->result_list.end(), q.result_list.begin(), q.result_list.end(), std::back_inserter(new_vector));
    this->result_list.clear();
    this->result_list.assign(new_vector.begin(), new_vector.end());
    return *this;
}

QueryBase& TextQuery::operator|(const QueryBase &q){
    std::vector<Result> new_vector;
    std::set_union(this->result_list.begin(), this->result_list.end(), q.result_list.begin(), q.result_list.end(), std::back_inserter(new_vector));
    this->result_list.clear();
    this->result_list.assign(new_vector.begin(), new_vector.end());

    return *this;
}


void Query::parseExpr(const std::string &expr){
    std::istringstream istrm;
    istrm.str(expr);
    std::string command;
    bool next_should_operator = false;
    while(istrm >> command){
        if(next_should_operator){
            if(command == "&" or
               command == "|"){
                commands.push(command);
                next_should_operator = false;
            } else {
                throw std::runtime_error("error expr: '"+expr+"' need '&' or '|' operator.");
            }
        } else {
            if(command == "&" or
               command == "|"){
                throw std::runtime_error("error expr: '"+expr+"' can't allow '&' or '|' here.");
            } else {
                commands.push(command);
                next_should_operator = true;
            }
        }
    }
    if(commands.size() % 2 == 0){
        throw std::runtime_error("error: '&' or '|' must have a right operator!");
    }
}

void Query::evalCommands(std::ifstream& in){
    if(commands.empty()){
        return;
    }
    TextQuery tq;
    while(!commands.empty()){
        auto command = commands.front();
        if(command == "&"){
            commands.pop();
            auto right_command = commands.front();
            TextQuery add_q;
            add_q.occursInFile(in, right_command);
            tq & add_q;
        } else if(command == "|"){
            commands.pop();
            auto right_command = commands.front();
            TextQuery or_q;
            or_q.occursInFile(in, right_command);
            tq | or_q;
        } else {
            tq.occursInFile(in, command);
        }
        commands.pop();
    }
    std::cout<<"occurs ";
    std::cout<<std::to_string(tq.result_list.size());
    std::cout<<" times:"<<'\n';
                          auto begin = tq.result_list.begin();
                          auto end = tq.result_list.end();
               while(begin != end){
                   Result& r = *begin;
                   std::cout<<"["<<std::to_string(r.line_number);
                   std::cout<<"] "<<r.line_content<<'\n';
                   ++begin;
               }
}


static const std::string FILE_NAME = "/home/nnero/tmp/tq_letter.txt";

int main(){
    std::ifstream in;
    in.open(FILE_NAME, std::ifstream::in);
    if(in){//success
        Query query;
        while(true){
            std::string line;
            std::getline(std::cin,line);
            query.parseExpr(line);
            query.evalCommands(in);
        }
    } else {
        std::cout<<"File not exists!"<<std::endl;
    }
    return 0;
}

