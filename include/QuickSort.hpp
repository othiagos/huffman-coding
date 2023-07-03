#pragma once

#include "LinkedList.hpp"
#include "TreeNode.hpp"

/**
 * @class QuickSort
 * @brief Represents the QuickSort algorithm for sorting.
 */
class QuickSort {
private:

    /**
     * @brief Swaps two TreeNode pointers.
     * @param value1 Pointer to the first TreeNode.
     * @param value2 Pointer to the second TreeNode.
     */
    static void swap(TreeNode *value1, TreeNode *value2);

    /**
     * @brief Swaps two table objects.
     * @param value1 First table object.
     * @param value2 Second table object.
     */
    static void swap(table &value1, table &value2);

    /**
     * @brief Partitions the LinkedList of TreeNode pointers.
     * @param list The LinkedList to be partitioned.
     * @param low The index of the lower boundary of the partition.
     * @param high The index of the upper boundary of the partition.
     * @return The index of the partition.
     */
    static int partition(LinkedList<TreeNode*> &list, const int &low, const int &high);

    /**
     * @brief Partitions the LinkedList of table objects.
     * @param list The LinkedList to be partitioned.
     * @param low The index of the lower boundary of the partition.
     * @param high The index of the upper boundary of the partition.
     * @return The index of the partition.
     */
    static int partition(LinkedList<table> &list, const int &low, const int &high);

    /**
     * @brief Performs the QuickSort algorithm on the LinkedList of TreeNode pointers.
     * @param list The LinkedList to be sorted.
     * @param low The index of the lower boundary of the subarray to be sorted.
     * @param high The index of the upper boundary of the subarray to be sorted.
     */
    static void sort(LinkedList<TreeNode*> &list, const int &low, const int &high);

    /**
     * @brief Performs the QuickSort algorithm on the LinkedList of table objects.
     * @param list The LinkedList to be sorted.
     * @param low The index of the lower boundary of the subarray to be sorted.
     * @param high The index of the upper boundary of the subarray to be sorted.
     */
    static void sort(LinkedList<table> &list, const int &low, const int &high);

public:

    /**
     * @brief Sorts the LinkedList of TreeNode pointers using the QuickSort algorithm.
     * @param list The LinkedList to be sorted.
     */
    static void sort(LinkedList<TreeNode*> &list);

    /**
     * @brief Sorts the LinkedList of table objects using the QuickSort algorithm.
     * @param list The LinkedList to be sorted.
     */
    static void sort(LinkedList<table> &list);
};