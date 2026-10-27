// Testes automatizados da Lista 07 (sem dependências externas).
// Executar: ./list07_tests   (retorna 0 se tudo passar, 1 caso contrário)

#include <iostream>
#include <string>

#include "LinkedList.h"

static int g_total = 0;
static int g_failed = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        ++g_total;                                                           \
        if (!(cond)) {                                                       \
            ++g_failed;                                                      \
            std::cerr << "  [FALHA] " << #cond << "  (linha " << __LINE__    \
                      << ")\n";                                              \
        }                                                                    \
    } while (0)

#define CHECK_EQ(actual, expected)                                           \
    do {                                                                     \
        ++g_total;                                                           \
        std::string a_ = (actual);                                           \
        std::string e_ = (expected);                                         \
        if (a_ != e_) {                                                      \
            ++g_failed;                                                      \
            std::cerr << "  [FALHA] linha " << __LINE__ << ": obtido \""     \
                      << a_ << "\" esperado \"" << e_ << "\"\n";             \
        }                                                                    \
    } while (0)


template <typename T>
bool invariantsOk(const LinkedList<T>& list) {
    std::size_t n = 0;
    Node<T>* last = nullptr;
    for (Node<T>* cur = list.getHead(); cur != nullptr; cur = cur->next) {
        ++n;
        last = cur;
    }
    if (n != list.size()) return false;
    if (last != list.getTail()) return false;
    if (last != nullptr && last->next != nullptr) return false;
    return (list.getHead() == nullptr) == (list.getTail() == nullptr);
}

// Questão 1
void testAddSorted() {
    std::cout << "Questao 1: addSorted\n";

    LinkedList<int> empty;
    empty.addSorted(7);
    CHECK_EQ(empty.toString(), "7");
    CHECK(empty.getHead() == empty.getTail());
    CHECK(invariantsOk(empty));

    LinkedList<int> a{10, 20, 30};
    a.addSorted(5);  // início
    CHECK_EQ(a.toString(), "5 -> 10 -> 20 -> 30");
    CHECK(a.getHead()->item == 5);
    CHECK(invariantsOk(a));

    LinkedList<int> b{10, 20, 30};
    b.addSorted(25);  // meio
    CHECK_EQ(b.toString(), "10 -> 20 -> 25 -> 30");
    CHECK(invariantsOk(b));

    LinkedList<int> c{10, 20, 30};
    c.addSorted(99);  // fim
    CHECK_EQ(c.toString(), "10 -> 20 -> 30 -> 99");
    CHECK(c.getTail()->item == 99);
    CHECK(invariantsOk(c));

    LinkedList<int> d{10, 20, 30};
    d.addSorted(10);  // duplicado no início
    d.addSorted(30);  // duplicado no fim
    d.addSorted(20);  // duplicado no meio
    CHECK_EQ(d.toString(), "10 -> 10 -> 20 -> 20 -> 30 -> 30");
    CHECK(invariantsOk(d));

    LinkedList<int> e;
    for (int v : {5, 1, 4, 2, 3, 9, 0, 7}) e.addSorted(v);
    CHECK_EQ(e.toString(), "0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 7 -> 9");
    CHECK(e.size() == 8);
    CHECK(invariantsOk(e));
}

