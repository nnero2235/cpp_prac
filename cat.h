// -*-c++-*-
#ifndef CAT_H
#define CAT_H
#include<string>


class Cat{
public:
    Cat(const std::string& fileName,int lines);
    void display()const;
private:
    std::string fileName;
    int lines = -1;
};

#endif
