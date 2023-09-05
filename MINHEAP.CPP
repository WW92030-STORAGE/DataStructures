#include <iostream>
#include <cstdint>

using namespace std;

template <typename T>
class min_heap {
    T** arr;
    uint32_t capacity;
    uint32_t size;
    
    public:
    void swap(T** x, T** y) {
        T* temp = *x;
        *x = *y;
        *y = temp;
    }
    min_heap(uint32_t cap) {
        size = 0;
        capacity = cap;
        arr = new T*[cap];
    }
    
    void solve(uint32_t i) {
        uint32_t l = left(i);
        uint32_t r = right(i);
        uint32_t smallest = i;
        if (l < size && greater(arr[i], arr[l])) smallest = l;
        if (r < size && greater(arr[smallest], arr[r])) smallest = r;
        if (smallest != i) {
            swap(&arr[i], &arr[smallest]);
            solve(smallest);
        }
    }
    
    uint32_t parent(uint32_t i) { return (i - 1) / 2; }
    uint32_t left(uint32_t i) { return (i<<1) + 1; }
    uint32_t right (uint32_t i) { return (i<<1) + 2; }
    
    T* poll() {
        if (size <= 0) return nullptr;
        
        if (size == 1) {
            size--;
            return arr[0];
        }
        
        T* root = arr[0];
        arr[0] = arr[size - 1];
        size--;
        solve(0);
        return root;
    }
    
    T* peek() { return arr[0]; };
    
    bool greater(T* x, T* y) { // greater?
        return *x > *y;
    }
    
    bool insert(T* val) {
        if (size == capacity) return false;
        size++;
        uint32_t i = size - 1;
        arr[i] = val;
        
        while (i != 0 && greater(arr[parent(i)], arr[i])) {
            swap(&arr[i], &arr[parent(i)]);
            i = parent(i);
        }
        
        return true;
    }
};

int main() {
    min_heap<uint32_t> heap(1024);
    for (uint32_t i = 0; i < 621; i++) heap.insert(new uint32_t ((835 * i + 22) % 621) );
    
    while (true) {
        uint32_t* x = heap.poll();
        if (x == nullptr) break;
        cout << *x << endl;
    }
}
