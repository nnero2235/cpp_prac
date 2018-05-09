#include<fstream>
#include<string>
#include<iostream>

int main(){
    std::string sourceFile = "/home/nnero/orgs/todo-list.org";
    std::string targetFile = "/home/nnero/orgs/todo-list_cp.org";
    std::ifstream in(sourceFile);
    std::ofstream out;
    if(in){
        out.open(targetFile, std::ios::out);
        char buf[256];
        //int len = 0;
        while(!in.eof()){
            in >> buf;
            out << buf <<std::endl;
        }
        out.close();
    } else {
        std::cout<<"file not found!"<<std::endl;
    }
    return 0;
}
