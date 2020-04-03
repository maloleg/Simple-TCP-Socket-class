#include "main.cpp"

int main(){
    ServerSocket a;

    a.Bind();
    a.Listen();

    while (1){
//        a << "Hello world!";
        a.Shutdown();
    }
}