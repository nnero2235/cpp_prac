#include "cat.h"
#include "util.h"
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<exception>
#include<stdexcept>

Cat::Cat(const std::string& fileName,int lines){
    this->fileName = fileName;
    this->lines = lines;
}

void Cat::display()const{
    std::fstream in(this->fileName);
    char buf[512];
    int lineNumber = 0;
    std::vector<std::string> contents;
    while(in.getline(buf,512)){
        lineNumber++;
        std::string line(std::to_string(lineNumber));
        line.append(" ");
        line.append(buf);
        contents.push_back(line);
    }
    printVector(contents);
}


int main(int argc,char** argv){
    if(argc < 2){
        std::cout<<"usage: cat [-l] [lines] [fileLocation]."<<std::endl;
        exit(0);
    }
    bool lineFlag = false;
    int lines = -1;
    std::string fileName;
    while(*++argv){
        const std::string arg(*argv);
        if(arg.compare("-i") == 0){
            lineFlag = true;
        } else if(lineFlag){
            try{
                lines = stoi(arg);
            } catch(std::runtime_error& e) {
                std::cout<<"unknow arg: "<<arg<<std::endl;
                exit(0);
            }
            lineFlag = false;
        } else{
            fileName = arg;
            break;
        }
    }
    if(fileName.size() == 0){
        std::cout<<"error: file is not specified"<<std::endl;
    }
    Cat cat(fileName,lines);
    cat.display();
    return 0;
}
