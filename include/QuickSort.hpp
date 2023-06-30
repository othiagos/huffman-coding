#pragma once

#include "LinkedList.hpp"
#include "TreeNodeChar.hpp"

class QuickSort {
private:
    static void swap(TreeNodeChar &value1, TreeNodeChar &value2);
    static void swap(table &value1, table &value2);
    
    static int partition(LinkedList<TreeNodeChar> &list, const int &low, const int &high);
    static int partition(LinkedList<table> &list, const int &low, const int &high);

    static void sort(LinkedList<TreeNodeChar> &list, const int &low, const int &high);
    static void sort(LinkedList<table> &list, const int &low, const int &high);

public:
    static void sort(LinkedList<TreeNodeChar> &list);
    static void sort(LinkedList<table> &list);
};