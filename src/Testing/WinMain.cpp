#include "pch.h"
#include <iostream>
#include "List.h"
using namespace std;



int main()
{
	GCList<int> intList = GCList<int>();
	
	intList.PushFront(5);
	cout << intList.GetFirstNode()->GetData() << " ; " << intList.GetLastNode()->GetData() << endl;
	
	intList.PushFront(6);
	cout << intList.GetFirstNode()->GetData() << " ; " << intList.GetLastNode()->GetData() << endl;
	
	intList.PushBack(4);
	cout << intList.GetFirstNode()->GetData() << " ; " << intList.GetLastNode()->GetData() << endl;
	
	intList.GetFirstNode()->PushBefore(3);
	cout << intList.GetFirstNode()->GetData() << " ; " << intList.GetLastNode()->GetData() << endl;

	intList.GetLastNode()->PushAfter(7);
	cout << intList.GetFirstNode()->GetData() << " ; " << intList.GetLastNode()->GetData() << endl;
	
	return 0;
}