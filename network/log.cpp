#include<nnero/common.hpp>

using namespace nnero::logger;

int main(){
    LOG(INFO)<<"hello"<<9<<'\n';
    LOG(WARN)<<"hello"<<10.8<<'\n';
    LOG(ERROR)<<"hello"<<'c'<<'\n';
    return 0;
}
