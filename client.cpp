#include "main.cpp"


int main(){
        //std::cout << "Hello, World!" << std::endl;
        ClientSocket a;

        char* Text = "Hello aa world!";

        while (1) {
            a.Connect();
            a.Read(Text, 5);
        }
}