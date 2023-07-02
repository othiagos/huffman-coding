#include "doctest.h"
#include "QuickSort.hpp"
#include "LinkedList.hpp"
#include "TreeNode.hpp"

TEST_CASE("Check sort") {
    LinkedList<TreeNode*> list;

    list.push_back(new TreeNode(3));
    list.push_back(new TreeNode(1));
    list.push_back(new TreeNode(2));

    QuickSort::sort(list);
    
    CHECK((
        list[0]->get_count() == 1 && 
        list[1]->get_count() == 2 &&
        list[2]->get_count() == 3
        ));
}

TEST_CASE("Check sort") {
    LinkedList<table> list;

    list.push_back(table({"c", "100"}));
    list.push_back(table({"a", "001"}));
    list.push_back(table({"b", "010"}));

    QuickSort::sort(list);
    
    CHECK((
        list[0].chars == "a" && 
        list[1].chars == "b" &&
        list[2].chars == "c"
        ));
}