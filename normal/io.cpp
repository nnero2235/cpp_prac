#include<unistd.h>
#include<cstdio>

int main(){
    char buf[32] = {"nice"};
    ::snprintf(stdout, 32, buf);
    return 0;
}
