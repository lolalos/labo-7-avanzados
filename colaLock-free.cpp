#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>
using namespace std;

template<typename T>
struct Node {
    T data;
    atomic<Node<T>*> next;
    
    Node(T val) : data(val), next(nullptr) {}
};

template<typename T>
class LockFreeQueue {
private:
    atomic<Node<T>*> head;
    atomic<Node<T>*> tail;
    int casCount;
    
    void printState(const string& operation, Node<T>* tailPtr) {
        cout << "  [CAS #" << ++casCount << "] " << operation << "\n";
        cout << "    -> tail apunta a nodo con valor: ";
        if(tailPtr) cout << tailPtr->data << "\n";
        else cout << "nullptr\n";
    }
    
public:
    LockFreeQueue() : casCount(0) {
        Node<T>* dummy = new Node<T>(T());
        head.store(dummy);
        tail.store(dummy);
    }
    
    void enqueue(T value, int threadId) {
        Node<T>* newNode = new Node<T>(value);
        
        cout << "\n--- Hilo " << threadId << ": Encolando " << value << " ---\n";
        
        while(true) {
            Node<T>* last = tail.load();
            Node<T>* next = last->next.load();
            
            if(last == tail.load()) {
                if(next == nullptr) {
                    if(last->next.compare_exchange_weak(next, newNode)) {
                        printState("next de " + to_string(last->data) + " -> " + to_string(value), last);
                        tail.compare_exchange_weak(last, newNode);
                        printState("tail avanzado a " + to_string(value), newNode);
                        break;
                    }
                } else {
                    tail.compare_exchange_weak(last, next);
                    printState("tail corregido (estaba atrasado)", next);
                }
            }
        }
        
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    
    void printQueue() {
        cout << "\n========== LISTA FINAL ==========\n";
        cout << "Contenido de la cola: ";
        Node<T>* current = head.load()->next.load();
        
        if(!current) {
            cout << "(vacia)";
        } else {
            while(current) {
                cout << current->data;
                current = current->next.load();
                if(current) cout << " -> ";
            }
        }
        cout << "\n=================================\n";
    }
    
    void explainABA() {
        cout << "\n========== PROBLEMA ABA ==========\n";
        cout << "El problema ABA ocurre cuando:\n\n";
        cout << "1. Hilo T1 lee tail apuntando a nodo A\n";
        cout << "2. T1 es suspendido antes de hacer CAS\n";
        cout << "3. Hilo T2 desencola A, luego B, luego encola nuevo nodo (reutiliza direccion A)\n";
        cout << "4. T1 se reanuda, ve que tail sigue siendo A (misma direccion)\n";
        cout << "5. CAS tiene exito aunque la estructura cambio entre medio\n\n";
        cout << "Solucion: Usar contadores de version (tagged pointers) o hazard pointers\n";
        cout << "En esta simulacion simplificada no ocurre porque no hay dequeue ni reuso de memoria\n";
        cout << "==================================\n";
    }
};

void threadFunction(LockFreeQueue<int>& queue, vector<int> values, int threadId) {
    for(int val : values) {
        queue.enqueue(val, threadId);
    }
}

int main() {
    LockFreeQueue<int> queue;
    
    cout << "COLA LOCK-FREE (Michael-Scott)\n";
    cout << "===============================\n";
    cout << "Simulacion con 2 hilos:\n";
    cout << "  Hilo 1: encola 1, 2\n";
    cout << "  Hilo 2: encola 3, 4\n\n";
    cout << "Estado inicial: cola vacia con nodo dummy\n";
    
    thread t1(threadFunction, ref(queue), vector<int>{1, 2}, 1);
    thread t2(threadFunction, ref(queue), vector<int>{3, 4}, 2);
    
    t1.join();
    t2.join();
    
    queue.printQueue();
    queue.explainABA();
    
    return 0;
}
