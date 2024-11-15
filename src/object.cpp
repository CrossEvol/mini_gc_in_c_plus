//
// Created by mizuk on 2024/11/15.
//

#include "include/static/object.h"
#include  <ostream>

std::ostream &operator<<(std::ostream &os, const ObjectType &obj) {
    switch (obj) {
        case ObjectType::INT:
            os << "INT";
            break;
        case ObjectType::PAIR:
            os << "PAIR";
            break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Object &obj) {
    switch (obj.type) {
        case ObjectType::INT:
            os << "Object(" << &obj << ", " << obj.type << ", " << (obj.marked == true ? "true" : "false") << ")";
            break;
        case ObjectType::PAIR:
            os << "Object(" << &obj << ", " << obj.type << ", " << (obj.marked == true ? "true" : "false") << ")";
            break;
    }
    return os;
}
