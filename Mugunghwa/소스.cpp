#include <iostream>
#include <cmath>

using namespace std;

int arr[100001];

int main()
{
	int num;
	arr[0] = 0;
	arr[1] = 1;
	for (int i = 2;i <= 100000;i++)
	{
		i = double(i);
		arr[i] = arr[i - int(pow(double((int)sqrt(i)),2))] + 1;
	}
	cin >> num;
	cout << arr[num];
}