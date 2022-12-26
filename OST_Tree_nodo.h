#define OST_TREE_NODO
#ifdef OST_TREE_NODO
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;

template <typename T>
class OST_Tree_nodo{
private:
    T* VBR;
    T* MBR;
    int estado;
    vector<OST_Tree_nodo*> hijos;
    OST_Tree_nodo* padre;
public:
    OST_Tree_nodo();
    bool dentro(T*,T*);
    T calcular_area(T*);
    T calcular_superposicion(T*,T*);
    void anadir(T*,T*,int,int);
    void dividir(OST_Tree_nodo<T>*,int);
    void actualizar(OST_Tree_nodo<T>*,OST_Tree_nodo<T>*);
    void imprimir();
    ~OST_Tree_nodo();
};

template <typename T>
OST_Tree_nodo<T>::OST_Tree_nodo(){
    VBR = nullptr;
    MBR = nullptr;
    padre = nullptr;
    estado = 0;
}

template <typename T>
bool OST_Tree_nodo<T>::dentro(T*area, T* objeto){
    if(area[0] <= objeto[0] && objeto[1] <= area[1] && area[2] <= objeto[2] && objeto[3] <= area[3])
        return true;
    return false;
}

template <typename T>
T OST_Tree_nodo<T>::calcular_area(T*campo){
    T area = abs((campo[1] - campo[0]) *(campo[3] - campo[2]));
    return area;
}

template <typename T>
T OST_Tree_nodo<T>::calcular_superposicion(T*i, T* j){
    T x_valor = 0;
    if(i[0] < j[0] && j[0] < i[1]){
        x_valor = i[1] - j[0];
    }
    else if(i[0] < j[1] && j[1] < i[1]){
        x_valor = j[1] - i[0];
    }
    else{
        return x_valor;
    }
    T y_valor = 0;
    if(i[2] < j[2] && j[2] < i[3]){
        y_valor = i[3] - j[2];
    }
    else if(i[2] < j[3] && j[3] < i[3]){
        y_valor = j[3] - i[2];
    }
    else{
        return y_valor;
    }
    return y_valor*x_valor;
}

template <typename T>
void OST_Tree_nodo<T>::anadir(T* nuevo_VBR,T* nuevo_MBR,int tamano, int maximo){
    OST_Tree_nodo<T> *tmp = this;
    while (!tmp->hijos.empty()){
        if(tmp->estado == 1){
            break;
        }
        else{
            T superposicion;
            vector<pair<T,OST_Tree_nodo<T>*>> resultados;
            T nuevos_datos[4];
            for (int i = 0; i < tmp->hijos.size(); i++){
                T i_superposicion = 0;
                nuevos_datos[0] = min(tmp->hijos[i]->MBR[0],nuevo_MBR[0]);
                nuevos_datos[1] = max(tmp->hijos[i]->MBR[1],nuevo_MBR[1]);
                nuevos_datos[2] = min(tmp->hijos[i]->MBR[2],nuevo_MBR[2]);
                nuevos_datos[3] = max(tmp->hijos[i]->MBR[3],nuevo_MBR[3]);
                for (int j = 0; j < tmp->hijos.size(); j++)
                    if(i != j)
                        i_superposicion+=calcular_superposicion(tmp->hijos[i]->MBR,tmp->hijos[j]->MBR);
                if(resultados.empty() || i_superposicion <= superposicion){
                    if(resultados.empty() || i_superposicion < superposicion)
                        resultados.clear();
                    T nueva_area = this->calcular_area(nuevos_datos);
                    resultados.push_back({nueva_area,tmp->hijos[i]});
                }
            }
            sort(resultados.begin(),resultados.end());
            tmp = resultados[0].second;
        }
    }
    
    OST_Tree_nodo<T> *nuevo_objeto = new OST_Tree_nodo<T>();
    nuevo_objeto->MBR = new T[4];
    nuevo_objeto->VBR = new T[4];
    nuevo_objeto->estado = 2;
    for (int i = 0; i < 4; i++){
        nuevo_objeto->MBR[i] = nuevo_MBR[i];
        nuevo_objeto->VBR[i] = nuevo_VBR[i];
    }
    if(tmp->estado == 0){
        OST_Tree_nodo<T> *nuevo_nodo = new OST_Tree_nodo<T>();
        nuevo_nodo->estado = 1;
        tmp->hijos.push_back(nuevo_nodo);
        int ultimo = tmp->hijos.size()-1;
        tmp->hijos[ultimo]->padre = tmp;
        tmp = tmp->hijos[ultimo];
    }
    tmp->hijos.push_back(nuevo_objeto);
    nuevo_objeto->padre = tmp;
    if(tmp->hijos.size() <= maximo)
        this->actualizar(tmp,nuevo_objeto);
    else
        this->dividir(tmp,tamano);
}