// Questão 2
void testDeleteNodeWithoutHead() {
    std::cout << "Questao 2: deleteNodeWithoutHead\n";

    // Função livre sobre nós crus (exemplo do enunciado).
    Node<int>* h = buildNodes<int>({20, 4, 5, 19, 47});
    Node<int>* p = h->next->next;  // nó com valor 5
    CHECK(p->item == 5);
    CHECK(deleteNodeWithoutHead(p));
    CHECK_EQ(nodesToString(h), "20 -> 4 -> 19 -> 47");
    freeNodes(h);

    h = buildNodes<int>({1, 2, 3});
    Node<int>* last = h->next->next;
    CHECK(!deleteNodeWithoutHead(last));
    CHECK_EQ(nodesToString(h), "1 -> 2 -> 3");
    freeNodes(h);

    CHECK(!deleteNodeWithoutHead<int>(nullptr));

    h = buildNodes<int>({1, 2, 3});
    CHECK(deleteNodeWithoutHead(h));
    CHECK_EQ(nodesToString(h), "2 -> 3");
    freeNodes(h);

    LinkedList<int> list{20, 4, 5, 19, 47};
    CHECK(list.deleteNodeWithoutHead(list.find(5)));
    CHECK_EQ(list.toString(), "20 -> 4 -> 19 -> 47");
    CHECK(list.size() == 4);
    CHECK(invariantsOk(list));

    // Remover o penúltimo exige atualizar tail
    CHECK(list.deleteNodeWithoutHead(list.find(19)));
    CHECK_EQ(list.toString(), "20 -> 4 -> 47");
    CHECK(list.getTail()->item == 47);
    CHECK(invariantsOk(list));

    CHECK(!list.deleteNodeWithoutHead(list.getTail()));
    CHECK(list.size() == 3);
}

// Questão 3
void testReverse() {
    std::cout << "Questao 3: reverse\n";

    LinkedList<int> a{20, 4, 5, 19, 47};
    a.reverse();
    CHECK_EQ(a.toString(), "47 -> 19 -> 5 -> 4 -> 20");
    CHECK(a.getHead()->item == 47);
    CHECK(a.getTail()->item == 20);
    CHECK(a.size() == 5);
    CHECK(invariantsOk(a));

    a.reverse();
    CHECK_EQ(a.toString(), "20 -> 4 -> 5 -> 19 -> 47");
    CHECK(invariantsOk(a));

    LinkedList<int> empty;
    empty.reverse();
    CHECK(empty.isEmpty());
    CHECK(empty.getHead() == nullptr && empty.getTail() == nullptr);

    LinkedList<int> one{42};
    one.reverse();
    CHECK_EQ(one.toString(), "42");
    CHECK(one.getHead() == one.getTail());

    LinkedList<int> two{1, 2};
    two.reverse();
    CHECK_EQ(two.toString(), "2 -> 1");
    CHECK(invariantsOk(two));

    // Inserir depois de inverter
    two.addLast(0);
    CHECK_EQ(two.toString(), "2 -> 1 -> 0");
}

// Questão 4
void testMerge() {
    std::cout << "Questao 4: mergeTwoLists\n";

    Node<int>* l1 = buildNodes<int>({1, 3, 5, 7});
    Node<int>* l2 = buildNodes<int>({2, 4, 6, 8});
    Node<int>* firstOfL1 = l1;
    Node<int>* m = mergeTwoLists(l1, l2);
    CHECK_EQ(nodesToString(m), "1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8");
    CHECK(m == firstOfL1);
    freeNodes(m);

    // Uma das listas vazia.
    m = mergeTwoLists(buildNodes<int>({1, 2}), static_cast<Node<int>*>(nullptr));
    CHECK_EQ(nodesToString(m), "1 -> 2");
    freeNodes(m);
    m = mergeTwoLists(static_cast<Node<int>*>(nullptr), buildNodes<int>({3}));
    CHECK_EQ(nodesToString(m), "3");
    freeNodes(m);

    // Ambas vazias.
    CHECK(mergeTwoLists<int>(nullptr, nullptr) == nullptr);

    // Tamanhos diferentes
    m = mergeTwoLists(buildNodes<int>({1, 2, 3}), buildNodes<int>({10, 20, 30, 40}));
    CHECK_EQ(nodesToString(m), "1 -> 2 -> 3 -> 10 -> 20 -> 30 -> 40");
    freeNodes(m);

    m = mergeTwoLists(buildNodes<int>({10, 20}), buildNodes<int>({1, 2, 3}));
    CHECK_EQ(nodesToString(m), "1 -> 2 -> 3 -> 10 -> 20");
    freeNodes(m);

    m = mergeTwoLists(buildNodes<int>({1, 3, 3}), buildNodes<int>({3, 3, 4}));
    CHECK_EQ(nodesToString(m), "1 -> 3 -> 3 -> 3 -> 3 -> 4");
    freeNodes(m);

    Node<int>* s1 = buildNodes<int>({5});
    Node<int>* s2 = buildNodes<int>({5});
    m = mergeTwoLists(s1, s2);
    CHECK(m == s1 && m->next == s2);
    freeNodes(m);

    LinkedList<int> a{1, 3, 5, 7};
    LinkedList<int> b{2, 4, 6, 8};
    a.mergeSorted(b);
    CHECK_EQ(a.toString(), "1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8");
    CHECK(a.size() == 8);
    CHECK(a.getTail()->item == 8);
    CHECK(b.isEmpty());
    CHECK(invariantsOk(a));
    CHECK(invariantsOk(b));
}

