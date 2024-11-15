//
// Created by mizuk on 2024/11/15.
//

#ifndef VM_H
#define VM_H

#include <vector>
#include <string>
#include "object.h"

constexpr int STACK_MAX = 256;
constexpr int INIT_OBJ_NUM_MAX = 8;


class VM {
    struct Heap {
        ObjectNode *firstObject;
        int size;
        int capacity;

        Heap(): firstObject(nullptr), size(0), capacity(INIT_OBJ_NUM_MAX) {
        }
    };

    std::vector<Object *> stack;
    Heap heap;

    void push(Object *object);

    void mark(Object *object);

    Object *newObject(ObjectType type);

public:
    VM(): stack(0), heap() {
    }

    ~VM();

    [[nodiscard]] int getSize() const {
        return heap.size;
    }

    void debugPrint();

    friend std::ostream &operator<<(std::ostream &os, const VM &vm);

    Object *pop();

    void pushInt(int intValue);

    Object *pushPair();

    void markAll();

    void sweep();

    void gc();
};

class StackOverflowException final : public std::exception {
public:
    const std::string message;

    StackOverflowException(): message("Stack overflow!") {
    }
};

class StackUnderflowException final : public std::exception {
public:
    const std::string message;

    StackUnderflowException(): message("Stack underflow!") {
    }
};

#endif //VM_H
