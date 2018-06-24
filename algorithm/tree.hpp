#ifndef TREE_NNERO_HPP
#define TREE_NNERO_HPP
#include<memory>
#include<iostream>


namespace nnero{
    namespace tree{

        template<typename T>
        class Node{
        public:
            Node(std::shared_ptr<T> data_ptr):
                m_data_ptr{std::move(data_ptr)}
            {
            }
            std::shared_ptr<T> m_data_ptr;
            std::shared_ptr<Node<T>> m_parent;
            std::shared_ptr<Node<T>> m_left;
            std::shared_ptr<Node<T>> m_right;
        };

        /*
          binary search tree
          offer O(logn) time to search
         */
        template<typename T>
        class BinaryTree{
            friend class Node<T>;
        public:
            BinaryTree() = default;
            ~BinaryTree() = default;
            void add(std::shared_ptr<T> t){
                if(m_size == 0){
                    m_root = std::make_shared<Node<T>>(new Node<T>(t));
                    ++m_size;
                    return;
                }
                size_t counter(0);
                std::shared_ptr<Node<T>> current_node = m_root;
                while(true){
                    ++counter;
                    if(*t > *(current_node->m_data_ptr)){
                        if(!current_node->m_right){
                            current_node->m_right == std::make_shared<Node<T>>(new Node<T>(t));
                            ++m_size;
                            break;
                        }
                        current_node = current_node->m_right;
                    } else if(*(current_node->m_data_ptr) < *t){
                        if(!current_node->m_left){
                            current_node->m_left == std::make_shared<Node<T>>(new Node<T>(t));
                            ++m_size;
                            break;
                        }
                        current_node = current_node->m_left;
                    } else {
                        std::cout<<"error same value so omitted!"<<std::endl;
                        break;
                    }
                }
                std::cout<<"add count:"<<counter<<std::endl;
            }

            std::shared_ptr<T>& search(const T& t){
                if(m_size == 0 || m_size == 1){
                    return m_root->m_data_ptr;
                }
                size_t counter(0);
                std::shared_ptr<Node<T>> current_node = m_root;
                while(true){
                    ++counter;
                    if(*(current_node->m_data_ptr) == t){
                        break;
                    } else if(*(current_node->m_data_ptr) > t){
                        if(!current_node->m_right){
                            break;
                        }
                        current_node = current_node->m_right;
                    } else if(*(current_node->m_data_ptr) < t){
                        if(!current_node->m_left){
                            break;
                        }
                        current_node = current_node->m_left;
                    } else {
                        std::cout<<"error can't reach"<<std::endl;
                    }
                }
                std::cout<<"search count:"<<counter<<std::endl;
                return current_node->m_data_ptr;
            }
            
            size_t size()const{
                return m_size;
            }
        private:
            std::shared_ptr<Node<T>> m_root;
            size_t m_size{0};
        };
    }
}

#endif
