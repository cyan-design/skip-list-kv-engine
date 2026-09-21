#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

struct Node {
    int key;
    std::string value;
    std::vector<Node*> next;
    Node(int k, const std::string& v, int level);
};

class SkipList {
public:
    SkipList(int maxLevel = 16);
    ~SkipList();

    void insert(int key, const std::string& val);
    bool search(int key, std::string& outVal);
    bool update(int key, const std::string& newVal);
    bool remove(int key);
    void printList();

    void dump(const std::string& filename);
    void load(const std::string& filename);

    // 新增：统计信息
    int size();               // 返回节点数量
    int levelCount();         // 返回当前跳表层数

    // 新增：范围查询
    void rangeQuery(int startKey, int endKey, std::vector<std::pair<int, std::string>>& result);

    // 新增：打印每一层结构
    void printLevels();

private:
    int maxLevel;
    int curLevel;
    int nodeCount;            // 记录节点数量
    Node* head;
    int randomLevel();
};

#endif
