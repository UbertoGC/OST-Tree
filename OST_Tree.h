#define OST_TREE
#ifdef OST_TREE
#include "OST_Tree_nodo.h"
template <typename T>
class OST_Tree{
private:
    OST_Tree_nodo<T> *raiz;
    int maximo_puntos;
    int size_n;
public:
    OST_Tree();
    OST_Tree(int, int);
    void anadir(T*,T*);
    int size_nodo();
    void imprimir();
    ~OST_Tree();
};
template <typename T>
OST_Tree<T>::OST_Tree(){
    maximo_puntos = 3;
    raiz=new OST_Tree_nodo<T>();
    size_n = 3;
}
template <typename T>
OST_Tree<T>::OST_Tree(int a, int m){
    maximo_puntos = m;
    size_n = a;
    raiz=new OST_Tree_nodo<T>();
}
template <typename T>
void OST_Tree<T>::anadir(T*nuevo_VBR,T*nuevo_MBR){
    raiz->anadir(nuevo_VBR,nuevo_MBR,size_n,maximo_puntos);
}
template <typename T>
int OST_Tree<T>::size_nodo(){
    return size_n;
}
template <typename T>
void OST_Tree<T>::imprimir(){
    raiz->imprimir();
}
template <typename T>
OST_Tree<T>::~OST_Tree()
{
}

#endif