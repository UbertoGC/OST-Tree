#define TPR_TREE_NODO
#ifdef TPR_TREE_NODO
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;

template <typename T>
class TPR_Tree_nodo{
private:
    T* VBR;
    T* MBR;
    int estado;
    vector<TPR_Tree_nodo*> hijos;
    TPR_Tree_nodo* padre;
public:
    TPR_Tree_nodo();
    bool dentro(T*,T*);
    T calcular_area(T*);
    T calcular_superposicion(T*,T*);
    void anadir(T*,T*,int,int,int);
    void buscar(T*,int);
    void eliminar(T*,int,int,int);
    void paso_de_tiempo(int);
    void dividir(TPR_Tree_nodo<T>*,int,int);
    void actualizar(TPR_Tree_nodo<T>*,TPR_Tree_nodo<T>*,bool);
    void imprimir();
    ~TPR_Tree_nodo();
};

template <typename T>
TPR_Tree_nodo<T>::TPR_Tree_nodo(){
    VBR = nullptr;
    MBR = nullptr;
    padre = nullptr;
    estado = 0;
}

template <typename T>
bool TPR_Tree_nodo<T>::dentro(T*area, T* objeto){
    if((area[0] <= objeto[0]) && (objeto[1] <= area[1]) && (area[2] <= objeto[2]) && (objeto[3] <= area[3]))
        return true;
    return false;
}

template <typename T>
T TPR_Tree_nodo<T>::calcular_area(T*campo){
    T area = abs((campo[1] - campo[0]) *(campo[3] - campo[2]));
    return area;
}

template <typename T>
T TPR_Tree_nodo<T>::calcular_superposicion(T*i, T* j){
    if(this->dentro(j,i)){
        return this->calcular_area(i);
    }
    T x_valor = 0;
    if(i[0] <= j[0] && j[0] <= i[1]){
        x_valor = min(j[1],i[1]) - j[0];
    }
    else if(i[0] <= j[1] && j[1] <= i[1]){
        x_valor = j[1] - i[0];
    }
    T y_valor = 0;
    if(i[2] <= j[2] && j[2] <= i[3]){
        y_valor = min(i[3],j[3]) - j[2];
    }
    else if(i[2] <= j[3] && j[3] <= i[3]){
        y_valor = j[3] - i[2];
    }
    T resultado = abs(y_valor*x_valor);
    x_valor = 0;
    if(j[0] <= i[0] && i[0] <= j[1]){
        x_valor = min(i[1],j[1]) - i[0];
    }
    else if(j[0] <= i[1] && i[1] <= j[1]){
        x_valor = i[1] - j[0];
    }
    y_valor = 0;
    if(j[2] <= i[2] && i[2] <= j[3]){
        y_valor = min(j[3],i[3]) - i[2];
    }
    else if(j[2] <= i[3] && i[3] <= j[3]){
        y_valor = i[3] - j[2];
    }
    resultado = max(resultado,abs(y_valor*x_valor));
    return resultado;
}

template <typename T>
void TPR_Tree_nodo<T>::paso_de_tiempo(int tiempo){
    if(tiempo <= 0)
        return;
    TPR_Tree_nodo<T> *tmp = this;
    queue<TPR_Tree_nodo<T>*> actualizacion;
    actualizacion.push(tmp);
    while (!actualizacion.empty()){
        TPR_Tree_nodo<T> *arbol=actualizacion.front();
        actualizacion.pop();
        if(arbol != this){
            for (int i = 0; i < 4; i++){
                arbol->MBR[i] += (arbol->VBR[i] * tiempo);
            }
        }
        for (int i = 0; i < arbol->hijos.size(); i++){
            actualizacion.push(arbol->hijos[i]);
        }
    }
}

