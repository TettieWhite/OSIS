#define _CRT_SECURE_NO_WARNINGS
#define UNICODE

#include <stdio.h>
#include <stdlib.h>


struct Node
{
	Node* next;
	Node* prev;
	char* str;
};

Node* head = nullptr;

Node* get_first() {
	return head;
}

Node* get_last() {
	Node* temp = get_first();

	while (temp != nullptr && temp->next != nullptr) {
		temp = temp->next;
	}

	return temp;
}

void push_back(char* str) {
	Node* temp = get_last();
	Node* new_item = new Node();
	new_item->next = nullptr;
	new_item->prev = temp;
	if (temp != nullptr) {
		temp->next = new_item;
	}
	else {
		head = new_item;
	}
	new_item->str = str;
}

void push_front(char* str) {
	Node* temp = get_first();
	Node* new_item = new Node();
	new_item->next = temp;
	new_item->prev = nullptr;
	if (temp != nullptr) {
		temp->prev = new_item;
	}
	head = new_item;
	new_item->str = str;
}

bool is_empty() {
	return head == nullptr;
}

Node* get_item(int index) {
	if (is_empty()) {
		//throw "There is no such item.";
		return nullptr;
	}

	int i = 0;
	Node* cur = head;
	for (i = 0; i < index && cur->next != nullptr; ++i) {
		cur = cur->next;
	}

	if (i != index) {
		//throw "There is no such item.";
		return nullptr;
	}

	return cur;
}

void remove(int index) {
	Node* cur = get_item(index);
	if (cur == nullptr) {
		printf("No such item.\n");
		return;
	}

	Node* next = cur->next;
	Node* prev = cur->prev;

	if (prev != nullptr) {
		prev->next = next;
	}

	if (next != nullptr) {
		next->prev = prev;
	}

	if (cur == head) {
		head = cur->next;
	}

	delete cur;
}

void clear() {
	while (head != nullptr) {
		remove(0);
	}
}

void output() {
	Node* cur = head;
	while (cur != nullptr) {
		printf("%s ", cur->str);
		cur = cur->next;
	}
	printf("\n");
}

int main3() {
	char cmd[16];
	int index;

	printf("Enter one of the folowwing commands: \npush_back 'str' - pb, \npush_front 'str' - pf, \nremove 'index' - r, \nclear - c, \noutput - o, \nexit - e,\ncls - cs\n\n");

	while (true) {
		char* str = new char[256];
		scanf("%s", cmd);

		switch (cmd[0])
		{
		case 'p': // push
		{
			if (cmd[1] == 'b') // push_back
			{
				scanf("%s", str);
				push_back(str);
				break;
			}
			else // push_front
			{
				scanf("%s", str);
				push_front(str);
				break;
			}
		}
		case 'r': // remove
		{
			scanf("%d", &index);
			remove(index);
			break;
		}
		case 'c': // clear or cls
		{
			if (cmd[1] == 's') // cls
			{
				system("cls");
				printf("Enter one of the folowwing commands: \npush_back 'str' - pb, \npush_front 'str' - pf, \nremove 'index' - r, \nclear - c, \noutput - o, \nexit - e,\ncls - cs\n\n");
				break;
			}
			else // clear
			{
				clear();
			}
			break;
		}
		case 'e': // exit
		{
			return 0;
		}
		case 'o':
		{
			output();
			break;
		}
		}
	}
	return 0;
}

int main()
{
	main3();
}