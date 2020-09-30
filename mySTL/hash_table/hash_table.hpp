// Hash node class template
template <typename K, typename V>
class HashNode {
public:
    HashNode(const K &key, const V &value) :
    key(key), value(value), next(NULL) {
    }

    K getKey() const {
        return key;
    }

    V getValue() const {
        return value;
    }

    void setValue(V value) {
        HashNode::value = value;
    }

    HashNode *getNext() const {
        return next;
    }

    void setNext(HashNode *next) {
        HashNode::next = next;
    }

private:
    // key-value pair
    K key;
    V value;
    // next bucket with the same key
    HashNode *next;
};


// Default hash function class
template <typename K>
struct KeyHash {
    unsigned long operator()(const K& key) const
    {
        return reinterpret_cast<unsigned long>(key) % TABLE_SIZE;
    }
};


// Hash map class template
template <typename K, typename V, typename F = KeyHash<K>>
class HashMap {
public:
    HashMap() {
        // construct zero initialized hash table of size
        table = new HashNode<K, V> *[TABLE_SIZE]();
    }

    ~HashMap() {
        // destroy all buckets one by one
        for (int i = 0; i < TABLE_SIZE; ++i) {
            HashNode<K, V> *entry = table[i];
            while (entry != NULL) {
                HashNode<K, V> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = NULL;
        }
        // destroy the hash table
        delete [] table;
    }


    /*hash节点  hashNode的key存放是就是原数据中的key
    比如一组数据 james(12) tom(24) jane(5) john(7) smith(3) von(18)
    使用的hash函数为mod12
    得到如下数据 james(0) tom(0) jane(5) john(7) smith(3) von(6)
    这里的0,0,5,7,3,6指的是在buckets中的位置  buckets[i]
    james和tom都在buckets[0]中，之后使用链表，插入hashNode,james的hashNode是 hashNode(james,12), tom hashNode是hashNode(tom,24)
    之后便直接在此链表中根据hashNode->key查找到元素。
    该hashNode中可以存放许多东西，用于存放tom的信息。
    */
    bool get(const K &key, V &value) {
        unsigned long hashValue = hashFunc(key);
        //table[]存放了各个链表的头结点，entry是其中一个
        HashNode<K, V> *entry = table[hashValue];//HashNode<K, V> **table, HashNode->next

        //entry是某个链表的头结点，遍历此链表，查找链表中的元素，其Value等于key
        while (entry != NULL) {
            if (entry->getKey() == key) {// HashNode.key  HashNode.Value
                value = entry->getValue();
                return true;
            }
            entry = entry->getNext();
        }
        return false;
    }

    void put(const K &key, const V &value) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = NULL;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != NULL && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == NULL) {
            entry = new HashNode<K, V>(key, value);
            if (prev == NULL) {
                // insert as first bucket
                table[hashValue] = entry;
            } else {
                prev->setNext(entry);
            }
        } else {
            // just update the value
            entry->setValue(value);
        }
    }

    void remove(const K &key) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = NULL;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != NULL && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == NULL) {
            // key not found
            return;
        }
        else {
            if (prev == NULL) {
                // remove first bucket of the list
                table[hashValue] = entry->getNext();
            } else {
                prev->setNext(entry->getNext());
            }
            delete entry;
        }
    }

private:
    // hash table
    HashNode<K, V> **table;
    F hashFunc;
};