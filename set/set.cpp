#include <cstddef>
#include <initializer_list>
#include <exception>
#include <algorithm>
#include <cassert>

template<class T>
class Set {
private:
    using ll = long long;

    ll max(ll a, ll b) const {
        return a > b ? a : b;
    }

    bool eq(const T& a, const T& b) const {
        return !(a < b) && !(b < a);
    }

    struct node {
        T value;
        node *l, *r, *p;
        node *leftmost, *rightmost;
        ll height;

        node(T value) : value(value), l(nullptr), r(nullptr), p(nullptr), leftmost(this), rightmost(this), height(1) {};
    };

    node * root_ = nullptr;
    size_t size_ = 0;

    ll height(node * root) const {
        return root ? root->height : 0;
    }

    ll bal(node * root) const {
        return root ? height(root->l) - height(root->r) : 0;
    }

    node* leftRotation(node * root) {
        node * b = root->r;
        node * c = root->r->l;

        root->r = c;
        b->l = root;
        upd(root);
        upd(b);
        return b;
    }

    node* rightRotation(node * root) {
        node * b = root->l;
        node * c = root->l->r;

        b->r = root;
        root->l = c;
        upd(root);
        upd(b);
        return b;
    }

    node* bigLeftRotation(node * root) {
        root->r = rightRotation(root->r);
        return leftRotation(root);
    }

    node* bigRightRotation(node * root) {
        root->l = leftRotation(root->l);
        return rightRotation(root);
    }

    void upd(node * root) {
        if (root) {
            root->height = max(height(root->l), height(root->r)) + 1;
            root->leftmost = root->rightmost = root;
            root->p = nullptr;
            if (root->l) {
                root->l->p = root;
                root->leftmost = root->l->leftmost;
            }
            if (root->r) {
                root->r->p = root;
                root->rightmost = root->r->rightmost;
            }
        }
    }

    node* rotate(node * root) {
        upd(root);
        if (bal(root) == -2) {
            if (bal(root->r) < 1) {
                root = leftRotation(root);
            }
            else {
                root = bigLeftRotation(root);
            }
        }
        else if (bal(root) == 2) {
            if (bal(root->l) > -1) {
                root = rightRotation(root);
            }
            else {
                root = bigRightRotation(root);
            }
        }
        upd(root);
        return root;
    }

    node* insert_(node * root, const T& elem) {
        if (!root) {
            ++size_;
            return new node(elem);
        }
        if (eq(elem, root->value)) {
            return root;
        }
        if (elem < root->value) {
            root->l = insert_(root->l, elem);
        }
        else {
            root->r = insert_(root->r, elem);
        }
        return rotate(root);
    }

    node* relink_min_(node * root) {
        if (!root->l) return root->r;
        root->l = relink_min_(root->l);
        return rotate(root);
    }

    node* relink_max_(node * root) {
        if (!root->r) return root->l;
        root->r = relink_max_(root->r);
        return rotate(root);
    }

    node* erase_(node * root, const T& elem) {
        if (!root) return nullptr;
        if (eq(root->value, elem)) {
            node *f = root->l, *s = root->r;
            if (!f && !s) {
                // I'm a leaf, yay.
                --size_;
                delete root;
                return nullptr;
            }
            else if (!f && s) {
                --size_;
                delete root;
                return s;
            }
            else if (f && !s) {
                --size_;
                delete root;
                return f;
            }
            else {
                ll fh = height(f), sh = height(s);
                if (fh >= sh) {
                    // go on with f
                    // now I need to relink f
                    // f has l and r. r is guaranteed to be nullptr;
                    // So I need to pass l through myself.
                    // p->l == f ? p->l = f->l : p->r = f->l;
                    // Actually no
                    // Just call erase_(root->l, f->value);
                    // actually no, relink.
                    while (f->r) f = f->r;
                    root->l = relink_max_(root->l);
                    f->l = root->l;
                    f->r = root->r;
                    delete root;
                    --size_;
                    return rotate(f);
                }
                else {
                    // go on with s
                    while (s->l) s = s->l;
                    root->r = relink_min_(root->r);
                    s->l = root->l;
                    s->r = root->r;
                    delete root;
                    --size_;
                    return rotate(s);
                }
            }
        }
        else if (elem < root->value) {
            root->l = erase_(root->l, elem);
        }
        else {
            root->r = erase_(root->r, elem);
        }
        return rotate(root);
    }

    node* find_(node * root, const T& elem) const {
        if (!root) {
            return nullptr;
        }
        if (eq(root->value, elem)) return root;
        if (elem < root->value) return find_(root->l, elem);
        else return find_(root->r, elem);
    }

    node* lower_bound_(node * root, const T& elem) const {
        if (!root) return nullptr;
        if (eq(root->value, elem)) {
            return root;
        }
        else if (elem < root->value) {
            node* res = lower_bound_(root->l, elem);
            if (!res || (root->value < res->value)) {
                res = root;
            }
            return res;
        }
        else {
            return lower_bound_(root->r, elem);
        }
    }

    void clear_(node * root) {
        if (!root) return;
        clear_(root->l);
        clear_(root->r);
        delete root;
    }

    node* deep_copy_(node * root) {
        if (!root) {
            return nullptr;
        }
        node * nroot = new node(root->value);
        nroot->l = deep_copy_(root->l);
        nroot->r = deep_copy_(root->r);
        upd(nroot);
        nroot->p = nullptr;
        return nroot;
    }

