#include <catch2/catch_test_macros.hpp>
#include "../src/include/static/vm.h"

TEST_CASE("VM Stack Operations", "[vm]") {
    SECTION("Stack push and pop work correctly") {
        VM vm{};
        vm.pushInt(1);
        vm.pushInt(2);

        auto *obj = vm.pop();
        REQUIRE(std::get<int>(obj->value) == 2);

        obj = vm.pop();
        REQUIRE(std::get<int>(obj->value) == 1);

        REQUIRE_THROWS_AS(vm.pop(), StackUnderflowException);
    }

    SECTION("Stack overflow is detected") {
        VM vm{};
        for (int i = 0; i <= STACK_MAX; i++) {
            vm.pushInt(i);
        }
        REQUIRE_THROWS_AS(vm.pushInt(999), StackOverflowException);
    }
}

TEST_CASE("Garbage Collection", "[gc]") {
    SECTION("Objects on stack are preserved") {
        VM vm{};
        vm.pushInt(1);
        vm.pushInt(2);

        auto initialSize = vm.getSize();
        vm.debugPrint();
        vm.gc();
        vm.debugPrint();
        REQUIRE(vm.getSize() == initialSize);
    }

    SECTION("Unreached objects are collected") {
        VM vm{};
        vm.pushInt(1);
        vm.pushInt(2);
        vm.pop();
        vm.pop();

        vm.debugPrint();
        vm.gc();
        vm.debugPrint();
        REQUIRE(vm.getSize() == 0);
    }

    SECTION("Nested objects are properly marked") {
        VM vm{};
        vm.pushInt(1);
        vm.pushInt(2);
        vm.pushPair(); // Creates pair (1,2)
        vm.pushInt(3);
        vm.pushInt(4);
        vm.pushPair(); // Creates pair (3,4)
        vm.pushPair(); // Creates pair ((1,2), (3,4))

        size_t initialSize = vm.getSize();
        vm.gc();
        REQUIRE(initialSize == 7);
        REQUIRE(vm.getSize() == initialSize);
    }

    SECTION("Cyclic references are handled") {
        VM vm{};
        vm.pushInt(1);
        vm.pushInt(2);
        auto *pairA = vm.pushPair();

        vm.pushInt(3);
        vm.pushInt(4);
        auto *pairB = vm.pushPair();

        // Create cycle: pairA->tail points to pairB, pairB->tail points to pairA
        auto &pairAValue = std::get<ObjectPair>(pairA->value);
        auto &pairBValue = std::get<ObjectPair>(pairB->value);
        pairAValue.second = pairB;
        pairBValue.second = pairA;

        vm.debugPrint();
        vm.gc();
        vm.debugPrint();
        REQUIRE(vm.getSize() == 4); // Objects should be preserved due to being reachable
    }
}

TEST_CASE("Object Creation and Value Storage", "[object]") {
    SECTION("Integer objects store correct values") {
        VM vm{};
        vm.pushInt(42);
        auto *obj = vm.pop();
        REQUIRE(obj->type == ObjectType::INT);
        REQUIRE(std::get<int>(obj->value) == 42);
    }

    SECTION("Pair objects store correct values") {
        VM vm{};
        vm.pushInt(1);
        vm.pushInt(2);
        auto *pair = vm.pushPair();

        REQUIRE(pair->type == ObjectType::PAIR);
        auto &pairValue = std::get<ObjectPair>(pair->value);
        REQUIRE(std::get<int>(pairValue.first->value) == 1);
        REQUIRE(std::get<int>(pairValue.second->value) == 2);
    }
}
