#include "Map.h"

Map::Map() {
    m_head = m_tail = nullptr;
    m_size = 0;
}


Map::~Map() {
    if (m_head == nullptr) return;
    Node* curr = m_head;
    Node* p;
    while (curr != nullptr) {
        p = curr->next;
        delete curr;
        curr = p;
    }
}

Map::Map(const Map& other) {
    this->m_size = other.m_size;
    if (other.m_size == 0) {
        this->m_head = nullptr;
        this->m_tail = nullptr;
        return;
    }
    this->m_head = new Node;
    this->m_head->prev = nullptr;
    Node* p = this->m_head;
    for (Node* q = other.m_head; q->next != nullptr; q = q->next) {
        Node* newNode = new Node;
        p->key = q->key;
        p->value = q->value;
        p->next = newNode;
        newNode->prev = p;
        p = newNode;
    }
    p->key = other.m_tail->key;
    p->value = other.m_tail->value;
    p->next = nullptr;
    m_tail = p;
}

Map& Map::operator=(const Map& rhs) {
    if (this != &rhs) {
        Map temp(rhs);
        swap(temp);
    }
    return *this;
}

bool Map::empty() const { return (m_size == 0); }

int Map::size() const { return m_size; }

void Map::swap(Map& other) {
    // swap heads
    Node* tempHead = this->m_head;
    this->m_head = other.m_head;
    other.m_head = tempHead;
    // swap tails
    Node* tempTail = this->m_tail;
    this->m_tail = other.m_tail;
    other.m_tail = tempTail;
    // swap sizes
    int tempSize = this->m_size;
    this->m_size = other.m_size;
    other.m_size = tempSize;
}

bool Map::contains(const KeyType& key) const {
    for (Node* p = m_head; p != nullptr; p = p->next) {
        if (p->key == key)
            return true;
    }
    return false;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    if (contains(key)) return false;
    if (m_size == 0) {
        Node* newNode = new Node;
        newNode->key = key;
        newNode->value = value;
        newNode->prev = newNode->next = nullptr;
        m_head = m_tail = newNode;
        m_size++;
        return true;
    }
    Node* newNode = new Node;
    newNode->key = key;
    newNode->value = value;
    newNode->prev = m_tail;
    newNode->next = nullptr;
    m_tail->next = newNode;
    m_tail = newNode;
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
    for (Node* p = m_head; p != nullptr; p = p->next) {
        if (p->key == key) {
            p->value = value;
            return true;
        }
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    if (contains(key)) update(key, value);
    else insert(key, value);
    return true;
}

bool Map::erase(const KeyType& key) {
    if (!contains(key)) return false;
    Node* curr;
    for (curr = m_head; curr != nullptr; curr = curr->next) {
        if (curr->key == key) break;
    }
    if (curr->prev != nullptr)
        curr->prev->next = curr->next;
    else
        m_head = curr->next;
    
    if (curr->next != nullptr)
        curr->next->prev = curr->prev;
    else
        m_tail = curr->prev;
    
    delete curr;
    m_size--;
    return true;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    for (Node* p = m_head; p != nullptr; p = p->next) {
        if (p->key == key) {
            value = p->value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (i < 0 || i >= m_size) return false;
    Node* p = m_head;
    for (int j = 0; j < i; p = p->next, j++)
        ;
    key = p->key;
    value = p->value;
    return true;
}

bool combine(const Map& m1, const Map& m2, Map& result) {
    result = m1;
    bool flag = true;
    int n = m2.size();
    for (int i = 0; i < n; i++) {
        KeyType key;
        ValueType value;
        m2.get(i, key, value);
        if (result.contains(key)) {
            ValueType val;
            result.get(key, val);
            if (value != val) {
                flag = false;
                result.erase(key);
            }
        }
        else result.insert(key, value);
    }
    return flag;
}

void subtract(const Map& m1, const Map& m2, Map& result) {
    result = m1;
    int n = m1.size();
    for (int i = 0; i < n; i++) {
        KeyType key;
        ValueType value;
        m2.get(i, key, value);
        if (result.contains(key)) result.erase(key);
    }
}