    /*	void pprint(node * root, int n = 0) const {
    if (!root) return;
    pprint(root->l, n + 1);
    for (int i = 0; i < n; ++i) cout << '\t';
    cout << "(" << root->value << ", " << root->height << ")\n";
    pprint(root->r, n + 1);
    }*///

public:
    class SetIterator {
    private:
        const Set<T>* me;
        const node* item;
        bool end;

    public:
        const T& operator*() const {
            return item->value;
        }

        SetIterator(const Set<T>* me = nullptr, const node* ptr = nullptr, bool end = false) : me(me), item(ptr), end(end) {};

        const T* operator->() {
            return &(item->value);
        }

        SetIterator& operator=(const SetIterator& other) {
            if (this == &other) return *this;
            me = other.me;
            item = other.item;
            end = other.end;
            return *this;
        }

        SetIterator& operator++() {
            if (end) {
                //throw new exception("No elements after Set::end exist");
                return *this;
            }
            if (me->root_->rightmost == item) {
                this->item = nullptr;
                this->end = true;
                return *this;
            }
            if (item->r) {
                item = item->r;
                while (item->l) item = item->l;
                return *this;
            }
            // else there's no right child. Thus I need to go higher.
            // The moment I increase, by moving to the right, I stop;
            while (item->p->r == item) item = item->p;
            item = item->p;
            return *this;
        }

        SetIterator operator++(int) {
            if (end) {
                //throw new exception("No elements after Set::end exist");
                return *this;
            }
            auto res = SetIterator(me, item, end);
            if (me->root_->rightmost == item) {
                this->item = nullptr;
                this->end = true;
                return res;
            }
            if (item->r) {
                item = item->r;
                while (item->l) item = item->l;
                return res;
            }
            // else there's no right child. Thus I need to go higher.
            // The moment I increase, by moving to the right, I stop;
            while (item->p->r == item) item = item->p;
            item = item->p;
            return res;
        }

        SetIterator& operator--() {
            if (me->root_->leftmost == item) {
                //throw new exception("No elements before Set::begin exist");
                return *this;
            }
            if (end) {
                this->item = me->root_->rightmost;
                this->end = false;
                return *this;
            }
            if (item->l) {
                item = item->l;
                while (item->r) item = item->r;
                return *this;
            }
            // else there's no right child. Thus I need to go higher.
            // The moment I increase, by moving to the right, I stop;
            while (item->p->l == item) item = item->p;
            item = item->p;
            return *this;
        }

        SetIterator operator--(int) {
            if (me->root_->leftmost == item) {
                //throw new exception("No elements before Set::begin exist");
                return *this;
            }
            auto res = SetIterator(me, item, end);
            if (end) {
                this->item = me->root_->rightmost;
                this->end = false;
                return res;
            }
            if (item->l) {
                item = item->l;
                while (item->r) item = item->r;
                return res;
            }
            // else there's no right child. Thus I need to go higher.
            // The moment I increase, by moving to the right, I stop;
            while (item->p->l == item) item = item->p;
            item = item->p;
            return res;
        }

        bool operator!=(const SetIterator& other) const {
            if (me->root_ != other.me->root_) return true;
            if (end && other.end) return false;
            if (end != other.end) return true;
            return item != other.item;
        };

        bool operator==(const SetIterator& other) const {
            return !(*this != other);
        };
    };

    using iterator = SetIterator;

    Set() : root_(nullptr), size_(0) {};

    Set(Set<T> &other) {
        if (other.root_ == root_) {
            return;
        }
        clear_(root_);
        size_ = other.size_;
        root_ = deep_copy_(other.root_);
    }

    Set<T>& operator=(const Set<T>& other) {
        if (this == &other) {
            return *this;
        }
        clear_(root_);
        size_ = other.size_;
        root_ = deep_copy_(other.root_);
        return *this;
    }

    template<typename Iterator>
    Set(Iterator first, Iterator last) {
        root_ = nullptr;
        size_ = 0;
        for (; first != last; ++first) {
            root_ = insert_(root_, *first);
        }
    }

    Set(std::initializer_list<T> elems) {
        size_ = 0;
        root_ = nullptr;
        for (const auto &x : elems) {
            root_ = insert_(root_, x);
        }
    }

    SetIterator begin() const {
        return SetIterator(this, root_ ? root_->leftmost : nullptr, root_ == nullptr);
    }

    SetIterator end() const {
        return SetIterator(this, nullptr, true);
    }

    void insert(const T& elem) {
        root_ = insert_(root_, elem);
        upd(root_);
    }

    void erase(const T& elem) {
        root_ = erase_(root_, elem);
        upd(root_);
    }

    SetIterator find(const T& elem) const {
        auto res = find_(root_, elem);
        return SetIterator(this, res, res == nullptr);
    }

    SetIterator lower_bound(const T& elem) const {
        auto res = lower_bound_(root_, elem);
        return SetIterator(this, res, res == nullptr);
    }

    void clear() {
        clear_(root_);
        root_ = nullptr;
        size_ = 0;
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    //	void print() const {
    //		pprint(root_);
    //	}

    bool contains(const T & elem) const {
        return find(elem) != end();
    }

    ~Set() {
        clear();
    }
};