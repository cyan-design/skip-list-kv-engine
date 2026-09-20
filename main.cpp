#include "skip_list_kv.h"
#include <iostream>

int main() {
    SkipList<int, std::string> kv;

    // 插入测试数据
    kv.insert(1, "apple");
    kv.insert(3, "banana");
    kv.insert(5, "orange");
    kv.insert(2, "grape");

    std::cout << "插入数据完成\n";
    kv.printList();

    // 查询
    auto res = kv.search(3);
    if(res.has_value()) {
        std::cout << "查询key=3: " << res.value() << "\n";
    }

    // 修改
    kv.insert(3, "blueberry");
    std::cout << "更新key=3之后：\n";
    kv.printList();

    // 删除
    kv.remove(5);
    std::cout << "删除key=5之后：\n";
    kv.printList();

    return 0;
}
