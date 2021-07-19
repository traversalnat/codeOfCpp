#include<iostream>
#include<algorithm>
#include<string>
using namespace std;

int main() 
{
	string s {"This is an example"};
	cout << s << endl;
	s.erase(find(s.begin(), s.end(), ' '));
	cout << s << '\n';
}
