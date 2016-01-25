#include "bst_imp.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {

    {
        BST<int> bst;
        assert(bst.empty());
    }
    cout << 0 << endl;

    {
        BST<int> bst({3});
        // 3
        cout << bst << endl;
        assert(bst.value() == 3);
    }

    cout << 1 << endl;

    {
        BST<int> bst({3, 4});
        // 3    |
        //  \   |
        //   4  |
        cout << bst << endl;
        assert(bst.value() == 3);
        assert(bst.right().value() == 4);
    }

    cout << 2 << endl;

    {
        BST<int> bst({3, 4, 1});
        //   3    |
        //  / \   |
        // 1   4  |
        cout << bst << endl;
        assert(bst.size() == 3);
        assert(bst.value() == 3);
        assert(bst.right().value() == 4);
        assert(bst.left().value() == 1);
    }

    cout << 3 << endl;

    {
        BST<int> bst({3, 4, 1, 2});
        //    3    |
        //   / \   |
        //  1   4  |
        //   \     |
        //    2    |
        cout << bst << endl;
        assert(bst.size() == 4);
        assert(bst.value() == 3);
        assert(bst.right().value() == 4);
        assert(bst.left().value() == 1);
        assert(bst.left().right().value() == 2);
    }

    cout << 4 << endl;

    {
        BST<int> bst({3,4,1,2,7});
        //    3      |
        //   / \     |
        //  1   4    |
        //   \   \   |
        //    2   7  |
        cout << bst << endl;
        assert(bst.size() == 5);
        assert(bst.value() == 3);
        assert(bst.right().value() == 4);
        assert(bst.left().value() == 1);
        assert(bst.left().right().value() == 2);
        assert(bst.right().right().value() == 7);
    }

    BST<int> bst({3,4,1,2,7,3});
    //    3          |
    //   / \         |
    //  1   4        |
    //   \   \       |
    //    2   7      |
    //     \         |
    //      3        |

    std::cout << bst << std::endl; // prints 1 2 3 3 4 7
    assert(bst.size() == 6);
    cout << bst.size() << endl;
    assert(bst.min() == 1);
    assert(bst.max() == 7);
    assert(bst.height() == 4);
    assert(spine(bst).height() == 6);
    assert(max_diff(bst) == 3);
    std::cout << bst.find(4) << std::endl; // prints 4 7
    std::cout << bst.find(11) << std::endl; //prints nothing (possibly one space)
    std::cout << max_diff(bst) << std::endl; //prints 3

    return 0;
}
