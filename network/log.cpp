#include <memory>
#include <iostream>
#include <iomanip>
#include <thread>
#include <fstream>
#include <sstream>

int main(){
    std::ostringstream ostr;
    ostr<<"like"<<":"<<1<<1.2<<"I like %s aha"<<"i"<<'\n';
    std::ofstream ofs;
    //ofs.open("/home/nnero/project/cplusplus/cpp_prac/network/p.log", std::ios_base::app);
    //ofs<<ostr.str();
    std::cout<<ostr.str();
    return 0;
}
