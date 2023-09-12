#pragma once

#include <algorithm>
#include <functional>
template <typename T, class compare = std::less<T>>
class PriorityQueue {
public:
    PriorityQueue() : size_(0) {}
    ~PriorityQueue();
    T Top();
    void Pop();
    void Push(T value);
    bool Empty();
    size_t Size();
    PriorityQueue<T, compare> Merge(PriorityQueue<T, compare>& A, PriorityQueue<T, compare>& B);

private:
    struct Node {
        T value;
        Node* L;
        Node* R;
    };
    size_t size_;
    Node* root_ = nullptr;
    Node* Merge(Node* A, Node* B);
};

template <typename T, class compare>
T PriorityQueue<T, compare>::Top() {
    T answer = root_->value;
    return answer;
}

template <typename T, class compare>
void PriorityQueue<T, compare>::Pop() {
    root_ = Merge(root_->L, root_->R);
    --size_;
}

template <typename T, class compare>
void PriorityQueue<T, compare>::Push(T value) {
    Node* a = new Node;
    a->value = value;
    a->L = nullptr;
    a->R = nullptr;
    root_ = Merge(root_, a);
    ++size_;
}

template <typename T, class compare>
PriorityQueue<T, compare> PriorityQueue<T, compare>::Merge(PriorityQueue<T, compare>& A, PriorityQueue<T, compare>& B) {
    PriorityQueue<T, compare> new_queue;
    new_queue.root = Merge(A.root_, B.root_);
    new_queue.size_ = A.size_ + B.size_;
    return new_queue;
}

template <typename T, class compare>
typename PriorityQueue<T, compare>::Node* PriorityQueue<T, compare>::Merge(PriorityQueue<T, compare>::Node* A,
                                                                           PriorityQueue<T, compare>::Node* B) {
    if (A == nullptr || B == nullptr) {
        return (A != nullptr ? A : B);
    }
    if (compare()(B->value, A->value)) {
        std::swap(A, B);
    }
    if ((rand() & 1) == 1) {
        A->L = Merge(A->L, B);
    } else {
        A->R = Merge(A->R, B);
    }
    return A;
}

template <typename T, class compare>

bool PriorityQueue<T, compare>::Empty() {
    return size_ == 0;
}
template <typename T, class compare>
size_t PriorityQueue<T, compare>::Size() {
    return size_;
}
template <typename T, class compare>
PriorityQueue<T, compare>::~PriorityQueue() {
    delete root_;
}
