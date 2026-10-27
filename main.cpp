// Demonstração da Lista 07 - Algoritmos em Listas Encadeadas Simples.
// Executa os exemplos de entrada/saída do enunciado, questão por questão.

#include <iostream>

#include "LinkedList.h"

static void title(const std::string& text) {
    std::cout << "\n==================================================\n"
              << text
              << "\n==================================================\n";
}

static void demoAddSorted() {
    title("Questao 1 - addSorted(item)");

    LinkedList<int> a{10, 20, 30};
    std::cout << "Lista: " << a.toString() << "\nInserir: 5\n";
    a.addSorted(5);
    std::cout << "Lista Resultante: " << a.toString() << "\n\n";

    LinkedList<int> b{10, 20, 30};
    std::cout << "Lista: " << b.toString() << "\nInserir: 25\n";
    b.addSorted(25);
    std::cout << "Lista Resultante: " << b.toString() << "\n\n";

    LinkedList<int> empty;
    std::cout << "Lista: (vazia)\nInserir: 7\n";
    empty.addSorted(7);
    std::cout << "Lista Resultante: " << empty.toString()
              << " | size=" << empty.size() << "\n";
}

static void demoDeleteNodeWithoutHead() {
    title("Questao 2 - deleteNodeWithoutHead(P)");

    LinkedList<int> list{20, 4, 5, 19, 47};
    std::cout << "Lista: " << list.toString() << "\n";
    Node<int>* p = list.find(5);
    std::cout << "Ponteiro P aponta para o no com valor " << p->item << "\n";
    list.deleteNodeWithoutHead(p);
    std::cout << "Lista Resultante: " << list.toString() << "\n";

    Node<int>* last = list.getTail();
    std::cout << "\nTentando remover o ULTIMO no (" << last->item << ") sem predecessor: "
              << (list.deleteNodeWithoutHead(last) ? "removido" : "impossivel (P->next == nullptr)")
              << "\nLista: " << list.toString() << "\n";
}

static void demoReverse() {
    title("Questao 3 - reverse()");

    LinkedList<int> list{20, 4, 5, 19, 47};
    std::cout << "Antes : head -> " << list.toString()
              << " (tail = " << list.getTail()->item << ")\n";
    list.reverse();
    std::cout << "Depois: head -> " << list.toString()
              << " (tail = " << list.getTail()->item << ")\n";
}

static void demoMerge() {
    title("Questao 4 - mergeTwoLists(l1, l2)");

    Node<int>* l1 = buildNodes<int>({1, 3, 5, 7});
    Node<int>* l2 = buildNodes<int>({2, 4, 6, 8});
    std::cout << "list1 = [" << nodesToString(l1) << "]\n"
              << "list2 = [" << nodesToString(l2) << "]\n";
    Node<int>* merged = mergeTwoLists(l1, l2);
    std::cout << "Lista Resultante: [" << nodesToString(merged) << "]\n";
    freeNodes(merged);
}

static void demoRemoveNth() {
    title("Questao 5 - removeNthFromEnd(head, n)");

    struct Case { std::initializer_list<int> values; int n; };
    const Case cases[] = {
        {{1, 2, 3, 4, 5}, 2},
        {{1}, 1},
        {{1, 2}, 1},
    };
    for (const Case& c : cases) {
        Node<int>* head = buildNodes<int>(c.values);
        std::cout << "head = [" << nodesToString(head) << "], n = " << c.n;
        head = removeNthFromEnd(head, c.n);
        std::cout << "  ->  " << (head ? "[" + nodesToString(head) + "]" : std::string("[]")) << "\n";
        freeNodes(head);
    }
}

int main() {
    demoAddSorted();
    demoDeleteNodeWithoutHead();
    demoReverse();
    demoMerge();
    demoRemoveNth();
    std::cout << "\nPara validar tudo automaticamente execute: ./list07_tests\n";
    return 0;
}
