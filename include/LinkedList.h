#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <sstream>
#include <string>
#include <utility>

#include "List.h"

template <typename T>
struct Node {
    T item;
    Node<T>* next;

    explicit Node(const T& item) : item(item), next(nullptr) {}
    Node(const T& item, Node<T>* next) : item(item), next(next) {}
};


/** Converte uma cadeia de nós em texto: "1 -> 2 -> 3" (ou "[]" se vazia). */
template <typename T>
std::string nodesToString(const Node<T>* first) {
    if (first == nullptr) return "[]";
    std::ostringstream out;
    for (const Node<T>* cur = first; cur != nullptr; cur = cur->next) {
        out << cur->item;
        if (cur->next != nullptr) out << " -> ";
    }
    return out.str();
}

/** Constrói uma cadeia de nós (alocados com new) a partir dos valores dados. */
template <typename T>
Node<T>* buildNodes(std::initializer_list<T> values) {
    Node<T>* first = nullptr;
    Node<T>* last = nullptr;
    for (const T& v : values) {
        Node<T>* node = new Node<T>(v);
        if (last == nullptr) first = node;
        else last->next = node;
        last = node;
    }
    return first;
}

/** Libera todos os nós de uma cadeia. */
template <typename T>
void freeNodes(Node<T>* first) {
    while (first != nullptr) {
        Node<T>* next = first->next;
        delete first;
        first = next;
    }
}

template <typename T>
bool deleteNodeWithoutHead(Node<T>* p) {
    if (p == nullptr || p->next == nullptr) {
        return false;  // nada a fazer: P nulo ou último nó
    }
    Node<T>* successor = p->next;
    p->item = successor->item;   // 1) copia o conteúdo do sucessor para P
    p->next = successor->next;   // 2) P "pula" o sucessor
    delete successor;            // 3) libera o nó que sobrou (evita memory leak)
    return true;
}

template <typename T>
Node<T>* mergeTwoLists(Node<T>* list1, Node<T>* list2) {
    if (list1 == nullptr) return list2;
    if (list2 == nullptr) return list1;

    // Escolhe a cabeça da lista resultante.
    Node<T>* head;
    if (list2->item < list1->item) {
        head = list2;
        list2 = list2->next;
    } else {
        head = list1;
        list1 = list1->next;
    }
    Node<T>* tail = head;

    // Anexa sempre o menor entre as duas frentes
    while (list1 != nullptr && list2 != nullptr) {
        if (list2->item < list1->item) {
            tail->next = list2;
            list2 = list2->next;
        } else {
            tail->next = list1;
            list1 = list1->next;
        }
        tail = tail->next;
    }

    tail->next = (list1 != nullptr) ? list1 : list2;
    return head;
}

template <typename T>
Node<T>* removeNthFromEnd(Node<T>* head, int n) {
    if (head == nullptr || n <= 0) return head;

    Node<T>* fast = head;
    for (int i = 0; i < n; ++i) {
        if (fast == nullptr) return head;  // n maior que o tamanho da lista
        fast = fast->next;
    }

    if (fast == nullptr) {                 // n == tamanho: remove a cabeça
        Node<T>* newHead = head->next;
        delete head;
        return newHead;
    }

    Node<T>* slow = head;
    while (fast->next != nullptr) {
        fast = fast->next;
        slow = slow->next;
    }

    Node<T>* target = slow->next;
    slow->next = target->next;
    delete target;
    return head;
}


template <typename T>
class LinkedList : public List<T> {
private:
    Node<T>* head;
    Node<T>* tail;
    std::size_t count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    LinkedList(std::initializer_list<T> values) : LinkedList() {
        for (const T& v : values) addLast(v);
    }

    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    LinkedList(LinkedList&& other) noexcept
        : head(other.head), tail(other.tail), count(other.count) {
        other.head = other.tail = nullptr;
        other.count = 0;
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            count = other.count;
            other.head = other.tail = nullptr;
            other.count = 0;
        }
        return *this;
    }

    ~LinkedList() override { clear(); }

    void addFirst(const T& item) override {
        head = new Node<T>(item, head);
        if (tail == nullptr) tail = head;
        ++count;
    }

    void addLast(const T& item) override {
        Node<T>* node = new Node<T>(item);
        if (tail == nullptr) head = node;
        else tail->next = node;
        tail = node;
        ++count;
    }

    bool removeFirst() override {
        if (head == nullptr) return false;
        Node<T>* old = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete old;
        --count;
        return true;
    }

    bool removeLast() override {
        if (head == nullptr) return false;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node<T>* prev = head;
            while (prev->next != tail) prev = prev->next;
            delete tail;
            tail = prev;
            tail->next = nullptr;
        }
        --count;
        return true;
    }

    void clear() override {
        while (removeFirst()) {}
    }

    std::size_t size() const override { return count; }
    bool isEmpty() const override { return count == 0; }
    std::string toString() const override { return nodesToString<T>(head); }

    Node<T>* getHead() const { return head; }
    Node<T>* getTail() const { return tail; }

    /** Retorna o primeiro nó cujo item é igual a value (ou nullptr). */
    Node<T>* find(const T& value) const {
        for (Node<T>* cur = head; cur != nullptr; cur = cur->next) {
            if (cur->item == value) return cur;
        }
        return nullptr;
    }

    // QUESTÃO 1

    void addSorted(const T& item) {
        Node<T>* newNode = new Node<T>(item);

        if (head == nullptr) {
            head = tail = newNode;
        } else if (item < head->item) {
            newNode->next = head;
            head = newNode;
        } else if (!(item < tail->item)) {
            tail->next = newNode;
            tail = newNode;
        } else {
            // head->item <= item < tail->item: o ponto de inserção está no meio.
            Node<T>* prev = head;
            while (prev->next != nullptr && !(item < prev->next->item)) {
                prev = prev->next;
            }
            newNode->next = prev->next;
            prev->next = newNode;
        }
        ++count;
    }

    //  QUESTÃO 2

    bool deleteNodeWithoutHead(Node<T>* p) {
        if (p == nullptr || p->next == nullptr) return false;
        if (p->next == tail) tail = p;  // o sucessor (tail) será liberado
        ::deleteNodeWithoutHead(p);
        --count;
        return true;
    }

    //  QUESTÃO 3

    void reverse() {
        Node<T>* prev = nullptr;
        Node<T>* cur = head;
        tail = head;  // o antigo primeiro nó será o último
        while (cur != nullptr) {
            Node<T>* next = cur->next;  // guarda o resto da lista
            cur->next = prev;           // inverte o elo
            prev = cur;
            cur = next;
        }
        head = prev;  // prev é o antigo último nó
    }

    // - QUESTÃO 4

    void mergeSorted(LinkedList& other) {
        if (this == &other) return;
        head = mergeTwoLists<T>(head, other.head);
        count += other.count;
        tail = head;
        while (tail != nullptr && tail->next != nullptr) tail = tail->next;
        other.head = other.tail = nullptr;
        other.count = 0;
    }

    //  QUESTÃO 5

    bool removeNthFromEnd(int n) {
        if (n < 1 || static_cast<std::size_t>(n) > count) return false;
        head = ::removeNthFromEnd<T>(head, n);
        --count;
        if (head == nullptr) {
            tail = nullptr;
        } else if (n == 1) {  // o último nó foi removido recalcula tail
            tail = head;
            while (tail->next != nullptr) tail = tail->next;
        }
        return true;
    }
};

#endif
