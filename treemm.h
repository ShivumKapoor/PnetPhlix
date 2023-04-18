#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <string>
#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap //clang++ main.cpp -o n
{
	private:
		//Node definition
		struct Node
		{
			Node(KeyType key, ValueType toAdd) 
			{
				m_key = key;
				m_values.push_back(toAdd);
				left = nullptr; 
				right = nullptr;
			};

			//primary value of each node
			KeyType m_key;

			//values mapped to key
			std::vector<ValueType> m_values;

			ValueType& getValue(int i)
			{
				ValueType& toReturn = m_values[i];
				return toReturn;
			}

			//traverse through nodes
			Node* left;
			Node* right;
		};

		//root Node
		Node* m_root; 
	
		//helper funciton delete all nodes from the tree
		
		void FreeTree(Node* node)
		{
			if (node == nullptr)
			{
				return;
			}

			FreeTree(node->left);
			FreeTree(node->right);

			delete node;
		}
	public:
		class Iterator
		{
			public:
				Iterator()
				{
					m_keyNode = nullptr;
				}

				Iterator(Node* pointAt)
				{
					if (pointAt != nullptr)
					{
						m_keyNode = pointAt;
						m_currIndex = 0;
					} else
					{
						m_keyNode = nullptr;
					}
				}

				ValueType& get_value() const
				{
					return m_keyNode->getValue(m_currIndex);
				}

				bool is_valid() const
				{
					return m_keyNode != nullptr;
				}

				void advance()
				{
					m_currIndex++;
					if (m_currIndex >= m_keyNode->m_values.size())
					{
						m_keyNode = nullptr;
					}
				}

			private:
				Node* m_keyNode;
				int m_currIndex;
		};

		TreeMultimap()
		{
			m_root = nullptr;
		}

		~TreeMultimap()
		{
			FreeTree(m_root);
		}

		void insert(const KeyType& key, const ValueType& value)
		{
			KeyType keyCopy = key;
			ValueType valueCopy = value;
			//first check if root node is nullptr
			if (m_root == nullptr)
			{
				m_root = new Node(keyCopy, valueCopy);
				return;
			}

			//keep going until you find where to add
			Node* curr_node = m_root;
			for(;;)
			{
				//found duplicate key
				if (key == curr_node->m_key)
				{
					curr_node->m_values.push_back(valueCopy);
					return;
				} else if (key < curr_node->m_key) //our new node is to the left of the current node
				{
					//if no new node exists there
					if (curr_node->left == nullptr)
					{
						curr_node->left = new Node(keyCopy, valueCopy);
						return;
					} else
					{
						curr_node = curr_node->left;
					}
				} else if (key > curr_node->m_key) //our new node is to the right of the current node
				{
					//if no new node exists there
					if (curr_node->right == nullptr)
					{
						curr_node->right = new Node(keyCopy, valueCopy);
						return;
					} else
					{
						curr_node = curr_node->right;
					}
				}
			}
		}

		Iterator find(const KeyType& key) const
		{
			Node* curr_node = m_root;

			while (curr_node != nullptr)
			{
				if (key == curr_node->m_key) //if you find the key
				{
					return Iterator(curr_node);
				} else if (key > curr_node->m_key) //if the key is larger than the current node
				{
					curr_node = curr_node->right;
				} else if (key < curr_node->m_key) //if the key is smaller than the current node
				{
					curr_node = curr_node->left;
				}
			}

			//if you could not find the value
			return Iterator();
		}
};

#endif // TREEMULTIMAP_INCLUDED
