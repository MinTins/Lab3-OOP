

 
#pragma once

#include <ostream>
#include <vector>
#include <memory>
#include <string>
template<typename T>
class PriorityQueue
{
public:
	PriorityQueue()
	{

	}

	
	bool isEmpty() const
	{
		return mElements.empty();
	}

	
	std::unique_ptr<T> pop()
	{
		swap(0, mElements.size() - 1);
		auto top = std::move(mElements.back());
		mElements.pop_back();
		siftDown(0);
		return top;
	}

	void push(std::unique_ptr<T> elem)
	{
		elem->index = mElements.size();
		mElements.emplace_back(std::move(elem));
		siftUp(mElements.size() - 1);
	}

	void update(std::size_t i)
	{
		int parent = getParent(i);
		if (parent >= 0 && *mElements[parent] < *mElements[i])
			siftUp(i);
		else
			siftDown(i);
	}

	void remove(std::size_t i)
	{
		swap(i, mElements.size() - 1);
		mElements.pop_back();
		if (i < mElements.size())
			update(i);
	}

	
	std::ostream& print(std::ostream& os, int i = 0, std::string tabs = "") const
	{
		if (i < mElements.size())
		{
			os << tabs << *mElements[i] << std::endl;
			display(getLeftChild(i), tabs + '\t');
			display(getRightChild(i), tabs + '\t');
		}
		return os;
	}

private:
	std::vector<std::unique_ptr<T>> mElements;

	
	int getParent(int i) const
	{
		return (i + 1) / 2 - 1;
	}

	std::size_t getLeftChild(std::size_t i) const
	{
		return 2 * (i + 1) - 1;
	}

	std::size_t getRightChild(std::size_t i) const
	{
		return 2 * (i + 1);
	}

	
	void siftDown(std::size_t i)
	{
		std::size_t left = getLeftChild(i);
		std::size_t right = getRightChild(i);
		std::size_t j = i;
		if (left < mElements.size() && *mElements[j] < *mElements[left])
			j = left;
		if (right < mElements.size() && *mElements[j] < *mElements[right])
			j = right;
		if (j != i)
		{
			swap(i, j);
			siftDown(j);
		}
	}

	void siftUp(std::size_t i)
	{
		int parent = getParent(i);
				if (parent >= 0 && *mElements[parent] < *mElements[i])
		{
			swap(i, parent);
			siftUp(parent);
		}
		return;
		int lc = getLeftChild(parent);
		int rc = getRightChild(parent);
		if (rc < mElements.size() && *mElements[lc] < *mElements[rc])
		{
			swap(lc, rc);
		}
	}

	inline void swap(std::size_t i, std::size_t j)
	{
		auto tmp = std::move(mElements[i]);
		mElements[i] = std::move(mElements[j]);
		mElements[j] = std::move(tmp);
		mElements[i]->index = i;
		mElements[j]->index = j;
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& queue)
{
	return queue.print(os);
}
