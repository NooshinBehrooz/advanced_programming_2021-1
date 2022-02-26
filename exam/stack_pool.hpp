/**
* @file: stack_pool.hpp
* @project_title:  A pool of blazingly fast stacks
* @defination: Stack is a linear data structure which follows a particular order for perfoming of operations. The order may be LIFO (Last In First Out) or FILO (First In Last Out).
*/ 
#include <iostream>
#include <vector>

/** 
At first, the iterator class has been defiened 
*/
template <typename stack_pool, typename N, typename T>
  class Iterator { 
    stack_pool* ptr_pool;
    N current_val;
    public:
    using value_type = T ;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag; //forward_iterator is used

   /** the iterator_constructor */
    Iterator(stack_pool* ptr_pool , N current_val) : ptr_pool{ptr_pool}, 
     current_val{current_val} {} 

   /** dereference operator */
    reference operator*(){ return ptr_pool->value(current_val); }

   /** Pre-increment */
    Iterator& operator++(){
      current_val = ptr_pool->next(current_val) ;
      return *this;  
    }

    /** Post increment */
    Iterator operator++(int){
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    /** equality */
    friend bool operator==( const Iterator& x, const Iterator& y) {return 
     x.current_val==y.current_val;}  

    /** non_equality */
    friend bool operator!=( const Iterator& x, const Iterator& y) {return !(x==y);}
  }; 

/**
Secondly, the class of for stack_pool has been structed
*/
template <typename T, typename N = std::size_t>
class stack_pool{ 
  struct node_t{
    T value;
    N next;
  };

  /** 
  Thirdly, object vector pool has been defined
  */
  std::vector<node_t> pool;
  using stack_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type;  
  stack_type free_nodes{0};    // empty at the beginning
 
  node_t& node(stack_type x) noexcept { return pool[x-1]; }              
  const node_t& node(stack_type x) const noexcept { return pool[x-1]; }

/** 
Fourthly, the necessary functions and constructors have been introduced
*/
public: 

  /** default constructor  */
  stack_pool() noexcept = default;
  /** custom constructor by using std::vector::reserve(n)  */
  explicit stack_pool(size_type n) : pool{}  {pool.reserve(n); } 

  using iterator = Iterator<stack_pool, N, const T>;
  using const_iterator = Iterator<stack_pool, N, const T>;

  iterator begin(stack_type x) noexcept {return iterator{this, x};}
  iterator end(stack_type ) noexcept {return iterator{this, stack_type(0)};}

  const_iterator begin(stack_type x) const noexcept {return const_iterator{this, x};}
  const_iterator end(stack_type ) const noexcept {return const_iterator{this, stack_type(0)};}

  const_iterator cbegin(stack_type x) const noexcept {return const_iterator{this, x};}
  const_iterator cend(stack_type ) const noexcept {return const_iterator{this,stack_type(0) };}

 /** new_stack function: for creating a new stack */
  stack_type new_stack() noexcept {return stack_type(0);}    

 /** reserve function: for reserving n nodes with std::vector<node_t>::reserve */
  void reserve(size_type n) {pool.reserve(n); }  
 
 /** capacity function: to obtain the capacity of std::vector */
  size_type capacity() const noexcept {return pool.capacity(); } 

 /** empty function: to check wether the stack is empty or not */
  bool empty(stack_type x) const noexcept {
    if (x == end())
       return true;
    else
       return false;
  }

 /** end function: for returning the end of the stack */
  stack_type end() const noexcept { return stack_type(0); }       

 /** value function: for returning the value of the node */
  T& value(stack_type x) noexcept {return node(x).value; }    
  const T& value(stack_type x) const noexcept {return node(x).value; }     

  /** next function: for obtaining the index of the next node */
  stack_type& next(stack_type x) noexcept {return node(x).next; }   
  const stack_type& next(stack_type x) const noexcept {return node(x).next; }     

  /** push function: for inserting a new value in a specific stack where it is becoming the head of the stack */
  // L-value reference
  stack_type push(const T& val, stack_type head){
    if( empty(free_nodes) ){  // free_nodes are not available, stack_overflow
      pool.push_back(node_t{val, head});    
      return static_cast<N>(pool.size()) ;   
    }
    // there are free_nodes:
    else {
    auto tmp = free_nodes;
    value(free_nodes) = val;
    free_nodes = next(free_nodes);
    next(tmp) = head;
    return tmp;
    }  
  }

  /* push function: for inserting a new value in a mentioned stack where it is becoming the head of the stack */
  // R-value reference
  stack_type push(T&& val, stack_type head){
    if( empty(free_nodes) ){  // free_nodes are not available, stack_overflow
      pool.push_back(node_t{ std::move(val), head});    
      return static_cast<N>(pool.size()) ;   
    }
    // there are free_nodes:
    else {
    auto tmp = free_nodes;
    value(free_nodes) = std::move(val);
    free_nodes = next(free_nodes);
    next(tmp) = head;
    return tmp;
    }
  }

  /** pop function: to remove the newest/last element in a mentioned stack and
      changing the head by the previous element*/
  stack_type pop(stack_type x){
  if(empty(x)){
    std::cout << "Stack is underflow.";
  }
    auto tmp = next(x);     
    next(x) = free_nodes;    
    free_nodes = x;            //move the freenodes head to x
    return tmp;
  }

  /** free stack function: to empty the stack by iterating pop at each node */  
  stack_type free_stack(stack_type x){
    while( x != end() ){
      x = pop(x);
    }
    return x;
  }

  /** stack_print function: for printing the full_stack */
  void stack_print(stack_type x) {
    std::cout<<"the stacks nodes are:"<<std::endl;
    auto start = begin(x), stop = end(x);    
    for ( ; start != stop; ++start){
      std::cout <<"["<< *start << "]";
    }
    std::cout <<"\n******************\n"<< std::endl;
  }
    
  /** pool_print function: for printing the full stack_pool */
  void pool_print()noexcept {
    std::cout<< "\n The nodes of stack_pool are:" << std::endl;
    for (auto k = 0; k < pool.size(); k++) {
      std::cout << "[" << pool[k].value << "]";
    }
    std::cout << std::endl;
  } 
};   

/**
  int main() {
    
  // push (add) values to the l1 
  stack_pool<int> pool{ 16 };
  auto l1 = pool.new_stack();
  l1 = pool.push(1, l1);
  l1 = pool.push(3, l1);
  l1 = pool.push(5, l1);
  l1 = pool.push(7, l1);

  // push (add) values to the l2 
  auto l2 = pool.new_stack();
  l2 = pool.push(2, l2);
  l2 = pool.push(4, l2);
  l2 = pool.push(6, l2);
  l2 = pool.push(8, l2);
  l2 = pool.push(10, l2);
  l2 = pool.push(12, l2);
  l2 = pool.push(14, l2);
  l2 = pool.push(16, l2);
     
  // pop (remove) values from l1 and l2 
  l1 = pool.pop(l1);
  l2 = pool.pop(l2); 

  // push values to the l1 agaqin 
  l1 = pool.push(9, l1);
  l1 = pool.push(11, l1);
    
  // printing the full stacks and pool 
  pool.stack_print(l1);
  pool.stack_print(l2);
  pool.pool_print();

 return 0;
}
*/
