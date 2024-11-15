//
// Created by mizuk on 2024/11/15.
//

#include "include/static/vm.h"

#include <iostream>

#include "fmt/xchar.h"
#include "ostream"

#define DEBUG_PRINT

std::ostream &operator<<(std::ostream &os, const VM &vm) {
    os << "== stack ==" << std::endl;
    for (const auto object: vm.stack) {
        os << *object << std::endl;
    }
    os << "== heap ==" << std::endl;
    auto root = vm.heap.firstObject;
    while (root != nullptr) {
        std::cout << *root->value << std::endl;
        root = root->next;
    }
    os << "== ... ==" << std::endl;
    return os;
}

void VM::debugPrint() {
#ifdef DEBUG_PRINT
    std::cout << *this << std::endl;
#endif
}

VM::~VM() {
    // Clean up allocated objects
    ObjectNode *current = this->heap.firstObject;
    while (current != nullptr) {
        ObjectNode *next = current->next;
        delete current->value;
        delete current;
        current = next;
    }
}

void VM::push(Object *object) {
    if (this->stack.size() > STACK_MAX) throw StackOverflowException();
    this->stack.push_back(object);
}

void VM::mark(Object *object) {
    if (!object || object->marked) return;

    object->marked = true;
    if (object->type == ObjectType::PAIR) {
        auto [first, second] = std::get<ObjectPair>(object->value);
        this->mark(first);
        this->mark(second);
    }
}

Object *VM::newObject(ObjectType type) {
    if (this->heap.size == this->heap.capacity) this->gc();

    auto *object = new Object(type, false);
    auto *node = new ObjectNode(object, this->heap.firstObject);
    this->heap.firstObject = node;
    this->heap.size++;
    return object;
}

Object *VM::pop() {
    if (this->stack.empty()) throw StackUnderflowException();
    Object *last = this->stack.back();
    this->stack.pop_back();
    return last;
}

void VM::pushInt(int intValue) {
    Object *object = this->newObject(ObjectType::INT);
    object->value = intValue;
    this->push(object);
}

Object *VM::pushPair() {
    Object *object = this->newObject(ObjectType::PAIR);
    Object *tail = this->pop();
    Object *head = this->pop();
    object->value = std::make_pair(head, tail);
    this->push(object);
    return object;
}

void VM::markAll() {
    for (auto *object: this->stack) {
        this->mark(object);
    }
}

void VM::sweep() {
    ObjectNode **head = &this->heap.firstObject;
    while (*head) {
        if (!(*head)->value->marked) {
            ObjectNode *unreached = *head;
            *head = unreached->next;
            delete unreached->value;
            delete unreached;
            this->heap.size--;
        } else {
            (*head)->value->marked = false;
            head = &(*head)->next;
        }
    }
}

void VM::gc() {
    const auto preSize = this->heap.size;
    this->markAll();
    this->sweep();
    this->heap.capacity = this->heap.size == 0 ? INIT_OBJ_NUM_MAX : this->heap.size * 2;
    fmt::print("Collected {} objects, {} remaining.\n", preSize - this->heap.size, this->heap.size);
}
