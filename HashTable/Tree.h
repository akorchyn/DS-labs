#pragma once

#include "ICollection.h"
#include <queue>
#include <cmath>

template< class T >
class Tree : public ICollection<T>
{
	class Node
	{
	public:
		Node(const T& data) : left(nullptr), right(nullptr), data(data) {}

		void add(const T& item)
		{
			if (item < data) // move left
			{
				if (left)
				{
					return left->add(item);
				}
				left = new Node(item);
			}
			else // move right (data is more or equal)
			{
				if (right)
				{
					return right->add(item);
				}
				right = new Node(item);
			}
		}

		bool contains(const T& item) const
		{
			if (is_equal(item, data)) // data is equal
			{
				return true;
			}
			if (item < data)
			{
				return left ? left->contains(item) : false;
			}
			return right ? right->contains(item) : false;
		}

		bool remove(const T& item)
		{
			if (is_equal(item, data))
			{
				remove_root_node();
				return true;
			}
			if (item < data)
			{
				if (left)
				{
					if (is_equal(left->data, item) && left->is_simple_node())
					{
						left = remove_simple_node(left);
						return true;
					}
					return left->remove(item);
				}
			}
			else
			{
				if (right)
				{
					if (is_equal(right->data, item) && right->is_simple_node())
					{
						right = remove_simple_node(right);
						return true;;
					}
					return right->remove(item);
				}
			}
			return false;
		}

		template< class Callback >
		void traverse(Callback&& callback)
		{
			if ( left )
			{
				left->traverse(std::forward<Callback>(callback));;
			}
			callback(data);
			if (right)
			{
				right->traverse(std::forward<Callback>(callback));
			}
		}

		uint32_t active_childs() const
		{
			return (left != nullptr) + (right != nullptr);
		}

		bool is_simple_node() const
		{
			return (left == nullptr) || (right == nullptr);
		}

		Node* remove_simple_node(Node* node_to_delete)
		{
			auto tmp = node_to_delete->right ? node_to_delete->right : node_to_delete->left;
			delete node_to_delete;
			return tmp;
		}

		void remove_root_node()
		{
			// We should replace this node to the smalest in right three or the biggest in the left three.
			auto& node = right ? right : left;
			data = right ? right->min_value() : left->max_value();
			if (node->is_simple_node())
			{
				node = remove_simple_node(node);
				return;
			}

			node->remove(data);
;		}

		T min_value()
		{
			struct Node* current = this;
			while (current && current->left)
				current = current->left;
			return current->data;
		}

		T max_value()
		{
			struct Node* current = this;
			while (current && current->right)
				current = current->right;
			return current->data;
		}

		bool is_equal(const T& a, const T& b) const
		{
			return (a < b) == (b < a);
		}

		void tree_to_list()
		{
			auto tail = this;
			auto rest = right;
			while (rest != nullptr)
			{
				if (rest->left == nullptr)
				{
					tail = rest;
					rest = rest->right;
				}
				else // rotating
				{
					auto temp = rest->left;
					rest->left = temp->right;
					temp->right = rest;
					rest = temp;
					tail->right = temp;
				}
			}
		}

		size_t size() const
		{
			std::queue<const Node*> q;
			int count = 1;
			q.push(this);

			while (!q.empty())
			{
				const Node* temp = q.front();
				if (temp->left)
				{
					q.push(temp->left);
					count++;
				}

				if (temp->right)
				{
					q.push(temp->right);
					count++;
				}
				q.pop();
			}

			return count;
		}

		void compress(Node* root, size_t count)
		{
			auto scanner = root;
			for (size_t i = 0; i < count; ++i)
			{
				auto child = scanner->right;
				scanner->right = child->right;
				scanner = scanner->right;
				child->right = scanner->left;
				scanner->left = child;
			}
		}
		
		void remove_childs()
		{
			if (left)
			{
				left->remove_childs();
			}
			if (right)
			{
				right->remove_childs();
			}
			delete left;
			delete right;
		}

		void list_to_tree(size_t size)
		{
			if (left)
			{
				return;
			}
			auto leaves = static_cast<size_t>(std::round( size + 1 - std::pow(2, std::log2(size + 1))));
			compress(this, leaves);
			size = size - leaves;
			for (; size > 1; size = size / 2)
			{
				compress(this, size / 2);
			}
		}

		Node* left;
		Node* right;
		T data;
	};

public:
	Tree() : root(nullptr) {}
	Tree(const Tree&) = delete;
	~Tree()
	{
		if (root)
		{
			root->remove_childs();
		}
		delete root;
	}

	void add(const T& item) override
	{
		if (!root)
		{
			root = new Node(item);
			return;
		}
		try_rebalance();
		return root->add(item);
	}

	bool contains(const T& item) const override
	{
		if (!root)
		{
			return false;
		}
		return root->contains(item);
	}
	bool remove(const T& item) override
	{
		if (!root)
		{
			return false;
		}
		if (root->active_childs() == 0)
		{
			if (root->is_equal(root->data, item))
			{
				delete root;
				root = nullptr;
				return true;
			}
			return false;
		}

		try_rebalance();
		return root->remove(item);
	}
	template< class Callback >
	void traverse(Callback&& callback)
	{
		if (!root)
		{
			return;
		}
		return root->traverse(std::forward<Callback>(callback));
	}

	size_t size() const override
	{
		return root ? root->size() : 0;
	}

	// Day–Stout–Warren algorithm  https://en.wikipedia.org/wiki/Day%E2%80%93Stout%E2%80%93Warren_algorithm
	void balance()
	{
		if (!root)
		{
			return;
		}

		auto dummy_root = new Node({});
		dummy_root->right = root;
		dummy_root->tree_to_list();
		dummy_root->list_to_tree(dummy_root->size() - 1);
		root = dummy_root->right;
		delete dummy_root;
	}

private:
	void try_rebalance()
	{
		ops++;
		if (ops % 1000 == 0)
		{
			ops = 0;
			balance();
		}
	}


	Node* root;
	size_t ops = 0;
};
