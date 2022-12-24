#define OST_TREE_NODO
#ifdef OST_TREE_NODO
#include <iostream>
#include <vector>
#include <queue>
#include "punto.h"
using namespace std;
template <typename T>
class OST_Tree_nodo{
private:
    pair<punto<T>,punto<T>> limite;
    vector<punto<T>> valores;
    vector<OST_Tree_nodo*> hijos;
    OST_Tree_nodo* padre;

public:
    OST_Tree_nodo();
    bool dentro(OST_Tree_nodo<T>*, punto<T>*);
    void anadir(T,T,int,int);
    void dividir(OST_Tree_nodo<T>*,int);
    void actualizar(OST_Tree_nodo<T>*,OST_Tree_nodo<T>*);
    void imprimir();
    ~OST_Tree_nodo();
};

template <typename T>
OST_Tree_nodo<T>::OST_Tree_nodo(){
    punto<T> *punto_nulo=new punto<T>();
    limite = {*punto_nulo,*punto_nulo};
    limite.first.x = INT16_MAX;
    limite.first.y = INT16_MAX;
    limite.second.x = INT16_MIN;
    limite.second.y = INT16_MIN;
    padre = nullptr;
    delete punto_nulo;
}

template <typename T>
bool OST_Tree_nodo<T>::dentro(OST_Tree_nodo<T>*tmp, punto<T>* alf){
    if((tmp->limite.first < *alf) && (*alf <= (tmp->limite.second)))
        return true;
    return false;
}

template <typename T>
void OST_Tree_nodo<T>::anadir(T nuevo_x,T nuevo_y,int tamano, int maximo){
    punto<T> *nuevo_punto=new punto<T>(nuevo_x,nuevo_y);
    OST_Tree_nodo<T> *tmp = this;
    int i = 0;
    while (!tmp->hijos.empty()){
        if(i < tmp->hijos.size()){
            if(tmp->hijos[i]->limite.first.nulo()){
                break;
            }
            else if(this->dentro(tmp->hijos[i],nuevo_punto)){
                tmp = tmp->hijos[i];
                i = 0;
            }
            else{
                i++;
            }
        }
        else{
            break;
        }
    }
    if(i == tmp->hijos.size()){
        OST_Tree_nodo<T> *nuevo_nodo = new OST_Tree_nodo<T>();
        tmp->hijos.push_back(nuevo_nodo);
        tmp->hijos[i]->padre = tmp;
    }
    tmp->hijos[i]->valores.push_back(*nuevo_punto);
    if(tmp->hijos[i]->valores.size() <= maximo)
        this->actualizar(tmp->hijos[i],nullptr);
    else
        this->dividir(tmp->hijos[i],tamano);
}

template <typename T>
void OST_Tree_nodo<T>::dividir(OST_Tree_nodo<T>*tmp,int tamano){
    while (1){
        OST_Tree_nodo<T>* a = new OST_Tree_nodo<T>();
        OST_Tree_nodo<T>* b = new OST_Tree_nodo<T>();
        if(tmp->valores.empty()){
            for (int i = 0; i < tmp->hijos.size(); i++){
                if(i < tmp->hijos.size()/2){
                    a->hijos.push_back(tmp->hijos[i]);
                    this->actualizar(a,tmp->hijos[i]);
                }
                else{
                    b->hijos.push_back(tmp->hijos[i]);
                    this->actualizar(b,tmp->hijos[i]);
                }
            }
            if(tmp->padre == nullptr){
                tmp->hijos.clear();
                tmp->hijos.push_back(a);
                tmp->hijos.push_back(b);
                break;
            }
            else{
                OST_Tree_nodo<T>* f = tmp->padre;
                for (int j = 0; j < f->hijos.size(); j++){
                    if(f->hijos[j] == tmp){
                        delete f->hijos[j]; 
                        f->hijos[j] = a;
                        break;
                    }
                }
                this->actualizar(f,a);
                f->hijos.push_back(b);
                if(f->hijos.size() > tamano){
                    tmp = f;
                }
                else{
                    this->actualizar(f,b);
                    break;
                }
            }
        }
        else{
            for (int i = 0; i < tmp->valores.size(); i++){
                if(i < tmp->valores.size()/2){
                    a->valores.push_back(tmp->valores[i]);
                    this->actualizar(a,nullptr);
                }
                else{
                    b->valores.push_back(tmp->valores[i]);
                    this->actualizar(b,nullptr);
                }
            }
            OST_Tree_nodo<T>* f = tmp->padre;
            for (int j = 0; j < f->hijos.size(); j++){
                if(f->hijos[j] == tmp){
                    delete f->hijos[j]; 
                    f->hijos[j] = a;
                    break;
                }
            }
            this->actualizar(f,a);
            f->hijos.push_back(b);
            if(f->hijos.size() > tamano){
                tmp = f;
            }
            else{
                this->actualizar(f,b);
                break;
            }
        }
    }
}

