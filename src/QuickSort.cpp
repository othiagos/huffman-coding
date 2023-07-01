#include "QuickSort.hpp"

void QuickSort::swap(TreeNode &value1, TreeNode &value2) {
    TreeNode temp = value1;
    value1 = value2;
    value2 = temp;
}

void QuickSort::swap(table &value1, table &value2) {
    table temp = value1;
    value1 = value2;
    value2 = temp;
}

int QuickSort::partition(LinkedList<TreeNode> &list, const int &low, const int &high) {
    TreeNode x = list[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (list[j].get_count() < x.get_count()) {
            i++;
            swap(list[i], list[j]);
        }
    }
    swap(list[i + 1], list[high]);
    return i + 1;
}

int QuickSort::partition(LinkedList<table> &list, const int &low, const int &high) {
    table x = list[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (list[j].chars.compare(x.chars) < 0) {
            i++;
            swap(list[i], list[j]);
        }
    }
    swap(list[i + 1], list[high]);
    return i + 1;
}

void QuickSort::sort(LinkedList<TreeNode> &list, const int &low, const int &high) {
    if (low < high) {
        int p = partition(list, low, high);
        sort(list, low, p - 1);
        sort(list, p + 1, high);
    }
}

void QuickSort::sort(LinkedList<table> &list, const int &low, const int &high) {
    if (low < high) {
        int p = partition(list, low, high);
        sort(list, low, p - 1);
        sort(list, p + 1, high);
    }
}

void QuickSort::sort(LinkedList<TreeNode> &list) {
    sort(list, 0, list.size() - 1);
}

void QuickSort::sort(LinkedList<table> &list) {
    sort(list, 0, list.size() - 1);
}
