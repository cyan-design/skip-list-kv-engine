#include "skiplist.h"
#include <fstream>

Node::Node(int k, const std::string& v, int level)
    : key(k), value(v), next(level, nullptr) {}

SkipList::SkipList(int maxL)
    : maxLevel(maxL), curLevel(0), nodeCount(0) {
    head = new Node(-1, "", maxLevel);
    srand((unsigned)time(nullptr));
}

SkipList::~SkipList() {
    Node* p = head->next[0];
    while (p != nullptr) {
        Node* tmp = p;
        p = p->next[0];
        delete tmp;
    }
    delete head;
}

int SkipList::randomLevel() {
    int lvl = 1;
    while (rand() % 2 == 0 && lvl < maxLevel) {
        lvl++;
    }
    return lvl;
}

void SkipList::insert(int key, const std::string& val) {
    std::vector<Node*> update(maxLevel, nullptr);
    Node* p = head;

    for (int i = curLevel - 1; i >= 0; i--) {
        while (p->next[i] != nullptr && p->next[i]->key < key) {
            p = p->next[i];
        }
        update[i] = p;
    }

    p = p->next[0];
    if (p != nullptr && p->key == key) {
        p->value = val;
        return;
    }

    int newLv = randomLevel();
    if (newLv > curLevel) {
        for (int i = curLevel; i < newLv; i++) {
            update[i] = head;
        }
        curLevel = newLv;
    }

    Node* newNode = new Node(key, val, newLv);
    for (int i = 0; i < newLv; i++) {
        newNode->next[i] = update[i]->next[i];
        update[i]->next[i] = newNode;
    }

    nodeCount++;
}

bool SkipList::search(int key, std::string& outVal) {
    Node* p = head;

    for (int i = curLevel - 1; i >= 0; i--) {
        while (p->next[i] != nullptr && p->next[i]->key < key) {
            p = p->next[i];
        }
    }

    p = p->next[0];
    if (p != nullptr && p->key == key) {
        outVal = p->value;
        return true;
    }
    return false;
}

bool SkipList::update(int key, const std::string& newVal) {
    Node* p = head;

    for (int i = curLevel - 1; i >= 0; i--) {
        while (p->next[i] != nullptr && p->next[i]->key < key) {
            p = p->next[i];
        }
    }

    p = p->next[0];
    if (p != nullptr && p->key == key) {
        p->value = newVal;
        return true;
    }
    return false;
}

bool SkipList::remove(int key) {
    std::vector<Node*> update(maxLevel, nullptr);
    Node* p = head;

    for (int i = curLevel - 1; i >= 0; i--) {
        while (p->next[i] != nullptr && p->next[i]->key < key) {
            p = p->next[i];
        }
        update[i] = p;
    }

    p = p->next[0];
    if (p == nullptr || p->key != key) return false;

    for (int i = 0; i < curLevel; i++) {
        if (update[i]->next[i] != p) break;
        update[i]->next[i] = p->next[i];
    }

    delete p;
    nodeCount--;

    while (curLevel > 0 && head->next[curLevel - 1] == nullptr) {
        curLevel--;
    }

    return true;
}

void SkipList::printList() {
    Node* p = head->next[0];
    while (p != nullptr) {
        std::cout << "[" << p->key << ":" << p->value << "] ";
        p = p->next[0];
    }
    std::cout << "\n";
}

void SkipList::dump(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "dump文件打开失败！" << std::endl;
        return;
    }

    Node* p = head->next[0];
    while (p != nullptr) {
        out << p->key << " " << p->value << "\n";
        p = p->next[0];
    }

    out.close();
    std::cout << "数据已dump保存到 " << filename << std::endl;
}

void SkipList::load(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "load文件不存在，跳过加载" << std::endl;
        return;
    }

    int k;
    std::string v;
    while (in >> k >> v) {
        insert(k, v);
    }

    in.close();
    std::cout << "从文件加载数据完成" << std::endl;
}

int SkipList::size() {
    return nodeCount;
}

int SkipList::levelCount() {
    return curLevel;
}

void SkipList::rangeQuery(int startKey, int endKey, std::vector<std::pair<int, std::string>>& result) {
    result.clear();
    Node* p = head;

    for (int i = curLevel - 1; i >= 0; i--) {
        while (p->next[i] != nullptr && p->next[i]->key < startKey) {
            p = p->next[i];
        }
    }

    p = p->next[0];
    while (p != nullptr && p->key <= endKey) {
        result.push_back({p->key, p->value});
        p = p->next[0];
    }
}

void SkipList::printLevels() {
    for (int i = curLevel - 1; i >= 0; i--) {
        std::cout << "Level " << i << ": ";
        Node* p = head->next[i];
        while (p != nullptr) {
            std::cout << "[" << p->key << "] ";
            p = p->next[i];
        }
        std::cout << "\n";
    }
}
