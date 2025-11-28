

#include <iostream>
#include <algorithm>
using namespace std;

// Definición de la estructura del nodo
class Nodo {
public:
    int clave;
    Nodo* izq;
    Nodo* der;
    int altura;

    Nodo(int valor) {
        clave = valor;
        izq = nullptr;
        der = nullptr;
        altura = 1;  // Un nodo nuevo tiene altura 1
    }
};

// Clase ArbolAVL
class ArbolAVL {
private:
    Nodo* raiz;

    // Obtener la altura de un nodo
    int obtenerAltura(Nodo* nodo) {
        if (nodo == nullptr)
            return 0;
        return nodo->altura;
    }

    // Calcular el factor de balance de un nodo
    int factorBalance(Nodo* nodo) {
        if (nodo == nullptr)
            return 0;
        return obtenerAltura(nodo->izq) - obtenerAltura(nodo->der);
    }

    // Actualizar la altura de un nodo
    void actualizarAltura(Nodo* nodo) {
        if (nodo != nullptr) {
            nodo->altura = 1 + max(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));
        }
    }

    // Rotación simple a la derecha
    Nodo* rotarDerecha(Nodo* y) {
        Nodo* x = y->izq;
        Nodo* T2 = x->der;

        // Realizar rotación
        x->der = y;
        y->izq = T2;

        // Actualizar alturas
        actualizarAltura(y);
        actualizarAltura(x);

        return x;  // Nueva raíz
    }

    // Rotación simple a la izquierda
    Nodo* rotarIzquierda(Nodo* x) {
        Nodo* y = x->der;
        Nodo* T2 = y->izq;

        // Realizar rotación
        y->izq = x;
        x->der = T2;

        // Actualizar alturas
        actualizarAltura(x);
        actualizarAltura(y);

        return y;  // Nueva raíz
    }

    // Función de inserción recursiva
    Nodo* insertar(Nodo* nodo, int clave) {
        // 1. Inserción normal de BST
        if (nodo == nullptr)
            return new Nodo(clave);

        if (clave < nodo->clave)
            nodo->izq = insertar(nodo->izq, clave);
        else if (clave > nodo->clave)
            nodo->der = insertar(nodo->der, clave);
        else
            return nodo;  // No se permiten claves duplicadas

        // 2. Actualizar altura del nodo ancestro
        actualizarAltura(nodo);

        // 3. Obtener el factor de balance
        int fb = factorBalance(nodo);

        // 4. Si el nodo está desbalanceado, hay 4 casos:

        // Caso 1: Rotación simple derecha (Left-Left)
        if (fb > 1 && clave < nodo->izq->clave)
            return rotarDerecha(nodo);

        // Caso 2: Rotación simple izquierda (Right-Right)
        if (fb < -1 && clave > nodo->der->clave)
            return rotarIzquierda(nodo);

        // Caso 3: Rotación doble izquierda-derecha (Left-Right)
        if (fb > 1 && clave > nodo->izq->clave) {
            nodo->izq = rotarIzquierda(nodo->izq);
            return rotarDerecha(nodo);
        }

        // Caso 4: Rotación doble derecha-izquierda (Right-Left)
        if (fb < -1 && clave < nodo->der->clave) {
            nodo->der = rotarDerecha(nodo->der);
            return rotarIzquierda(nodo);
        }

        return nodo;
    }

    // Recorrido in-order (izquierda-raíz-derecha)
    void inOrder(Nodo* nodo) {
        if (nodo != nullptr) {
            inOrder(nodo->izq);
            cout << nodo->clave << " ";
            inOrder(nodo->der);
        }
    }

    // Recorrido pre-order (raíz-izquierda-derecha)
    void preOrder(Nodo* nodo) {
        if (nodo != nullptr) {
            cout << nodo->clave << " ";
            preOrder(nodo->izq);
            preOrder(nodo->der);
        }
    }

    // Recorrido post-order (izquierda-derecha-raíz)
    void postOrder(Nodo* nodo) {
        if (nodo != nullptr) {
            postOrder(nodo->izq);
            postOrder(nodo->der);
            cout << nodo->clave << " ";
        }
    }

public:
    ArbolAVL() {
        raiz = nullptr;
    }

    // Método público para insertar
    void insertarClave(int clave) {
        raiz = insertar(raiz, clave);
    }

    // Obtener la altura total del árbol
    int obtenerAlturaTotal() {
        return obtenerAltura(raiz);
    }

    // Métodos públicos para recorridos
    void mostrarInOrder() {
        cout << "In-order: ";
        inOrder(raiz);
        cout << endl;
    }

    void mostrarPreOrder() {
        cout << "Pre-order: ";
        preOrder(raiz);
        cout << endl;
    }

    void mostrarPostOrder() {
        cout << "Post-order: ";
        postOrder(raiz);
        cout << endl;
    }
};

// Función para mostrar el árbol de forma visual
void mostrarArbolVisual(Nodo* nodo, int espacio = 0, int nivel = 5) {
    if (nodo == nullptr)
        return;
    
    espacio += nivel;
    mostrarArbolVisual(nodo->der, espacio);
    
    cout << endl;
    for (int i = nivel; i < espacio; i++)
        cout << " ";
    cout << nodo->clave << endl;
    
    mostrarArbolVisual(nodo->izq, espacio);
}

int main() {
    ArbolAVL arbol;

    cout << "=== ARBOL AVL - Insercion y Rotaciones ===" << endl << endl;

    // Ejemplo de insercion de elementos
    int elementos[] = {10, 20, 30, 40, 50, 25};
    int n = sizeof(elementos) / sizeof(elementos[0]);

    for (int i = 0; i < n; i++) {
        cout << "Insertando: " << elementos[i] << endl;
        arbol.insertarClave(elementos[i]);
        cout << "Altura del arbol: " << arbol.obtenerAlturaTotal() << endl;
        arbol.mostrarInOrder();
        cout << endl;
    }

    cout << "\n========================================" << endl;
    cout << "           ARBOL FINAL" << endl;
    cout << "========================================" << endl;
    cout << "\nAltura total: " << arbol.obtenerAlturaTotal() << endl;
    
    cout << "\n========================================" << endl;
    cout << "           RECORRIDOS" << endl;
    cout << "========================================" << endl;
    arbol.mostrarInOrder();
    arbol.mostrarPreOrder();
    arbol.mostrarPostOrder();
    cout << "\n========================================" << endl;

    return 0;
}