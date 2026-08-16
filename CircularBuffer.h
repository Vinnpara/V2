#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#define NOMINMAX

#include <fstream>
#include <windows.h>
#include <iostream>

#include <memory>
#include <stdexcept>
#include <cstddef>

#include <mutex>
#include <string>

template <typename T>

class CircularBuffer {
public:
	explicit CircularBuffer(size_t size) :buffer(std::make_unique<T[]>(size)), max_size(size) {}

	T ReadByte() //Read from the buffer, retrieve and remove the oldest
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (IsEmpty()) { throw std::runtime_error("Buffer is empty"); }

		T item = buffer[head];
		full = 0;
		head = (tail + 1) % max_size;

		return item;
	};

	void WriteByte(T item) // Add an item to the buffer
	{
		std::lock_guard<std::mutex> lock(mutex_);
		buffer[tail] = item;
		std::cout << "\nThe tail end of buffer " << buffer[tail];

		if (full)
		{//If the buffer is full, it gets overwritten while discarding the oldest item
			head = (head + 1) % max_size;
		}

		tail = (tail + 1) % max_size;
		full = (tail == head);
	};

	bool IsFull() const { return full; }
	bool IsEmpty() const { return (!full && (head == tail)); }

	const T& peek() const {
		std::lock_guard<std::mutex> lock(mutex_);
		if (empty_internal()) {
			throw std::runtime_error("Buffer is empty; cannot peek");
		}
		return buffer_[head_];
	}

	size_t size() const
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (full) return  max_size;

		if (tail >= head) return tail - head;

		return max_size + tail - head;
	};

	void reset()
	{
		head = 0;
		tail = 0;
		full = false;
	};

	size_t capacity() { return max_size; }


private:

	std::unique_ptr<T[]> buffer; // Internal data storage
	size_t head = 0;             // Points to oldest element (for reading)
	size_t tail = 0;             // Points to next write slot
	size_t max_size;             // Total capacity
	bool full = false;           //  flag to separate full vs empty
	mutable std::mutex mutex_;
};

#endif  