template <typename T>
void OST_Tree_nodo<T>::dividir(OST_Tree_nodo<T>*tmp,int tamano){
    while (1){
        OST_Tree_nodo<T>* a = new OST_Tree_nodo<T>();
        OST_Tree_nodo<T>* b = new OST_Tree_nodo<T>();
        bool vacio = true;
        T superposicion;
        T area;
        T margen;
        vector<vector<int>> resultados;
        vector<int> a_1;
        vector<int> b_1;
        T nuevos_datos_1[4];
        T nuevos_datos_2[4];
        for (int i = 0; i < 4; i++){
            if(i%2 == 0){
                nuevos_datos_1[i] = INT16_MAX;
                nuevos_datos_2[i] = INT16_MAX;
            }
            else{
                nuevos_datos_1[i] = INT16_MIN;
                nuevos_datos_2[i] = INT16_MIN;
            }
        }
        for (int i = 0; i < tmp->hijos.size(); i++){
            for (int j = 0; j < 4; j++){
                T eje = tmp->hijos[i]->MBR[j];
                for (int k = 0; k < tmp->hijos.size(); k++){
                    if(tmp->hijos[k]->MBR[j] < eje){
                        a_1.push_back(k);
                        nuevos_datos_1[0] = min(nuevos_datos_1[0],tmp->hijos[k]->MBR[0]);
                        nuevos_datos_1[1] = max(nuevos_datos_1[1],tmp->hijos[k]->MBR[1]);
                        nuevos_datos_1[2] = min(nuevos_datos_1[2],tmp->hijos[k]->MBR[2]);
                        nuevos_datos_1[3] = max(nuevos_datos_1[3],tmp->hijos[k]->MBR[3]);
                    }
                    else{
                        b_1.push_back(k);
                        nuevos_datos_2[0] = min(nuevos_datos_2[0],tmp->hijos[k]->MBR[0]);
                        nuevos_datos_2[1] = max(nuevos_datos_2[1],tmp->hijos[k]->MBR[1]);
                        nuevos_datos_2[2] = min(nuevos_datos_2[2],tmp->hijos[k]->MBR[2]);
                        nuevos_datos_2[3] = max(nuevos_datos_2[3],tmp->hijos[k]->MBR[3]);
                    }
                }
                //Calculo del margen
                T nuevo_margen = b_1.size() -  a_1.size();
                if(a_1.size() > b_1.size())
                    nuevo_margen = a_1.size() - b_1.size();
                    
                if(vacio){
                    margen = nuevo_margen;
                    superposicion = this->calcular_superposicion(nuevos_datos_1,nuevos_datos_2);
                    if(tmp->padre!=nullptr){
                        OST_Tree_nodo<T>*f = tmp->padre;
                        for (int k = 0; k < f->hijos.size(); k++){
                            if(f->hijos[k] != tmp){
                                superposicion += this->calcular_superposicion(f->hijos[k]->MBR,nuevos_datos_1);
                                superposicion += this->calcular_superposicion(f->hijos[k]->MBR,nuevos_datos_2);
                            }
                        }
                    }
                    area = this->calcular_area(nuevos_datos_1);
                    area += this->calcular_area(nuevos_datos_2);
                    resultados.push_back(a_1);
                    resultados.push_back(b_1);
                    vacio = false;
                }
                else if(nuevo_margen == margen){
                    //Calculo de superposicion
                    T nuevo_superposicion = this->calcular_superposicion(nuevos_datos_1,nuevos_datos_2);
                    if(tmp->padre!=nullptr){
                        OST_Tree_nodo<T>*f = tmp->padre;
                        for (int k = 0; k < f->hijos.size(); k++){
                            if(f->hijos[k] != tmp){
                                nuevo_superposicion += this->calcular_superposicion(f->hijos[k]->MBR,nuevos_datos_1);
                                nuevo_superposicion += this->calcular_superposicion(f->hijos[k]->MBR,nuevos_datos_2);
                            }
                        }
                    }
                    
                    if(nuevo_superposicion == superposicion){
                        //Calculo de area
                        T nueva_area = this->calcular_area(nuevos_datos_1);
                        nueva_area += this->calcular_area(nuevos_datos_2);
                        
                        if(nueva_area < area){
                            area = nueva_area;
                            resultados.clear();
                            resultados.push_back(a_1);
                            resultados.push_back(b_1);
                        }
                        //Fin calculo de area
                    }
                    else if(nuevo_superposicion < nuevo_superposicion){
                        superposicion = nuevo_superposicion;
                        area = this->calcular_area(nuevos_datos_1);
                        area += this->calcular_area(nuevos_datos_2);
                        resultados.clear();
                        resultados.push_back(a_1);
                        resultados.push_back(b_1);
                    }
                    //Fin calculo de superposicion
                }
                else if(nuevo_margen < margen){
                    margen = nuevo_margen;
                    superposicion = this->calcular_superposicion(nuevos_datos_1,nuevos_datos_2);
                    if(tmp->padre!=nullptr){
                        OST_Tree_nodo<T>*f = tmp->padre;
                        for (int k = 0; k < f->hijos.size(); k++){
                            if(f->hijos[k] != tmp){
                                superposicion += this->calcular_superposicion(f->hijos[k]->MBR,nuevos_datos_1);
                                superposicion += this->calcular_superposicion(f->hijos[k]->MBR,nuevos_datos_2);
                            }
                        }
                    }
                    area = this->calcular_area(nuevos_datos_1);
                    area += this->calcular_area(nuevos_datos_2);
                    resultados.clear();
                    resultados.push_back(a_1);
                    resultados.push_back(b_1);
                }
                //Fin calculo del margen
                a_1.clear();
                b_1.clear();
            }
        }
        for (int i = 0; i < resultados[0].size(); i++){
            cout<<resultados[0][i]<<endl;
            a->hijos.push_back(tmp->hijos[resultados[0][i]]);
            this->actualizar(a,tmp->hijos[resultados[0][i]]);
        }
        for (int i = 0; i < resultados[1].size(); i++){
            cout<<resultados[1][i]<<endl;
            b->hijos.push_back(tmp->hijos[resultados[1][i]]);
            this->actualizar(b,tmp->hijos[resultados[1][i]]);
        }
        if(tmp->estado == 1){
            a->estado = 1;
            b->estado = 1;
        }
        if(tmp->padre == nullptr){
            tmp->hijos.clear();
            tmp->hijos.push_back(a);
            a->padre = tmp;
            tmp->hijos.push_back(b);
            b->padre = tmp;
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
            if(f != this)
                this->actualizar(f,a);
            f->hijos.push_back(b);
            if(f->hijos.size() > tamano){
                tmp = f;
            }
            else{
                if(f != this)
                    this->actualizar(f,b);
                break;
            }
        }
            /*
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
            a->estado = 1;
            b->estado = 1;
            OST_Tree_nodo<T>* f = tmp->padre;
            for (int j = 0; j < f->hijos.size(); j++){
                if(f->hijos[j] == tmp){
                    delete f->hijos[j];
                    f->hijos[j] = a;
                    break;
                }
            }
            if(f != this)
                this->actualizar(f,a);
            f->hijos.push_back(b);
            if(f->hijos.size() > tamano){
                tmp = f;
            }
            else{
                if(f != this)
                    this->actualizar(f,b);
                break;
            }
        }*/
    }
}

