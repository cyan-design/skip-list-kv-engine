#ifndef SKIP_LIST_KV_H
#define SKIP_LIST_KV_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <optional>

// 跳表节点
template<typename K, typename V>
struct Node {
    K key;
    V value;
    std::vector<Node*> next;

    Node(K k, V v, int level) : key(k), value(v), next(level, nullptr) {}
};

template<typename K, typename V>
class SkipList {
private:
    int maxLevel;
    int currentLevel;
    Node<K, V>* head;
    const double probability = 0.5;

    // 随机生成层数
    int randomLevel() {
        int level = 1;
        while ((static_cast<double>(rand()) / RAND_MAX) < probability && level < maxLevel) {
            level++;
        }
        return level;
    }

public:
    SkipList(int maxLvl = 16) : maxLevel(maxLvl), currentLevel(1) {
        srand((unsigned)time(nullptr));
        // 头节点，最大层数
        head = new Node<K, V>(K{}, V{}, maxLevel);
    }

    ~SkipList() {
        Node<K, V>* cur = head;
        while (cur) {
            Node<K, V>* nextNode = cur->next[0];
            delete cur;
            cur = nextNode;
        }
    }

    // 插入key-value
    void insert(K key, V value) {
        std::vector<Node<K, V>*> update(maxLevel, nullptr);
        Node<K, V>* p = head;

        // 从最高层往下找
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (p->next[i] && p->next[i]->key < key) {
                p = p->next[i];
            }
            update[i] = p;
        }

        // 已存在，直接更新value
        if (p->next[0] && p->next[0]->key == key) {
            p->next[0]->value = value;
            return;
        }

        int newLevel = randomLevel();
        if (newLevel > currentLevel) {
            for (int i = currentLevel; i < newLevel; i++) {
                update[i] = head;
            }
            currentLevel = newLevel;
        }

        Node<K, V>* newNode = new Node<K, V>(key, value, newLevel);
        for (int i = 0; i < newLevel; i++) {
            newNode->next[i] = update[i]->next[i];
            update[i]->next[i] = newNode;
        }
    }

    // 查询，返回std::optional，找不到返回空
    std::optional<V> search(K key) {
        Node<K, V>* p = head;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (p->next[i] && p->next[i]->key < key) {
                p = p->next[i];
            }
        }
        p = p->next[0];
        if (p && p->key == key) {
            return p->value;
        }
        return std::nullopt;
    }

    // 删除key
    bool remove(K key) {
        std::vector<Node<K, V>*> update(maxLevel, nullptr);
        Node<K, V>* p = head;

        for (int i = currentLevel - 1; i >= 0; i--) {
            while (p->next[i] && p->next[i]->key < key) {
                p = p->next[i];
            }
            update[i] = p;
        }

        p = p->next[0];
        if (!p || p->key != key) return false;

        for (int i = 0; i < currentLevel; i++) {
            if (update[i]->next[i] != p) break;
            update[i]->next[i] = p->next[i];
        }
        delete p;

        // 清理空高层
        while (currentLevel > 1 && head->next[currentLevel -1] == nullptr) {
            currentLevel--;
        }
        return true;
    }

    // 打印底层链表，用来调试看结果
    void printList() {
        Node<K, V>* p = head->next[0];
        std::cout << "底层链表：";
        while(p) {
            std::cout << "[" << p->key << ":" << p->value << "] ";
            p = p->next[0];
        }
        std::cout << "\n";
    }
};

#endif
