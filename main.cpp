#include "skiplist.h"

int main() {
    SkipList sk(16);

    sk.load("dump.txt");

    sk.insert(1, "apple");
    sk.insert(2, "grape");
    sk.insert(3, "banana");
    sk.insert(5, "orange");
    sk.insert(7, "peach");
    sk.insert(9, "lemon");

    std::cout << "插入数据完成\n";

    std::cout << "底层链表：";
    sk.printList();

    std::cout << "当前节点数量：" << sk.size() << "\n";
    std::cout << "当前跳表层数：" << sk.levelCount() << "\n";

    std::string val;
    if (sk.search(3, val)) {
        std::cout << "查询key=3：" << val << "\n";
    }

    sk.update(3, "blueberry");
    std::cout << "更新key=3之后：\n";
    std::cout << "底层链表：";
    sk.printList();

    sk.remove(5);
    std::cout << "删除key=5之后：\n";
    std::cout << "底层链表：";
    sk.printList();

    std::cout << "\n跳表各层结构：\n";
    sk.printLevels();

    std::cout << "\n范围查询 key 2 到 7：\n";
    std::vector<std::pair<int, std::string>> rangeResult;
    sk.rangeQuery(2, 7, rangeResult);
    for (auto& item : rangeResult) {
        std::cout << "key=" << item.first << ", value=" << item.second << "\n";
    }

    std::cout << "\n最终节点数量：" << sk.size() << "\n";

    sk.dump("dump.txt");

    return 0;
}
