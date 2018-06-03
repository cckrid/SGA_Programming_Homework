#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class CircularQueue
{
private:
	T * data; // ������ �迭
	int capacity; // ť �뷮
	int front; // ���� �ε���
	int rear; // �Ĵ� �ε���
public:
	CircularQueue(int capacity) {
		capacity = capacity + 1; // ������ �迭�� ũ��� ���� �뷮���� 1�� ���� ũ�� (���� ���� ����)
		data = new T[capacity]; // T �迭 capacity + 1���� �޸� ������ �Ҵ�
		front = rear = 0; // ���� �Ĵ� �ʱ�ȭ
	}
	
	~CircularQueue() {
		delete[] data; // ������ �迭 �Ҹ�
	}
	
	void Clear() {
		front = rear = 0;
	}
	
	void Enqueue(const T& data) {
		int index; // �����Ͱ� �� �ε���
		index = rear;
		rear = (rear + 1) % (capacity);
		this->data[index] = data; // index ��°�� ����� �����Ϳ� data�� ����
	}

	T Dequeue() {
		int index = front; // index�� ������ �ε��� ����
		front = (front + 1) % capacity;
		return data[index]; // ���ܵǴ� �����͸� ��ȯ�Ѵ�.
	}

	int GetSize() {
		// ������ �ε����� �Ĵ��� �ε����� ���ų� �׺��� �۴ٸ�
		// �Ĵ��� �ε������� ������ �ε����� ������ ��ȯ
		if (front <= rear) {
			return rear - front;
		}
		// ������ �ε����� �Ĵ��� �ε������� ũ�ٸ�
		else
			// �뷮���� ������ �ε����� �� �ڿ� �Ĵ��� �ε����� ���� ���� ��ȯ
			return capacity - front + rear; 
	}

	bool IsEmpty() {
		return front == rear; 
	}

	bool IsFull() {
		return front == (rear + 1) % capacity;
	}

	int GetRear() { return rear; }
	int GetFront() { return front; }

	vector<T> GetTotalData() {
		int tempFront = front;
		vector<T> result;
		while (tempFront != rear) {
			result.push_back(data[tempFront]);
			tempFront = (tempFront + 1) % capacity;
		}
		return result;
	}

	void Show() {
		int tempFront = front;
		while (tempFront != rear) {
			cout << data[tempFront] << " ";
			tempFront = (tempFront + 1) % capacity;
		}
	}
};
