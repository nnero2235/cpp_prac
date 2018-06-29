#ifndef TREE_NNERO_HPP
#define TREE_NNERO_HPP
#include<memory>
#include<iostream>


namespace nnero{
    namespace tree{

        template<typename T>
        class Node{
        public:
            Node(const T& t):
                m_data{t}
            {}
            T m_data;
            Node* m_parent{nullptr};
            Node* m_left{nullptr};
            Node* m_right{nullptr};
        };

        /*
          binary search tree
          offer O(logn) time to search
         */
        template<typename T>
        class BinaryTree{
        public:
            BinaryTree() = default;
            ~BinaryTree(){
                if(m_root){
                    freeNode(m_root);
                    m_size = 0;
                }
            }
            void add(const T& t){
                if(m_size == 0){
                    m_root = new Node<T>(t);
                    ++m_size;
                    return;
                }
                Node<T>* current_node = m_root;
                while(true){
                    if(t > current_node->m_data){
                        if(!current_node->m_right){
                            current_node->m_right = new Node<T>(t);
                            current_node->m_right->m_parent = current_node;
                            ++m_size;
                            return;
                        }
                        current_node = current_node->m_right;
                    } else if(t < current_node->m_data){
                        if(!current_node->m_left){
                            current_node->m_left = new Node<T>(t);
                            current_node->m_left->m_parent = current_node;
                            ++m_size;
                            return;
                        }
                        current_node = current_node->m_left;
                    } else{
                        std::cout<<"not allow repeat values"<<std::endl;
                        return;
                    }
                }
            }

            bool del(const T& t){
                if(m_size == 0){
                    return false;
                }
                Node<T>* current_node = m_root;
                while(true){
                    if(current_node->m_data == t){
                        /*leaf point so delete directly*/
                        if(current_node->m_left == nullptr && current_node->m_right == nullptr){
                            Node<T>* parent = current_node->m_parent;
                            if(parent){
                                if(parent->m_left == current_node){
                                    parent->m_left = nullptr;
                                } else if(parent->m_right == current_node){
                                    parent->m_right = nullptr;
                                }
                            } else {
                                m_root = nullptr;
                            }
                            delete current_node;
                        } else if(current_node->m_left == nullptr){
                            Node<T>* parent = current_node->m_parent;
                            if(parent){
                                if(parent->m_left == current_node){
                                    parent->m_left = current_node->m_right;
                                } else if(parent->m_right == current_node){
                                    parent->m_right = current_node->m_right;
                                }
                            } else {
                                m_root = m_root->m_right;
                                m_root->m_parent = nullptr;
                            }
                            delete current_node;
                        } else if(current_node->m_right == nullptr){
                            Node<T>* parent = current_node->m_parent;
                            if(parent){
                                if(parent->m_left == current_node){
                                    parent->m_left = current_node->m_left;
                                } else if(parent->m_right == current_node){
                                    parent->m_right = current_node->m_left;
                                }
                            } else {
                                m_root = m_root->m_left;
                                m_root->m_parent = nullptr;
                            }
                            delete current_node;
                        } else {
                            Node<T>* target = current_node->m_right;
                            while(target->m_left){
                                target = target->m_left;
                            }
                            if(target->m_right){
                                Node<T>* target_parent = target->m_parent;
                                if(target_parent->m_left == target){
                                    target_parent->m_left = target->m_right;
                                } else {
                                    target_parent->m_right = target->m_right;
                                }
                                target->m_right->m_parent = target_parent;
                            } else {
                                Node<T>* target_parent = target->m_parent;
                                if(target_parent->m_left == target){
                                    target_parent->m_left = nullptr;
                                } else {
                                    target_parent->m_right = nullptr;
                                }
                            }
                            target->m_left = current_node->m_left;
                            current_node->m_left->m_parent = target;
                            target->m_right = current_node->m_right;
                            current_node->m_right->m_parent = target;
                            Node<T>* parent = current_node->m_parent;
                            if(parent){
                                if(parent->m_left == current_node){
                                    parent->m_left = target;
                                } else {
                                    parent->m_right = target;
                                }
                                target->m_parent = parent;
                            } else {
                                target->m_parent = nullptr;
                                m_root = target;
                            }
                            delete current_node;
                        }
                        --m_size;
                        return true;
                    } else if(t > current_node->m_data){
                        if(!current_node->m_right){
                            return false;
                        }
                        current_node = current_node->m_right;
                    } else if(t < current_node->m_data){
                        if(!current_node->m_left){
                            return false;
                        }
                        current_node = current_node->m_left;
                    }
                }
            }

            bool search(const T& t){
                if(m_size == 0){
                    return false;
                }
                Node<T>* current_node = m_root;
                size_t counter(0);
                while(true){
                    ++counter;
                    if(current_node->m_data == t){
                        std::cout<<"search count:"<<counter<<std::endl;
                        return true;
                    } else if(t > current_node->m_data){
                        if(!current_node->m_right){
                            std::cout<<"search count:"<<counter<<std::endl;
                            return false;
                        }
                        current_node = current_node->m_right;
                    } else if(t < current_node->m_data){
                        if(!current_node->m_left){
                            std::cout<<"search count:"<<counter<<std::endl;
                            return false;
                        }
                        current_node = current_node->m_left;
                    }
                }
            }

            void travPrint(){
                if(m_root){
                    print(m_root);
                }
            }
            
            size_t size()const{
                return m_size;
            }

        private:
            void print(Node<T>* node){
                if(node){
                    print(node->m_left);
                    std::cout<<node->m_data<<std::endl;
                    print(node->m_right);
                }
            }
            
            void freeNode(Node<T>* node){
                if(node){
                    freeNode(node->m_left);
                    freeNode(node->m_right);
                    delete node;
                }
            }
            Node<T>* m_root;
            size_t m_size{0};
        };
    }
}

#endif
