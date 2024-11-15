//
// Created by mizuk on 2024/11/15.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <ostream>
#include <utility>
#include <variant>

enum class ObjectType {
    INT, PAIR
};

struct Object;

using ObjectPair = std::pair<Object *, Object *>;

struct Object {
    ObjectType type;
    bool marked;
    std::variant<int, ObjectPair, std::monostate> value;

    Object(ObjectType t, bool m): type(t), marked(m), value(std::monostate{}) {
    }
};

struct ObjectNode {
    Object *value;
    ObjectNode *next;

    ObjectNode(Object *v, ObjectNode *n): value(v), next(n) {
    }
};

std::ostream &operator<<(std::ostream &os, const ObjectType &obj);

std::ostream &operator<<(std::ostream &os, const Object &obj);

#endif //OBJECT_H
