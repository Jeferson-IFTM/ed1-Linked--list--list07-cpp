#ifndef LIST_H
#define LIST_H

#include <cstddef>
#include <string>

/**
 * Interface (classe abstrata) de uma lista linear.
 * LinkedList<T> (LinkedList.h) é a implementação encadeada simples.
 */
template <typename T>
class List {
public:
    virtual ~List() = default;

    virtual void addFirst(const T& item) = 0;
    virtual void addLast(const T& item) = 0;

    // Retornam false quando a lista está vazia.
    virtual bool removeFirst() = 0;
    virtual bool removeLast() = 0;

    virtual void clear() = 0;
    virtual std::size_t size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual std::string toString() const = 0;
};

#endif // LIST_H
