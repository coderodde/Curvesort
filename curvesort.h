#ifndef CURVESORT_H
#define CURVESORT_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <type_traits>

namespace net {
    
    namespace coderodde {
        
        namespace sorting {
            
            template<typename Iter>
            void sort_impl(Iter begin, Iter end, std::true_type)
            {
                using value_type =
                typename std::iterator_traits<Iter>::value_type;
                
                class Curvesort {
                    
                    struct Node {
                        value_type key;
                        size_t count;
                        struct Node* prev;
                        struct Node* next;
                        
                        Node(value_type key) : key{key}, count{1} {}
                    };
                    
                    value_type previous_key;
                    
                    Node* head;
                    Node* tail;
                    Node* prev_updated; // Points to the most recently added or
                    // updated node.
                    
                public:
                    
                    // We choose to add the first value in the constructor so
                    // that in count we do not have to make an additional check
                    // whether the node list is empty or not. This adds to
                    // efficiency.
                    Curvesort(value_type first_value)
                    :
                    previous_key{first_value}
                    {
                        Node* node = new Node(first_value);
                        node->next = nullptr;
                        node->prev = nullptr;
                        
                        head = node;
                        tail = node;
                        prev_updated = node;
                    }
                    
                    ~Curvesort()
                    {
                        Node* next;
                        
                        while (head)
                        {
                            next = head->next;
                            delete head;
                            head = next;
                        }
                    }
                    
                    void update_smaller_node(value_type key)
                    {
                        Node* tmp = prev_updated->prev;
                        
                        while (tmp && tmp->key > key)
                        {
                            tmp = tmp->prev;
                        }
                        
                        if (tmp == nullptr)
                        {
                            // 'key' is smaller than any currently stored key.
                            // Therefore, we update the list head node:
                            Node* newnode = new Node(key);
                            newnode->next = head;
                            newnode->prev = nullptr;
                            head->prev = newnode;
                            head = newnode;
                            prev_updated = newnode;
                        }
                        else if (tmp->key == key)
                        {
                            // Node exists, just increment the counter:
                            tmp->count++;
                            prev_updated = tmp;
                        }
                        else
                        {
                            // Insert a new node between 'tmp' and 'tmp.next':
                            Node* newnode = new Node(key);
                            newnode->prev = tmp;
                            newnode->next = tmp->next;
                            newnode->prev->next = newnode;
                            newnode->next->prev = newnode;
                            prev_updated = newnode;
                        }
                    }
                    
                    void update_greater_node(value_type key)
                    {
                        Node* tmp = prev_updated->next;
                        
                        while (tmp && tmp->key < key)
                        {
                            tmp = tmp->next;
                        }
                        
                        if (tmp == nullptr)
                        {
                            // 'key' is larger than any currently stored key.
                            // Therefore, we update the list tail node:
                            Node* newnode = new Node(key);
                            newnode->prev = tail;
                            newnode->next = nullptr;
                            tail->next = newnode;
                            tail = newnode;
                            prev_updated = newnode;
                        }
                        else if (tmp->key == key)
                        {
                            // Node exists, just increment the counter:
                            tmp->count++;
                            prev_updated = tmp;
                        }
                        else
                        {
                            // Insert a new node between 'tmp.prev' and 'tmp':
                            Node* newnode = new Node(key);
                            newnode->prev = tmp->prev;
                            newnode->next = tmp;
                            tmp->prev->next = newnode;
                            tmp->prev = newnode;
                            prev_updated = newnode;
                        }
                    }
                    
                    void count(Iter begin, Iter end)
                    {
                        begin++; // Omit the first value since we added to the
                        // node list.
                        while (begin != end)
                        {
                            value_type current_key = *begin;
                            
                            if (current_key < previous_key)
                            {
                                update_smaller_node(current_key);
                            }
                            else if (current_key > previous_key)
                            {
                                update_greater_node(current_key);
                            }
                            else
                            {
                                prev_updated->count++;
                            }
                            
                            previous_key = current_key;
                            begin++;
                        }
                    }
                    
                    void build(Iter begin)
                    {
                        Iter iter = begin;
                        
                        for (Node* node = head; node; node = node->next)
                        {
                            size_t count = node->count;
                            value_type key = node->key;
                            
                            for (size_t i = 0; i != count; ++i)
                            {
                                *iter++ = key;
                            }
                        }
                    }
                };
                
                Curvesort curvesort(*begin);
                curvesort.count(begin, end);
                curvesort.build(begin);
            }
            
            template<typename Iter>
            void sort_impl(Iter begin, Iter end, std::false_type)
            {
                // Not a sequence of primitive integral types. Fall back to
                // std::sort.
                std::sort(begin, end);
            }
            
            template<typename Iter>
            void sort(Iter begin, Iter end)
            {
                using value_type =
                typename std::iterator_traits<Iter>::value_type;
                
                sort_impl(begin, end, std::is_integral<value_type>());
            }
        } // End of 'net::coderodde::sorting'
    } // End of 'net::coderodde'
} // End of 'net'

#endif // CURVESORT_H
