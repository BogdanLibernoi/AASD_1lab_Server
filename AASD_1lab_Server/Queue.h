#pragma once
#include <iostream>

template<typename T>
class Queue
{
private:
	int size;
	T* queue;

	int front, rear;




public:
	struct Iterator
	{
		int index;
		Queue<T>* que;

		Iterator(int index, Queue<T>* queue)
			: index(index), que(queue)
		{}

		bool operator==(const Iterator& other) const { return index == other.index; }
		bool operator!=(const Iterator& other) const { return index != other.index; }

		Iterator& operator++()
		{
			index = (index + 1) % que->size;
			return *this;
		}
		Iterator& operator++(int a)
		{
			index = (index + 1) % que->size;
			return *this;
		}
		Iterator& operator--()
		{
			index = (index - 1 + que->size) % que->size;
			return *this;
		}
		Iterator& operator--(int a)
		{
			index = (index - 1 + que->size) % que->size;
			return *this;
		}

		T& operator*() { return que->queue[index]; }
		T* operator->() { return &(que->queue[index]); }
	};

	Iterator begin() { return Iterator(front, this); }
	Iterator end() { return Iterator(rear, this); }



	Queue()
		:front(-1), rear(-1), size(10)
	{
		queue = new T[size];
	}
	Queue(int _size)
		:front(-1), rear(-1), size(_size)
	{
		queue = new T[size];
	}


	bool IsFull()const {
		if (front == 0 && rear == (size - 1))
			return true;
		if (front == rear + 1)
			return true;

		return false;
	}
	bool IsEmpty()const {
		if (front == -1)
			return true;

		return false;
	}


	void Enqueue(T value) {

		if (IsFull()) {
			std::cout << "queue is full" << std::endl;
		}
		else
		{
			if (front == -1) front = 0;

			++rear %= size;
			queue[rear] = value;
		}
	}
	T Dequeue()
	{
		T temp;
		if (IsEmpty()) {
			std::cout << "queue is empty" << std::endl;
		}
		else
		{
			temp = queue[front];

			if (front == rear)
			{
				front = -1;
				rear = -1;
			}

			else
				++front %= size;

			std::cout << " is poped..." << std::endl;
			return temp;

		}

	}


	~Queue()
	{
		delete[] queue;
	}

};







