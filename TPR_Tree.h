#define TPR_TREE
#ifdef TPR_TREE
#include "TPR_Tree_nodo.h"
template <typename T>
class TPR_Tree{
private:
    TPR_Tree_nodo<T> *raiz;
    int maximo_puntos;
    int size_n;
public:
    TPR_Tree();
    TPR_Tree(int, int);
    void anadir(T*,T*,int);
    void buscar(T*,int);
    void eliminar(T*,int);
    int size_nodo();
    void imprimir();
    ~TPR_Tree();
};
template <typename T>
TPR_Tree<T>::TPR_Tree(){
    maximo_puntos = 3;
    raiz=new TPR_Tree_nodo<T>();
    size_n = 3;
}
template <typename T>
TPR_Tree<T>::TPR_Tree(int a, int m){
    maximo_puntos = m;
    size_n = a;
    raiz=new TPR_Tree_nodo<T>();
}
template <typename T>
void TPR_Tree<T>::anadir(T*nuevo_VBR,T*nuevo_MBR,int tiempo){
    raiz->anadir(nuevo_VBR,nuevo_MBR,size_n,maximo_puntos,tiempo);
}
template <typename T>
void TPR_Tree<T>::buscar(T*region, int tiempo){
    raiz->buscar(region,tiempo);
}
template <typename T>
void TPR_Tree<T>::eliminar(T*region, int tiempo){
    raiz->eliminar(region,size_n,maximo_puntos,tiempo);
}
template <typename T>
int TPR_Tree<T>::size_nodo(){
    return size_n;
}
template <typename T>
void TPR_Tree<T>::imprimir(){
    raiz->imprimir();
}
template <typename T>
TPR_Tree<T>::~TPR_Tree()
{
}

#endif