#pragma once

#include "LinkedList.hpp"
#include "TreeNode.hpp"

class QuickSort {
private:
    static void swap(TreeNode &value1, TreeNode &value2);
    static void swap(table &value1, table &value2);
    
    static int partition(LinkedList<TreeNode> &list, const int &low, const int &high);
    static int partition(LinkedList<table> &list, const int &low, const int &high);

    static void sort(LinkedList<TreeNode> &list, const int &low, const int &high);
    static void sort(LinkedList<table> &list, const int &low, const int &high);

public:
    static void sort(LinkedList<TreeNode> &list);
    static void sort(LinkedList<table> &list);
};