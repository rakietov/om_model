//Algorytm Knutha-Morrisa-Pratta - wyszukiwanie wzorca
//Bartosz Bednarczyk
//www.algorytm.org
 
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
 
using namespace std;
 
void Pref( vector<int> &P, string &S ){
	unsigned int t = 0, i, n = S.size();
	P.resize(n+1, 0);
 
	for( i = 2; i < n; i++ ){
		while (t > 0 && S[t + 1] != S[i]) t = P[t];
		if( S[t+1] == S[i] ) t++;
		P[i] = t;
	}
}
 
bool KMP( string &T, string &W ){
	string S = "#" + W + "#" + T;
	vector<int> P;
	Pref(P, S);
 
	unsigned int i, ws = W.size();
 
	for( i = ws + 2; i < S.size(); i++ ){
		//wypisz pozycje wzorca w tekscie
		if( P[i] == ws )return true; //printf("%d\n", i-ws-ws);
	}
	return false;
}
 



int KMPi( string &T, string &W ){
	string S = "#" + W + "#" + T;
	vector<int> P;
	Pref(P, S);
 
	unsigned int i, ws = W.size();
 
	for( i = ws + 2; i < S.size(); i++ ){
		//wypisz pozycje wzorca w tekscie
		if( P[i] == ws )return i-ws-ws;//printf("%d\n", i-ws-ws);
	}
}
 










