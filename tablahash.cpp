

#include <iostream>
#include <vector>
using namespace std;

// Clase Tabla Hash con encadenamiento usando vector de vectores
class TablaHash {
private:
    int tamano;
    vector<vector<int>> tabla;  // Encadenamiento con vectores
    int totalColisiones;

    // Funcion hash: h(x) = x mod m
    inline int funcionHash(int clave) const {
        return clave % tamano;
    }

public:
    TablaHash(int tam) : tamano(tam), tabla(tam), totalColisiones(0) {}

    // Insertar un elemento con encadenamiento
    void insertar(int valor) {
        int indice = funcionHash(valor);
        
        if (tabla[indice].empty()) {
            cout << "  " << valor << " -> Indice " << indice << " (sin colision)" << endl;
        } else {
            totalColisiones++;
            cout << "  " << valor << " -> Indice " << indice << " (COLISION #" << totalColisiones << ")" << endl;
        }
        
        tabla[indice].push_back(valor);
    }

    // Mostrar la tabla hash completa
    void mostrarTabla() const {
        cout << "\n========================================" << endl;
        cout << "  TABLA HASH (Tamano: " << tamano << ")" << endl;
        cout << "========================================" << endl;
        
        for (int i = 0; i < tamano; i++) {
            cout << "Indice [" << i << "]: ";
            
            if (tabla[i].empty()) {
                cout << "vacio";
            } else {
                for (size_t j = 0; j < tabla[i].size(); j++) {
                    cout << tabla[i][j];
                    if (j < tabla[i].size() - 1) cout << " -> ";
                }
            }
            cout << endl;
        }
        cout << "========================================" << endl;
    }

    // Obtener total de colisiones
    inline int obtenerColisiones() const {
        return totalColisiones;
    }

    // Buscar un elemento (optimizado)
    bool buscar(int valor) const {
        int indice = funcionHash(valor);
        const vector<int>& lista = tabla[indice];
        
        for (int elem : lista) {
            if (elem == valor) return true;
        }
        return false;
    }

    // Calcular factor de carga
    double factorCarga() const {
        int elementosUsados = 0;
        for (const auto& lista : tabla) {
            if (!lista.empty()) elementosUsados++;
        }
        return static_cast<double>(elementosUsados) / tamano;
    }
};

int main() {
    cout << "\n========================================" << endl;
    cout << "  TABLA HASH CON ENCADENAMIENTO" << endl;
    cout << "========================================" << endl;

    // Datos de ejemplo
    const int datos[] = {23, 57, 87, 15, 42, 34, 20};
    const int n = sizeof(datos) / sizeof(datos[0]);

    cout << "\nDatos a insertar: ";
    for (int i = 0; i < n; i++) {
        cout << datos[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "\n" << endl;

    // ===== TABLA CON TAMANO 10 =====
    cout << "\n========================================" << endl;
    cout << "  TABLA HASH - Tamano 10" << endl;
    cout << "  Funcion: h(x) = x mod 10" << endl;
    cout << "========================================" << endl;
    
    TablaHash tabla10(10);
    
    cout << "\nInsertando elementos:" << endl;
    for (int i = 0; i < n; i++) {
        tabla10.insertar(datos[i]);
    }
    
    tabla10.mostrarTabla();
    cout << "\nTotal de colisiones: " << tabla10.obtenerColisiones() << endl;
    cout << "Factor de carga: " << tabla10.factorCarga() * 100 << "%" << endl;

    // ===== TABLA CON TAMANO 7 =====
    cout << "\n\n========================================" << endl;
    cout << "  TABLA HASH - Tamano 7" << endl;
    cout << "  Funcion: h(x) = x mod 7" << endl;
    cout << "========================================" << endl;
    
    TablaHash tabla7(7);
    
    cout << "\nInsertando elementos:" << endl;
    for (int i = 0; i < n; i++) {
        tabla7.insertar(datos[i]);
    }
    
    tabla7.mostrarTabla();
    cout << "\nTotal de colisiones: " << tabla7.obtenerColisiones() << endl;
    cout << "Factor de carga: " << tabla7.factorCarga() * 100 << "%" << endl;

    // ===== COMPARACION =====
    cout << "\n\n========================================" << endl;
    cout << "  COMPARACION DE COLISIONES" << endl;
    cout << "========================================" << endl;
    cout << "Tabla tamano 10: " << tabla10.obtenerColisiones() << " colisiones" << endl;
    cout << "Tabla tamano 7:  " << tabla7.obtenerColisiones() << " colisiones" << endl;
    
    int diferencia = tabla7.obtenerColisiones() - tabla10.obtenerColisiones();
    cout << "\nDiferencia: " << diferencia << " colisiones mas en tabla de tamano 7" << endl;
    
    cout << "\nConclusiones:" << endl;
    cout << "- Tabla mas grande (10) genera MENOS colisiones" << endl;
    cout << "- Tabla mas pequena (7) genera MAS colisiones" << endl;
    cout << "- El encadenamiento maneja bien ambas situaciones" << endl;
    cout << "========================================\n" << endl;

    return 0;
}