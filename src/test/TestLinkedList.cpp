#include "doctest.h"
#include "LinkedList.hpp"

TEST_CASE("Check size") {
    LinkedList<int> list;

    list.push_back(1);
    list.push_back(3);
    list.push_back(5);

    CHECK(list.size() == 3);
}

TEST_CASE("Check size") {
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(3);
    list.push_front(5);

    CHECK(list.size() == 3);
}

TEST_CASE("Check push back") {
    LinkedList<int> list;

    list.push_back(1);
    list.push_back(3);
    list.push_back(5);

    CHECK(list.get_item(0) == 1);
    CHECK(list.get_item(1) == 3);
    CHECK(list.get_item(2) == 5);
}

TEST_CASE("Check push front") {
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(3);
    list.push_front(5);

    CHECK(list.get_item(0) == 5);
    CHECK(list.get_item(1) == 3);
    CHECK(list.get_item(2) == 1);
}

TEST_CASE("Check pop back") {
    LinkedList<int> list;

    list.push_back(1);
    list.push_back(3);
    list.push_back(5);

    CHECK(list.pop_back() == 5);
    CHECK(list.pop_back() == 3);
    CHECK(list.pop_back() == 1);
    CHECK(list.size() == 0);
}

TEST_CASE("Check pop front") {
    LinkedList<int> list;

    list.push_back(1);
    list.push_back(3);
    list.push_back(5);

    CHECK(list.pop_front() == 1);
    CHECK(list.pop_front() == 3);
    CHECK(list.pop_front() == 5);
    CHECK(list.size() == 0);
}

TEST_CASE("Check set item") {
    LinkedList<int> list;

    list.push_back(1);
    list.push_back(3);
    list.push_back(5);

    list.set_item(6, 0);
    list.set_item(7, 1);
    list.set_item(8, 2);

    CHECK(list.pop_front() == 6);
    CHECK(list.pop_front() == 7);
    CHECK(list.pop_front() == 8);
}

TEST_CASE("Check insert") {
    LinkedList<int> list;

    list.push_back(1);
    list.push_back(3);
    list.push_back(5);

    list.insert(8, 3);
    list.insert(7, 2);
    list.insert(6, 0);

    CHECK(list.get_item(0) == 6);
    CHECK(list.get_item(1) == 1);
    CHECK(list.get_item(2) == 3);
    CHECK(list.get_item(3) == 7);
    CHECK(list.get_item(4) == 5);
    CHECK(list.get_item(5) == 8);
}

TEST_CASE("Check erase") {
    LinkedList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    CHECK(list.erase(4) == 5);
    CHECK(list.erase(2) == 3);
    CHECK(list.erase(0) == 1);
    CHECK(list.size() == 2);
}

TEST_CASE("Check operator==") {
    LinkedList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    list[2] = 8;
    list[0] = list[2];

    CHECK(list[0] == 8);
    CHECK(list[2] == 8);
    CHECK(list[4] == 5);
}

TEST_CASE("Check iterator") {
    LinkedList<int> list;

    list.push_back(1);
    list.push_back(2);

    for (int i : list) {
        list.push_back(i * 3);
    }

    CHECK(list[0] == 1);
    CHECK(list[1] == 2);
    CHECK(list[2] == 3);
    CHECK(list[3] == 6);
}

TEST_CASE("Check initializer list") {
    LinkedList<int> list{ 3, 4, 5};

    CHECK(list[0] == 3);
    CHECK(list[1] == 4);
    CHECK(list[2] == 5);
}

TEST_CASE("Check throw empty list") {
    LinkedList<int> list;

    CHECK_THROWS_AS(list.pop_back(), llexcp::EmptyList);
}

TEST_CASE("Check throw empty list") {
    LinkedList<int> list;

    CHECK_THROWS_AS(list.pop_front(), llexcp::EmptyList);
}

TEST_CASE("Check throw empty list") {
    LinkedList<int> list;

    CHECK_THROWS_AS(list.erase(0), llexcp::EmptyList);
}

TEST_CASE("Check throw invalid position") {
    LinkedList<int> list;

    CHECK_THROWS_AS(list.get_item(0), llexcp::InvalidPosition);
}

TEST_CASE("Check throw invalid position") {
    LinkedList<int> list;

    CHECK_THROWS_AS(list.get_item(-1), llexcp::InvalidPosition);
}

TEST_CASE("Check throw invalid position") {
    LinkedList<int> list;

    CHECK_THROWS_AS(list.get_item(3), llexcp::InvalidPosition);
}

TEST_CASE("Check throw invalid position") {
    LinkedList<int> list;

    CHECK_THROWS_AS(list.insert(1, -1), llexcp::InvalidPosition);
}

TEST_CASE("Check throw invalid position") {
    LinkedList<int> list;

    CHECK_THROWS_AS(list.insert(1, 1), llexcp::InvalidPosition);
}