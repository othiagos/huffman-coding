#include "QuickSort.hpp"

void QuickSort::swap(TreeNodeChar &value1, TreeNodeChar &value2) {
    TreeNodeChar temp = value1;
    value1 = value2;
    value2 = temp;
}

int QuickSort::partition(LinkedList<TreeNodeChar> &list, const int &low, const int &high) {
    TreeNodeChar x = list[high];
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

void QuickSort::sort(LinkedList<TreeNodeChar> &list, const int &low, const int &high) {
    if (low < high) {
        int p = partition(list, low, high);
        sort(list, low, p - 1);
        sort(list, p + 1, high);
    }
}

void QuickSort::sort(LinkedList<TreeNodeChar> &list) {
    sort(list, 0, list.size() - 1);
}
