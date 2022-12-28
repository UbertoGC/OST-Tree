#include "OST_Tree.h"

int main() {
    OST_Tree<int> alfa;
    int continuar;
    int tiempo;

    int datos_MBR[4] = {2, 4, 3, 4};
    int datos_VBR[4] = {1, 1, 1, 1};

    alfa.anadir(datos_VBR, datos_MBR, 0);
    alfa.imprimir();

    cout << "continuar?(0: no, 1: anadir, 2: buscar, 3: eliminar)" << endl;
    cin >> continuar;

    while (continuar) {
        if (continuar == 1) {
            cout << "datos VBR:" << endl;
            for (int i = 0; i < 4; i++) {
                cin >> datos_VBR[i];
            }
            cout << "datos MBR:" << endl;
            for (int i = 0; i < 4; i++) {
                cin >> datos_MBR[i];
            }
        } else {
            cout << "datos MBR:" << endl;
            for (int i = 0; i < 4; i++) {
                cin >> datos_MBR[i];
            }
        }
        cout << "tiempo:" << endl;
        cin >> tiempo;
        if (continuar == 1) {
            alfa.anadir(datos_VBR, datos_MBR, tiempo);
        } else if (continuar == 2) {
            alfa.buscar(datos_MBR, tiempo);
        } else if (continuar == 3) {
            alfa.eliminar(datos_MBR, tiempo);
        }
        alfa.imprimir();
        cout << "continuar?(0: no, 1: anadir, 2: buscar, 3: eliminar)" << endl;
        cin >> continuar;
    }
    /**/
    return 0;
}