template <typename T>
void OST_Tree_nodo<T>::actualizar(OST_Tree_nodo<T>*tmp,OST_Tree_nodo<T>*cambiado){
    bool cambio = true;
    OST_Tree_nodo<T>* ant = cambiado;
    while (tmp != nullptr && tmp != this && cambio){
        cambio = false;
        if(tmp->valores.size() == 1){
            tmp->limite.first.x = tmp->valores[0].x;
            tmp->limite.first.y = tmp->valores[0].y;
        }
        else if(tmp->valores.size() == 2 && tmp->limite.first.nulo()){
            if(tmp->valores[0].x < tmp->valores[1].x){
                tmp->limite.first.x = tmp->valores[0].x; 
                tmp->limite.second.x = tmp->valores[1].x;
            }
            else{
                tmp->limite.first.x = tmp->valores[1].x; 
                tmp->limite.second.x = tmp->valores[0].x;
            }
            if(tmp->valores[0].y < tmp->valores[1].y){
                tmp->limite.first.y = tmp->valores[0].y; 
                tmp->limite.second.y = tmp->valores[1].y;
            }
            else{
                tmp->limite.first.y = tmp->valores[1].y; 
                tmp->limite.second.y = tmp->valores[0].y;
            }
            tmp->limite.first.vacio = false;
            tmp->limite.second.vacio = false;
        }
        else if(tmp->valores.size() > 1){
            int ultimo=tmp->valores.size() - 1;
            if(tmp->limite.first.x > tmp->valores[ultimo].x){
                tmp->limite.first.x = tmp->valores[ultimo].x;
                cambio = true;
            }
            if(tmp->limite.first.y > tmp->valores[ultimo].y){
                tmp->limite.first.y = tmp->valores[ultimo].y;
                cambio = true;
            }
            if(tmp->limite.second.x < tmp->valores[ultimo].x){
                tmp->limite.second.x = tmp->valores[ultimo].x;
                cambio = true;
            }
            if(tmp->limite.second.y < tmp->valores[ultimo].y){
                tmp->limite.second.y = tmp->valores[ultimo].y;
                cambio = true;
            }
        }
        else{
            if(ant->limite.first.x > tmp->limite.first.x){
                ant->limite.first.x = tmp->limite.first.x;
                cambio = true;
            }
            if(ant->limite.first.y > tmp->limite.first.y){
                ant->limite.first.y = tmp->limite.first.y;
                cambio = true;
            }
            if(ant->limite.second.x < tmp->limite.second.x){
                ant->limite.second.x = tmp->limite.second.x;
                cambio = true;
            }
            if(ant->limite.second.y < tmp->limite.second.y){
                ant->limite.second.y = tmp->limite.second.y;
                cambio = true;
            }
        }
        ant = tmp;
        tmp = tmp->padre;
    }
}

template <typename T>
void OST_Tree_nodo<T>::imprimir(){
    OST_Tree_nodo<T>*tmp = this;
    queue<OST_Tree_nodo<T>*> impresion;
    impresion.push(tmp);
    while (!impresion.empty()){
        tmp = impresion.front();
        impresion.pop();
        if(!tmp->limite.first.nulo()){
            cout << "{(" << tmp->limite.first.x << " | "<< tmp->limite.first.y << ") | ";
            cout << "(" << tmp->limite.second.x << " | "<< tmp->limite.second.y << ")} : ";
            if(!tmp->hijos.empty()){
                for (auto i : tmp->hijos){
                    cout << "{(" << i->limite.first.x << " | "<< i->limite.first.y << ") | ";
                    cout << "(" << i->limite.second.x << " | "<< i->limite.second.y << ")} , ";
                    impresion.push(i);
                }
                cout<<endl;
            }
            else{
                cout << "[";
                for (auto i : tmp->valores){
                    cout << "(" << i.x << " | "<< i.y << ") , ";
                }
                cout<<"]"<<endl;
            }
        }
        else{
            if(tmp != this){
                cout << "{";
                for (auto i : tmp->valores){
                    cout << "(" << i.x << " | "<< i.y << ") , ";
                }
                cout<<"}"<<endl;
            }
            for (auto i : tmp->hijos){
                impresion.push(i);
            }
        }
    }
    
    
}
template <typename T>
OST_Tree_nodo<T>::~OST_Tree_nodo()
{
}
#endif