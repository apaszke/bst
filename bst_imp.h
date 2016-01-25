#include "bst.h"
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

template <typename T>
BST<T>::BST(std::initializer_list<T> list): BST(list.begin(), list.end()) {}

template <typename T>
template <typename Iter>
BST<T>::BST(Iter first, Iter last)
    : BST([&]() {
            std::function<BST<T> (BST<T> const &)> accumulate_tree = [&](BST<T> const &tree) {
                return first == last ? tree : accumulate_tree(tree + *first++);
            };
            return accumulate_tree(BST());
          }()
      ) {}

//TODO: sprawdz czy warunek BST jest zachowany
template <typename T>
BST<T>::BST(T value, BST left, BST right)
    : m_root(NodePtr(new Node(value, left.m_root, right.m_root))) {}

template <typename T>
BST<T> BST<T>::left() const {
    if (empty())
        throw std::logic_error("Can't get a left tree! The tree is empty!");
    return BST<T>(m_root->m_left);
}

template <typename T>
BST<T> BST<T>::right() const {
    if (empty())
        throw std::logic_error("Can't get a right tree! The tree is empty!");
    return BST<T>(m_root->m_right);
}

template <typename T>
T const & BST<T>::value() const {
    if (empty())
        throw std::logic_error("Can't get node's value! The tree is empty!");
    return m_root->m_value;
}

template <typename T>
bool BST<T>::empty() const {
    return !m_root;
}

template <typename T>
T const & BST<T>::min() const {
    if (!m_root)
        throw std::logic_error("Can't find min in an empty tree!");
    const auto& min = static_cast<T const & (*)(T const &, T const &)>(std::min<T>);
    return fold<T const &>(value(), min);
}

template <typename T>
T const & BST<T>::max() const {
    if (!m_root)
        throw std::logic_error("Can't find max in an empty tree!");
    const auto& max = static_cast<T const & (*)(T const &, T const &)>(std::max<T>);
    return fold<T const &>(value(), max);
}

#include <iostream>
using namespace std;
template <typename T>
template <typename Acc, typename Functor>
Acc BST<T>::fold(Acc a, Functor f) const {
    if (!m_root)
        return a;
    std::function<Acc (const Acc&, const NodePtr&)> fold_node =
        [&](const Acc& a, const NodePtr& node) {
            cout << node.get() << " " << !!node << endl;
            return node
                    ? fold_node(f(fold_node(a, node->m_left), node->m_value), node->m_right)
                    : a;
        };
    return fold_node(a, m_root);
}

template <typename T>
BST<T> BST<T>::find(T const & t) const {
    std::function<BST<T> (NodePtr const &)> find_node = [&](NodePtr const & n) {
        if (!n)                    return BST();
        else if (t == n->m_value)  return BST(n);
        else if (t < n->m_value)   return find_node(n->m_left);
        else /*  t > n->m_value */ return find_node(n->m_right);
    };
    return find_node(m_root);
}

template <typename T>
std::size_t BST<T>::size() const {
    return fold(std::size_t(), [](std::size_t total, T) { return total + 1; });
}

template <typename T>
std::size_t BST<T>::height() const {
    std::function<std::size_t (const NodePtr&)> subtree_height =
        [&](const NodePtr& n) {
            return n
                ? 1 + std::max(subtree_height(n->m_right), subtree_height(n->m_left))
                : 0;
        };
    return subtree_height(m_root);
}

template <typename T>
BST<T> spine(BST<T> tree) {
    return tree.fold(BST<T>(), [](BST<T> const &tree, T const &value) {
                                   return BST<T>(value, tree, BST<T>());
                               });
}

template <typename T>
BST<T> operator+(BST<T> tree, T value) {
    if (tree.empty())
        return BST<T>(value, BST<T>(), BST<T>());

    auto& node_val = tree.value();
    return node_val < value
           ? BST<T>(node_val, tree.left(), tree.right() + value)
           : BST<T>(node_val, tree.left() + value, tree.right());
}

template <typename T>
BST<T> operator+(T value, BST<T> tree) {
    return tree + value;
}

template <typename T>
std::ostream & operator<<(std::ostream &stream, BST<T> tree) {
    if (tree.empty())
        return stream;
    return stream << tree.left() << tree.value() << " " << tree.right();
}

template <typename T>
T max_diff(BST<T> tree) {
    using pair = std::tuple<T, T, std::size_t>;
    //TODO: lepiej referencje czy nie?
    auto total = static_cast<T const & (*)(pair const &)>(std::get<0, T, T, std::size_t>);
    auto previous = static_cast<T const & (*)(pair const &)>(std::get<1, T, T, std::size_t>);
    auto iter_count = static_cast<std::size_t const & (*)(pair const &)>(std::get<2, T, T, std::size_t>);
    //TODO; lepiej value przez & czy nie?
    auto fold_fn = [&](const pair& p, T value) {
        if (iter_count(p) == 1)
            return pair(value, value, 2);
        else if (iter_count(p) == 2)
            return pair(value - previous(p), value, 3);
        else
            return pair(std::max(total(p), value - previous(p)), value, iter_count(p)+1);
    };
    return total(tree.fold(pair(tree.value(), tree.value(), true), fold_fn));
}
