#define PUNTO
#ifdef PUNTO
#include <limits>
template <typename T>
class punto{
public:
    bool vacio;
    T x;
    T y;
public:
    punto();
    punto(T,T);
    bool nulo();
    bool operator<(const punto& a){
        if(this->x < a.x && this->y < a.y){
            return true;
        }
        return false;
    }
    bool operator<=(const punto& a){
        if(this->x <= a.x && this->y <= a.y){
            return true;
        }
        return false;
    }
    ~punto();
};
template <typename T>
punto<T>::punto()
{
    vacio = true;
}

template <typename T>
punto<T>::punto(T x_1,T y_1)
{
    x = x_1;
    y = y_1;
    vacio = false;
}

template <typename T>
bool punto<T>::nulo()
{
    return vacio;
}

template <typename T>
punto<T>::~punto()
{
}

#endif