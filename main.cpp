#include "OST_Tree.h"
int main(){
    OST_Tree<float> alfa;
    alfa.anadir(1.0,2.0);
    alfa.anadir(-4.0,6.0);
    alfa.anadir(1.5,0.0);
    alfa.anadir(0.0,0.0);
    alfa.anadir(-4.5,8.0);
    alfa.anadir(0.4,3.0);
    alfa.anadir(-3.5,1.0);
    cout<<"Holamundo"<<endl;
    alfa.imprimir();
    return 0;
}