template <typename T>
void TPR_Tree_nodo<T>::anadir(T* nuevo_VBR,T* nuevo_MBR,int tamano, int maximo, int tiempo){
    this->paso_de_tiempo(tiempo);
    TPR_Tree_nodo<T> *tmp = this;
    while (!tmp->hijos.empty()){
        if(tmp->estado == 1){
            break;
        }
        else{
            T superposicion;
            vector<pair<T,TPR_Tree_nodo<T>*>> resultados;
            T nuevos_datos[4];
            for (int i = 0; i < tmp->hijos.size(); i++){
                T i_superposicion = 0;
                nuevos_datos[0] = min(tmp->hijos[i]->MBR[0],nuevo_MBR[0]);
                nuevos_datos[1] = max(tmp->hijos[i]->MBR[1],nuevo_MBR[1]);
                nuevos_datos[2] = min(tmp->hijos[i]->MBR[2],nuevo_MBR[2]);
                nuevos_datos[3] = max(tmp->hijos[i]->MBR[3],nuevo_MBR[3]);
                for (int j = 0; j < tmp->hijos.size(); j++)
                    if(i != j)
                        i_superposicion+=calcular_superposicion(nuevos_datos,tmp->hijos[j]->MBR);
                if(resultados.empty() || i_superposicion <= superposicion){
                    superposicion = i_superposicion;
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
    
    TPR_Tree_nodo<T> *nuevo_objeto = new TPR_Tree_nodo<T>();
    nuevo_objeto->MBR = new T[4];
    nuevo_objeto->VBR = new T[4];
    nuevo_objeto->estado = 2;
    for (int i = 0; i < 4; i++){
        nuevo_objeto->MBR[i] = nuevo_MBR[i];
        nuevo_objeto->VBR[i] = nuevo_VBR[i];
    }
    if(tmp->estado == 0){
        TPR_Tree_nodo<T> *nuevo_nodo = new TPR_Tree_nodo<T>();
        nuevo_nodo->estado = 1;
        tmp->hijos.push_back(nuevo_nodo);
        int ultimo = tmp->hijos.size()-1;
        tmp->hijos[ultimo]->padre = tmp;
        tmp = tmp->hijos[ultimo];
    }
    tmp->hijos.push_back(nuevo_objeto);
    nuevo_objeto->padre = tmp;
    if(tmp->hijos.size() <= maximo)
        this->actualizar(tmp,nuevo_objeto,tiempo);
    else
        this->dividir(tmp,tamano,tiempo);
}

template <typename T>
void TPR_Tree_nodo<T>::buscar(T* region, int tiempo){
    this->paso_de_tiempo(tiempo);
    TPR_Tree_nodo<T> *tmp = this;
    queue<TPR_Tree_nodo<T> *> busqueda;
    vector<TPR_Tree_nodo<T> *> resultado;
    busqueda.push(tmp);
    while (!busqueda.empty()){
        tmp = busqueda.front();
        busqueda.pop();
        if(tmp->estado == 2){
            if(this->dentro(region,tmp->MBR)){
                resultado.push_back(tmp);
            }
        }
        else{
            for (int i = 0; i < tmp->hijos.size(); i++){
                if(this->calcular_superposicion(tmp->hijos[i]->MBR,region) != 0 || this->calcular_superposicion(region,tmp->hijos[i]->MBR) != 0){
                    busqueda.push(tmp->hijos[i]);
                }
            }
        }
    }
    if(resultado.empty()){
        cout<<"No se ha encontrado nodos en esta region"<<endl;
    }
    else{
        for (int i = 0; i < resultado.size(); i++){
            cout<<"[(";
            for (int j = 0; j < 4; j++){
                cout<<resultado[i]->MBR[j];
                if(j != 3)
                    cout<<" , ";
            }
            cout<<")(";
            for (int j = 0; j < 4; j++){
                cout<<resultado[i]->VBR[j];
                if(j != 3)
                    cout<<" , ";
            }
            cout<<")]";
            cout<<endl;
        }
    }
}

template <typename T>
void TPR_Tree_nodo<T>::eliminar(T* MBR_a_eliminar,int tamano, int maximo,int tiempo){
    this->paso_de_tiempo(tiempo);
    TPR_Tree_nodo<T> *tmp = this;
    queue<TPR_Tree_nodo<T> *> eliminador;
    int indice = -1;
    eliminador.push(tmp);
    while (!eliminador.empty() && indice == -1){
        tmp = eliminador.front();
        eliminador.pop();
        for (int i = 0; i < tmp->hijos.size(); i++){
            if(this->dentro(tmp->hijos[i]->MBR,MBR_a_eliminar)){
                if(tmp->hijos[i]->estado == 2){
                    indice = i;
                    break;
                }
                else{
                    eliminador.push(tmp->hijos[i]);
                }
            }
        }
    }
    if(indice == -1){
        cout<<"Objeto no encontrado"<<endl;
    }
    else{
        delete tmp->hijos[indice];
        tmp->hijos.erase(tmp->hijos.begin() + indice);
        if(tmp->hijos.empty()){
            this->hijos.clear();
            delete tmp;
            return;
        }
        TPR_Tree_nodo<T> *det = tmp->hijos[0];
        while(tmp->hijos.size() == 1){
            if(tmp->padre == nullptr){
                if(tmp->hijos[0]->estado == 0){
                    for (int i = 0; i < tmp->hijos[0]->hijos.size(); i++){
                        tmp->hijos.push_back(tmp->hijos[0]->hijos[i]);
                        tmp->hijos[0]->hijos[i]->padre = tmp;
                    }
                    tmp->hijos.erase(tmp->hijos.begin());
                }
                break;
            }
            TPR_Tree_nodo<T> * hermano;
            TPR_Tree_nodo<T> * f = tmp->padre;
            if(f->hijos.size() > 1){
                T superposicion;
                vector<pair<T,TPR_Tree_nodo<T>*>> resultados;
                T nuevos_datos[4];
                int ubicacion = -1;
                for (int i = 0; i < f->hijos.size(); i++){
                    if(f->hijos[i] != tmp){
                        T i_superposicion = 0;
                        nuevos_datos[0] = min(f->hijos[i]->MBR[0],tmp->hijos[0]->MBR[0]);
                        nuevos_datos[1] = max(f->hijos[i]->MBR[1],tmp->hijos[0]->MBR[1]);
                        nuevos_datos[2] = min(f->hijos[i]->MBR[2],tmp->hijos[0]->MBR[2]);
                        nuevos_datos[3] = max(f->hijos[i]->MBR[3],tmp->hijos[0]->MBR[3]);
                        for (int j = 0; j < f->hijos.size(); j++)
                            if(i != j && f->hijos[j] != tmp)
                                i_superposicion+=calcular_superposicion(nuevos_datos,f->hijos[j]->MBR);
                        if(resultados.empty() || i_superposicion <= superposicion){
                            if(resultados.empty() || i_superposicion < superposicion)
                                resultados.clear();
                            T nueva_area = this->calcular_area(nuevos_datos);
                            resultados.push_back({nueva_area,f->hijos[i]});
                        }
                    }
                    else{
                        ubicacion = i;
                    }
                } 
                sort(resultados.begin(),resultados.end());
                hermano = resultados[0].second;
                hermano->imprimir();
                cout<<"______________________________________________________________________________________________"<<endl;
                tmp->imprimir();
                cout<<"______________________________________________________________________________________________"<<endl;
                cout<<ubicacion<<endl;
                cout<<"______________________________________________________________________________________________"<<endl;
                hermano->hijos.push_back(tmp->hijos[0]);
                tmp->hijos[0]->padre = hermano;
                tmp->hijos.clear();
                f->hijos.erase(f->hijos.begin() + ubicacion);
                hermano->imprimir();
                delete tmp;
                cout<<"----------------------------------------------------------------------------------------------"<<endl;
                if(hermano->hijos.size() <= maximo){
                    tmp = f;
                }
                else{
                    this->dividir(hermano,tamano,tiempo);
                    break;
                }
            }
        }
        this->actualizar(det->padre,nullptr,tiempo);
    }
}

template <typename T>
void TPR_Tree_nodo<T>::dividir(TPR_Tree_nodo<T>*tmp,int tamano,int tiempo){
    while (1){
        TPR_Tree_nodo<T>* a = new TPR_Tree_nodo<T>();
        TPR_Tree_nodo<T>* b = new TPR_Tree_nodo<T>();
        bool vacio = true;
        T superposicion;
        T area;
        T margen;
        vector<vector<int>> resultados;
        vector<int> a_1;
        vector<int> b_1;
        T nuevos_datos_1[4];
        T nuevos_datos_2[4];
        for (int i = 0; i < tmp->hijos.size(); i++){
            for (int j = 0; j < 4; j++){
                T eje = tmp->hijos[i]->MBR[j];
                for (int s = 0; s < 4; s++){
                    if(s%2 == 0){
                        nuevos_datos_1[s] = INT16_MAX;
                        nuevos_datos_2[s] = INT16_MAX;
                    }
                    else{
                        nuevos_datos_1[s] = INT16_MIN;
                        nuevos_datos_2[s] = INT16_MIN;
                    }
                }
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
                        TPR_Tree_nodo<T>*f = tmp->padre;
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
                        TPR_Tree_nodo<T>*f = tmp->padre;
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
                    else if(nuevo_superposicion < superposicion){
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
                        TPR_Tree_nodo<T>*f = tmp->padre;
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
            a->hijos.push_back(tmp->hijos[resultados[0][i]]);
            this->actualizar(a,tmp->hijos[resultados[0][i]],0);
        }
        for (int i = 0; i < resultados[1].size(); i++){
            b->hijos.push_back(tmp->hijos[resultados[1][i]]);
            this->actualizar(b,tmp->hijos[resultados[1][i]],0);
        }
        a->estado = tmp->estado;
        b->estado = tmp->estado;
        if(tmp->padre == nullptr){
            tmp->hijos.clear();
            tmp->hijos.push_back(a);
            a->padre = tmp;
            tmp->hijos.push_back(b);
            b->padre = tmp;
            break;
        }
        else{
            TPR_Tree_nodo<T>* f = tmp->padre;
            for (int j = 0; j < f->hijos.size(); j++){
                if(f->hijos[j] == tmp){
                    delete f->hijos[j]; 
                    f->hijos[j] = a;
                    break;
                }
            }
            a->padre = f;
            if(f != this)
                this->actualizar(f,a,tiempo);
            f->hijos.push_back(b);
            b->padre = f;
            if(f->hijos.size() > tamano){
                tmp = f;
            }
            else{
                if(f != this)
                    this->actualizar(f,b,tiempo);
                break;
            }
        }
    }
}

template <typename T>
void TPR_Tree_nodo<T>::actualizar(TPR_Tree_nodo<T>*tmp,TPR_Tree_nodo<T>*cambiado, bool con_tiempo){
    bool cambio = true;
    TPR_Tree_nodo<T>* ant = cambiado;
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
        if(con_tiempo || cambiado == nullptr){
            T nuevo_MBR[4];
            T nuevo_VBR[4];
            nuevo_MBR[0]=INT16_MAX;
            nuevo_MBR[1]=INT16_MIN;
            nuevo_MBR[2]=INT16_MAX;
            nuevo_MBR[3]=INT16_MIN;
            nuevo_VBR[0]=0;
            nuevo_VBR[1]=0;
            nuevo_VBR[2]=0;
            nuevo_VBR[3]=0;
            for (int i = 0; i < tmp->hijos.size(); i++){
                tmp->hijos[i]->padre = tmp;
                nuevo_MBR[0] = min(nuevo_MBR[0],tmp->hijos[i]->MBR[0]);
                nuevo_MBR[1] = max(nuevo_MBR[1],tmp->hijos[i]->MBR[1]);
                nuevo_MBR[2] = min(nuevo_MBR[2],tmp->hijos[i]->MBR[2]);
                nuevo_MBR[3] = max(nuevo_MBR[3],tmp->hijos[i]->MBR[3]);
            }
            for (int j = 0; j < tmp->hijos.size(); j++){
                for (int i = 0; i < 4; i++){
                    int a = 0,b = 1;
                    if(i > 1){
                        a = 2;
                        b = 3;
                    }
                    if(tmp->hijos[j]->VBR[i] < 0){
                        if(nuevo_VBR[a] > tmp->hijos[j]->VBR[i]){
                            nuevo_VBR[a] = tmp->hijos[j]->VBR[i];
                            cambio = true;
                        }
                    }
                    else{
                        if(nuevo_VBR[b] < tmp->hijos[j]->VBR[i]){
                            nuevo_VBR[b] = tmp->hijos[j]->VBR[i];
                            cambio = true;
                        }
                    }
                }
            }
            for (int i = 0; i < 4; i++){
                if(nuevo_MBR[i] != tmp->MBR[i]){
                    tmp->MBR[i] = nuevo_MBR[i]; 
                    cambio = true;
                }
            }
            for (int i = 0; i < 4; i++){
                if(nuevo_VBR[i] != tmp->VBR[i]){
                    tmp->VBR[i] = nuevo_VBR[i]; 
                    cambio = true;
                }
            }
        }
        else{
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
            ant->padre = tmp;
        }
        ant = tmp;
        tmp = tmp->padre;
    }
}

template <typename T>
void TPR_Tree_nodo<T>::imprimir(){
    TPR_Tree_nodo<T>*tmp = this;
    queue<TPR_Tree_nodo<T>*> impresion;
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
TPR_Tree_nodo<T>::~TPR_Tree_nodo()
{
}
#endif