//Questão 5
void testRemoveNthFromEnd() {
    std::cout << "Questao 5: removeNthFromEnd\n";

    Node<int>* h = buildNodes<int>({1, 2, 3, 4, 5});
    h = removeNthFromEnd(h, 2);
    CHECK_EQ(nodesToString(h), "1 -> 2 -> 3 -> 5");
    freeNodes(h);

    h = buildNodes<int>({1});
    h = removeNthFromEnd(h, 1);
    CHECK(h == nullptr);
    CHECK_EQ(nodesToString(h), "[]");

    h = buildNodes<int>({1, 2});
    h = removeNthFromEnd(h, 1);
    CHECK_EQ(nodesToString(h), "1");
    freeNodes(h);

    h = buildNodes<int>({1, 2, 3});
    h = removeNthFromEnd(h, 3);
    CHECK_EQ(nodesToString(h), "2 -> 3");
    freeNodes(h);

    h = buildNodes<int>({1, 2, 3, 4});
    h = removeNthFromEnd(h, 1);
    CHECK_EQ(nodesToString(h), "1 -> 2 -> 3");
    freeNodes(h);

    h = buildNodes<int>({1, 2, 3});
    h = removeNthFromEnd(h, 4);
    CHECK_EQ(nodesToString(h), "1 -> 2 -> 3");
    h = removeNthFromEnd(h, 0);
    CHECK_EQ(nodesToString(h), "1 -> 2 -> 3");
    h = removeNthFromEnd(h, -1);
    CHECK_EQ(nodesToString(h), "1 -> 2 -> 3");
    freeNodes(h);

    CHECK(removeNthFromEnd<int>(nullptr, 1) == nullptr);

    LinkedList<int> list{1, 2, 3, 4, 5};
    CHECK(list.removeNthFromEnd(1));
    CHECK_EQ(list.toString(), "1 -> 2 -> 3 -> 4");
    CHECK(list.getTail()->item == 4);
    CHECK(invariantsOk(list));
    CHECK(list.removeNthFromEnd(4));  // cabeça
    CHECK_EQ(list.toString(), "2 -> 3 -> 4");
    CHECK(invariantsOk(list));
    CHECK(!list.removeNthFromEnd(10));
    CHECK(list.size() == 3);
    while (!list.isEmpty()) CHECK(list.removeNthFromEnd(1));
    CHECK(list.getHead() == nullptr && list.getTail() == nullptr);
}

//Operações básicas
void testBasics() {
    std::cout << "Basico: addFirst/addLast/removeFirst/removeLast\n";
    LinkedList<std::string> l;
    l.addLast("b");
    l.addFirst("a");
    l.addLast("c");
    CHECK_EQ(l.toString(), "a -> b -> c");
    CHECK(l.removeLast());
    CHECK(l.removeFirst());
    CHECK_EQ(l.toString(), "b");
    CHECK(l.removeLast());
    CHECK(!l.removeFirst());
    CHECK(!l.removeLast());
    CHECK(invariantsOk(l));
}

int main() {
    testBasics();
    testAddSorted();
    testDeleteNodeWithoutHead();
    testReverse();
    testMerge();
    testRemoveNthFromEnd();

    std::cout << "\n" << (g_total - g_failed) << "/" << g_total
              << " verificacoes passaram.\n";
    if (g_failed == 0) {
        std::cout << "TODOS OS TESTES PASSARAM.\n";
        return 0;
    }
    std::cout << g_failed << " FALHA(S).\n";
    return 1;
}
