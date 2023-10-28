struct node {
  int key;
  int val;
  node* next = nullptr;
  node* prev = nullptr;
  node(int k, int v) : key(k), val(v) {}
  node(int k, int v, node* n, node* p) : key(k), val(v), next(n), prev(p) {}
};

class LRUCache {
 private:
  unordered_map<int, node*> data;
  node* head;
  node* tail;

  int cnt;
  int cap;

  void addNode(node* n) {
    node* next = head->next;
    head->next = n;
    n->next = next;
    next->prev = n;
    n->prev = head;
  }

  void removeNode(node* n) {
    node* prev = n->prev;
    node* next = n->next;
    prev->next = next;
    next->prev = prev;
  }

  void moveToHead(node* n) {
    removeNode(n);
    addNode(n);
  }

  node* popTail() {
    node* prev = tail->prev;
    removeNode(prev);
    return prev;
  }

 public:
  LRUCache(int c) : cap(c), cnt(0) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    head = new node(-1, 0);
    tail = new node(-1, 0);
    head->next = tail;
    tail->prev = head;
  }

  ~LRUCache() {
    node* c = head;
    node* next;
    while (c != nullptr) {
      next = c->next;
      delete c;
      c = next;
    }
    head = nullptr;
    tail = nullptr;
  }

  int get(int key) {
    auto it = data.find(key);
    if (it == data.end()) return -1;

    moveToHead(it->second);
    return it->second->val;
  }

  void put(int key, int value) {
    auto it = data.find(key);
    if (it == data.end()) {
      // there is not item
      node* n = new node(key, value);
      data[key] = n;
      addNode(n);
      cnt++;

      if (cnt > cap) {
        node* t = popTail();
        data.erase(t->key);
        delete t;
        cnt--;
      }
    } else {
      // has item
      data[key]->val = value;
      moveToHead(it->second);
    }
  }
};