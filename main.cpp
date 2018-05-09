#include "nnero.h"

int main(){
    std::string myName ="nnero";
    Screen s(10,10);
    Screen s1(10,10,"NNERO");
    s.display();
    s.setX(90).setY(90).display();
    s1.display();
    s1.setX(100).setY(100).display();

    s1.setContents(myName);
    s1.display();
    std::cout<<myName<<std::endl;

    Screen* s3 = &s;
    s3->display();
    std::cout<<&s<<std::endl;
    std::cout<<&s1<<std::endl;
    std::cout<<s3<<std::endl;
    return 0;
}
