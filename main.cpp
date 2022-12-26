#include "OST_Tree.h"
int main(){
    OST_Tree<int> alfa;
    int continuar;
    int datos_MBR[4] ={2,4,3,4};
    int datos_VBR[4] = {1,1,1,1};
    alfa.anadir(datos_VBR,datos_MBR);
    alfa.imprimir();
    cout<<"continuar?(0: no, 1: si)"<<endl;
    cin>>continuar;
    while (continuar){
        cout<<"datos VBR:"<<endl;
        for (int i = 0; i < 4; i++)
        {
            cin>>datos_VBR[i];
        }
        cout<<"datos MBR:"<<endl;
        for (int i = 0; i < 4; i++)
        {
            cin>>datos_MBR[i];
        }
        alfa.anadir(datos_VBR,datos_MBR);
        alfa.imprimir();
        cout<<"continuar?(0: no, 1: si)"<<endl;
        cin>>continuar;
    }
    return 0;
}