template <typename T>
void OST_Tree_nodo<T>::actualizar(OST_Tree_nodo<T>*tmp,OST_Tree_nodo<T>*cambiado){
    bool cambio = true;
    OST_Tree_nodo<T>* ant = cambiado;
    while (tmp != nullptr && tmp != this && cambio){
        cambio = false;
        if(tmp->MBR == nullptr){
            tmp->MBR = new T[4];
            tmp->MBR[0] = INT16_MAX;
            tmp->MBR[1] = INT16_MIN;
            tmp->MBR[2] = INT16_MAX;
            tmp->MBR[3] = INT16_MIN;
            tmp->VBR = new T[4];
            tmp->VBR[0] = 0;
            tmp->VBR[1] = 0;
            tmp->VBR[2] = 0;
            tmp->VBR[3] = 0;
        }
        for (int i = 0; i < 4; i++){
            int a = 0,b = 1;
            if(i > 1){
                a = 2;
                b = 3;
            }
            if(ant->VBR[i] < 0){
                if(tmp->VBR[a] > ant->VBR[i]){
                    tmp->VBR[a] = ant->VBR[i];
                    cambio = true;
                }
            }
            else{
                if(tmp->VBR[b] < ant->VBR[i]){
                    tmp->VBR[b] = ant->VBR[i];
                    cambio = true;
                }
            }
        }
        if(tmp->MBR[0] > ant->MBR[0]){
            tmp->MBR[0] = ant->MBR[0];
            cambio = true;
        }
        if(tmp->MBR[1] < ant->MBR[1]){
            tmp->MBR[1] = ant->MBR[1];
            cambio = true;
        }
        if(tmp->MBR[2] > ant->MBR[2]){
            tmp->MBR[2] = ant->MBR[2];
            cambio = true;
        }
        if(tmp->MBR[3] < ant->MBR[3]){
            tmp->MBR[3] = ant->MBR[3];
            cambio = true;
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
        if(tmp->estado == 0){
            if(tmp != this){
                cout<<"{";
                for (int i = 0; i < 4; i++){
                    cout<<tmp->MBR[i];
                    if(i != 3)
                        cout<<" , ";
                }
                cout<<"}";
                cout<<"{";
                for (int i = 0; i < 4; i++){
                    cout<<tmp->VBR[i];
                    if(i != 3)
                        cout<<" , ";
                }
                cout<<"}";
            }
            else{
                cout<<"RAIZ";
            }
            cout<<": {";
            for (int i = 0; i < tmp->hijos.size(); i++){
                cout<<"(";
                for (int j = 0; j < 4; j++){
                    cout<<tmp->hijos[i]->MBR[j];
                    if(j != 3)
                        cout<<" , ";
                }
                cout<<")";
                if(i < tmp->hijos.size()-1){
                    cout<<" ; ";
                }
                impresion.push(tmp->hijos[i]);
            }
            cout<<"}"<<endl<<endl;
        }
        else{
            cout<<"{";
            for (int i = 0; i < 4; i++)
            {
                cout<<tmp->MBR[i];
                if(i != 3)
                    cout<<" , ";
            }
            cout<<"}";
            cout<<"{";
            for (int i = 0; i < 4; i++){
                cout<<tmp->VBR[i];
                if(i != 3)
                    cout<<" , ";
            }
            cout<<"}: {";
            for (int i = 0; i < tmp->hijos.size(); i++){
                cout<<"[(";
                for (int j = 0; j < 4; j++){
                    cout<<tmp->hijos[i]->MBR[j];
                    if(j != 3)
                        cout<<" , ";
                }
                cout<<")(";
                for (int j = 0; j < 4; j++)
                {
                    cout<<tmp->hijos[i]->VBR[j];
                    if(j != 3)
                        cout<<" , ";
                }
                cout<<")]";
                if(i < tmp->hijos.size()-1){
                    cout<<" ; ";
                }
            }
            cout<<"}"<<endl<<endl;
        }
    }
    
    
}
template <typename T>
OST_Tree_nodo<T>::~OST_Tree_nodo()
{
}
#endif