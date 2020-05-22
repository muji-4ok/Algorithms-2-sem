#include <cassert>
#include <memory>
#include <utility>
#include <type_traits>

template<typename T>
class List {
 protected:
  template<bool>
  class m_Iterator;
  struct Node;

 public:
  using iterator = m_Iterator<false>;
  using const_iterator = m_Iterator<true>;
  using value_type = T;

  List() = default;
  explicit List(size_t count, const T &value = T());
  List(const List<T> &other);
  List(List<T> &&other) noexcept;
  List<T> &operator=(const List<T> &other);
  List<T> &operator=(List<T> &&other) noexcept;
  ~List();

  size_t size() const { return m_size; }
  const T &front() const { return head->data; }
  const T &back() const { return tail->data; }
  T &front() { return head->data; }
  T &back() { return tail->data; }
  bool empty() const { return !m_size; }
  void clear();
  void reverse();
  void unique();
  void push_back(T &&value);
  void push_back(const T &value);
  void push_front(T &&value);
  void push_front(const T &value);
  void pop_back();
  void pop_front();
  template<typename... Args>
  void emplace_back(Args &&... args);
  template<typename... Args>
  void emplace_front(Args &&... args);
  iterator begin() { return iterator(head, this); }
  const_iterator begin() const {
    return const_iterator(const_cast<Node *>(head),
                          const_cast<List *>(this));
  }
  const_iterator cbegin() const { return begin(); }
  iterator end() { return iterator(nullptr, this); }
  const_iterator end() const { return const_iterator(nullptr, const_cast<List *>(this)); }
  const_iterator cend() const { return end(); }
  void erase(iterator pos);
  template<bool IsConst>
  void erase(iterator first, m_Iterator<IsConst> last);
  template<bool IsConst, typename... Args>
  iterator emplace(m_Iterator<IsConst> pos, Args &&... args);
  template<bool IsConst, typename U>
  iterator insert(m_Iterator<IsConst> pos, U &&value);
  template<bool IsConst, typename InputIterator>
  iterator insert(m_Iterator<IsConst> pos, InputIterator first, InputIterator last);

 protected:
  struct Node {
    template<typename... Args>
    explicit Node(Args &&... args) : data(std::forward<Args...>(args...)) {}
    Node *left = nullptr;
    Node *right = nullptr;
    T data;
  };

  template<bool IsConst>
  class m_Iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = std::conditional_t<IsConst, const T &, T &>;
    using pointer = std::conditional_t<IsConst, const T *, T *>;

    friend List;

    explicit m_Iterator(Node *node, List *list) : node(node), list(list) {}
    reference operator*() { return node->data; }
    reference operator*() const { return node->data; }
    pointer operator->() { return &(node->data); }
    pointer operator->() const { return &(node->data); }
    m_Iterator<IsConst> &operator++() {
      node = node->right;
      return *this;
    }
    m_Iterator<IsConst> operator++(int) {
      m_Iterator<IsConst> copy(*this);
      ++(*this);
      return copy;
    }
    m_Iterator<IsConst> &operator--() {
      if (node)
        node = node->left;
      else
        node = list->tail;

      return *this;
    }
    m_Iterator<IsConst> operator--(int) {
      m_Iterator<IsConst> copy(*this);
      --(*this);
      return copy;
    }
    template<bool IsConst_>
    bool operator==(const m_Iterator<IsConst_> &other) const {
      return node == other.node && list == other.list;
    }
    template<bool IsConst_>
    bool operator!=(const m_Iterator<IsConst_> &other) const { return !(*this == other); }

   protected:
    Node *node;
    List *list;
  };

  size_t m_size = 0;
  Node *head = nullptr;
  Node *tail = nullptr;
};

template<typename T>
List<T>::List(size_t count, const T &value) {
  assert(count >= 0);

  if (!count)
    return;

  head = new Node(value);
  Node *cur = head;

  for (size_t i = 1; i < count; ++i) {
    cur->right = new Node(value);
    cur->right->left = cur;
    cur = cur->right;
  }

  tail = cur;
  m_size = count;
}
template<typename T>
List<T>::List(const List<T> &other) {
  if (!other.m_size)
    return;

  head = new Node(other.head->data);
  Node *cur = head;
  Node *nextCopyFrom = other.head->right;

  while (nextCopyFrom) {
    cur->right = new Node(nextCopyFrom->data);
    cur->right->left = cur;
    cur = cur->right;
    nextCopyFrom = nextCopyFrom->right;
  }

  tail = cur;
  m_size = other.m_size;
}
template<typename T>
List<T>::List(List<T> &&other) noexcept {
  std::swap(m_size, other.m_size);
  std::swap(head, other.head);
  std::swap(tail, other.tail);
}
template<typename T>
List<T> &List<T>::operator=(const List<T> &other) {
  if (&other == this)
    return *this;

  List<T> copy(other);
  *this = std::move(copy);

  return *this;
}
template<typename T>
List<T> &List<T>::operator=(List<T> &&other) noexcept {
  std::swap(m_size, other.m_size);
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  return *this;
}
template<typename T>
List<T>::~List() {
  clear();
}
template<typename T>
void List<T>::clear() {
  Node *cur = head;

  while (cur) {
    Node *nextCur = cur->right;
    delete cur;
    cur = nextCur;
  }

  head = nullptr;
  tail = nullptr;
  m_size = 0;
}
template<typename T>
void List<T>::reverse() {
  Node *cur = head;

  while (cur) {
    Node *nextCur = cur->right;
    std::swap(cur->left, cur->right);
    cur = nextCur;
  }

  std::swap(head, tail);
}
template<typename T>
void List<T>::pop_back() {
  Node *newTail = tail->left;

  if (newTail)
    newTail->right = nullptr;

  delete tail;
  tail = newTail;

  if (!tail)
    head = tail;

  --m_size;
}
template<typename T>
void List<T>::pop_front() {
  Node *newHead = head->right;

  if (newHead)
    newHead->left = nullptr;

  delete head;
  head = newHead;

  if (!head)
    tail = head;

  --m_size;
}
template<typename T>
template<typename... Args>
void List<T>::emplace_back(Args &&... args) {
  ++m_size;

  if (!tail) {
    tail = new Node(std::forward<Args...>(args...));
    head = tail;
    return;
  }

  tail->right = new Node(std::forward<Args...>(args...));
  tail->right->left = tail;
  tail = tail->right;
}
template<typename T>
template<typename... Args>
void List<T>::emplace_front(Args &&... args) {
  ++m_size;

  if (!head) {
    head = new Node(std::forward<Args...>(args...));
    tail = head;
    return;
  }

  head->left = new Node(std::forward<Args...>(args...));
  head->left->right = head;
  head = head->left;
}
template<typename T>
void List<T>::push_back(T &&value) {
  emplace_back(std::move(value));
}
template<typename T>
void List<T>::push_back(const T &value) {
  emplace_back(value);
}
template<typename T>
void List<T>::push_front(T &&value) {
  emplace_front(std::move(value));
}
template<typename T>
void List<T>::push_front(const T &value) {
  emplace_front(value);
}
template<typename T>
void List<T>::erase(List::iterator pos) {
  if (pos.node == head) {
    pop_front();
  } else if (pos.node == tail) {
    pop_back();
  } else {
    --m_size;
    Node *left = pos.node->left;
    Node *right = pos.node->right;
    delete pos.node;

    if (left)
      left->right = right;

    if (right)
      right->left = left;
  }
}
template<typename T>
template<bool IsConst>
void List<T>::erase(List::iterator first, List::m_Iterator<IsConst> last) {
  for (iterator it = first; it != last;) {
    iterator cur = it++;
    erase(cur);
  }
}
template<typename T>
void List<T>::unique() {
  for (iterator cur = begin(), next = ++begin(); next != end();) {
    if (*cur == *next) {
      erase(cur);
      cur = next++;
    } else {
      ++cur;
      ++next;
    }
  }
}
template<typename T>
template<bool IsConst, typename U>
typename List<T>::iterator List<T>::insert(List::m_Iterator<IsConst> pos, U &&value) {
  return emplace(pos, std::forward<U>(value));
}
template<typename T>
template<bool IsConst, typename InputIterator>
typename List<T>::iterator List<T>::insert(List::m_Iterator<IsConst> pos,
                                           InputIterator first,
                                           InputIterator last) {
  auto it = first;
  iterator result = pos;

  if (it != last) {
    result = insert(pos, *it);
    ++it;
  }

  for (; it != last; ++it)
    insert(pos, *it);

  return result;
}
template<typename T>
template<bool IsConst, typename... Args>
typename List<T>::iterator List<T>::emplace(m_Iterator<IsConst> pos, Args &&... args) {
  if (pos == begin()) {
    push_front(std::forward<Args...>(args...));
    return begin();
  } else if (pos == end()) {
    push_back(std::forward<Args...>(args...));
    return --end();
  } else {
    ++m_size;
    Node *left = pos.node->left;
    Node *newNode = new Node(std::forward<Args...>(args...));
    newNode->left = left;
    newNode->right = pos.node;
    left->right = newNode;
    pos.node->left = newNode;
    return iterator(newNode, this);
  }
}
