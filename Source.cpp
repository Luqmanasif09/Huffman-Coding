#include<queue>
#include<string>
#include<fstream>
#include<vector>
#include <sstream> 
#include<algorithm>

#include<iostream>
using namespace std;

struct Huffman_Node
{
	char data;
	int Freq;
	Huffman_Node *Right;
	Huffman_Node *Left;
	Huffman_Node(char D ='\0', int F=0, Huffman_Node *R=nullptr, Huffman_Node *L=nullptr)
	{
		data = D;
		Freq = F;
		Right = R;
		Left = L;
	}
};
struct Frequncy
{
	int count;
	char ch;
	string Coded;
	Frequncy()
	{
		count = 0;
		ch = '\0';
		Coded = '\0';
	}
	int Get_Count()
	{
		return count;
	}
};

/*  COUNT FREQUENCY*/
bool Already_Exist(vector<Frequncy> Vs, char C)
{
	if (Vs.size() == 0)
	{
		return true;
	}
	for (int i = 0; i < Vs.size(); i++)
	{
		if (Vs[i].ch == C)
		{
			return false;
		}
	}
	return true;
}
void Calculate(char C, vector<Frequncy> &Vs,string S)
{
	if (!Already_Exist(Vs, C))
	{
		return;
	}
	Frequncy Obj;
	Obj.ch = C;
	for (int i = 0; i < S.size(); i++)
	{
		if (S[i] == C)
		{
			int C = Obj.Get_Count();
			C++;
			Obj.count = C;
		}
	}
	Vs.push_back(Obj);
}
void merge(vector<Frequncy> & v, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	vector<Frequncy> L;
	L.resize(n1);
	vector<Frequncy> R;
	R.resize(n2);

	for (i = 0; i < n1; i++)
	{
		L[i] = v[l + i];
	}
	for (j = 0; j < n2; j++)
	{
		R[j] = v[m + 1 + j];
	}

	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2)
	{
		if (L[i].count <= R[j].count)
		{
			v[k] = L[i];
			i++;
		}
		else
		{
			v[k] = R[j];
			j++;
		}
		k++;
	}
	while (i < n1)
	{
		v[k] = L[i];
		i++;
		k++;
	}
	while (j < n2)
	{
		v[k] = R[j];
		j++;
		k++;
	}
}
void mergeSort(vector<Frequncy> & v, int l, int r)
{
	if (l < r)
	{
		int m = l + (r - l) / 2;
		mergeSort(v, l, m);
		mergeSort(v, m + 1, r);
		merge(v, l, m, r);
	}
}
vector<Frequncy> Calculate_Frequency(string S)
{
	vector<Frequncy> Vs{ 0 };
	for (int i = 0; i < S.size(); i++)
	{
		Calculate(S[i], Vs, S);
	}

	mergeSort(Vs, 0, Vs.size() - 1);
	return Vs;
}

/*      Create Sorted Queue    */
void Copy_Into_Queue(queue<Huffman_Node*> &Q,const vector<Frequncy> &V)
{
	for (int i = 0; i < V.size(); i++)
	{
		Huffman_Node *H = new Huffman_Node(V[i].ch, V[i].count);
		Q.push(H);
	}
}

/*   Generate tree */
void Travsel(Huffman_Node *H, queue<Huffman_Node*> &Q)
{
	queue<Huffman_Node*> temp_Qeue;
	while (Q.size() != 0)
	{
		if (Q.front()->Freq > H->Freq)
		{
			break;
		}
		temp_Qeue.push(Q.front());
		Q.pop();
	}
	temp_Qeue.push(H);
	while (Q.size() != 0)
	{
		temp_Qeue.push(Q.front());
		Q.pop();
	}
	Q = temp_Qeue;
	while (temp_Qeue.size()!=0)
	{
		temp_Qeue.pop();
	}

}
void Create_Tree(queue<Huffman_Node*> &Q)
{
	while (Q.size() != 1)
	{
		Huffman_Node* First = Q.front();
		Q.pop();
		Huffman_Node* Second = Q.front();
		Q.pop();

		Huffman_Node* Temp = new Huffman_Node('$', First->Freq + Second->Freq);
		Temp->Left = First;
		Temp->Right = Second;
		Travsel(Temp, Q);
	}
}

/*   Codes*/
void Create_Code(Huffman_Node *H, char C, vector<Frequncy> &V, string S,int i)
{
	if (H == nullptr)
	{
		return;
	}
	if (H->data==C)
	{
		V[i].Coded = S;
	}
	Create_Code(H->Left,C,V, S + "0",i);
	Create_Code(H->Right, C,V, S + "1",i);
}
string Copy_Into_Code_File(Huffman_Node *H, char C, vector<Frequncy> V)
{
	string temp;
	for (int i = 0; i < V.size(); i++)
	{
		if (V[i].ch == C)
		{
			return V[i].Coded;
		}
	}
}
void Encode_Data_File(Huffman_Node *H,vector<Frequncy> & V,string S)
{
	ofstream W;
	W.open("Mycodes.txt");
	for (int i = 0; i < V.size(); i++)
	{
		W << V[i].ch << " " << V[i].Coded << endl;
	}
	W.close();
}
void Generate_Codes(Huffman_Node *H, vector<Frequncy> & V)
{
	string temp;
	for (int i = 0; i < V.size(); i++)
	{
		Create_Code(H, V[i].ch, V,temp,i);
		temp.clear();
	}
}


/*    Printing*/
void Print_Codes(const vector<Frequncy> & V)
{
	for (int i = 0; i < V.size(); i++)
	{
		cout << V[i].ch << ' ' << V[i].Coded << endl;
	}
}

/*    Decode  */
char Check_Code(string S, const vector<Frequncy> & V)
{
	for (int i = 0; i < V.size(); i++)
	{
		if (V[i].Coded == S)
		{
			return V[i].ch;
		}
	}
	return '$';
}
string Decode(Huffman_Node * H, const vector<Frequncy> & V, string Encode_String)
{
	char ch = 0;
	string S;
	string Decode_Msg;
	for (int i = 0; i < Encode_String.size(); i++)
	{
		S.push_back(Encode_String[i]);
		char C = Check_Code(S, V);
		if(C != '$')
		{
			Decode_Msg.push_back(C);
			S.clear();
		}
	}
	return Decode_Msg;
}


void Read_from_File(string & S)
{
	ifstream fin;
	fin.open("input.txt");
	if (!fin)
	{
		cout << "\nFile not found!!!!........\n";
		return;
	}
	char ch = '\0';
	while (!fin.eof())
	{
		getline(fin, S);
	}
	fin.close();
}
void Menu()
{
	cout << "\t\t********  Huffman Encoding and Decoding  ********";
	cout << "\n\nPress 1 : to read a Sectences from file ans store in new file";
	cout << "\nPress 2 : to display codes generated against each character";
	cout << "\nPress 3 : to read an encoded message(in form of 0s and 1s)	from file";
	cout << "\nPress 4 : to display the decoded message against the message";
	cout << "\nPress 5 : for searching whether a code is present in your generated codes or not ";
	cout << "\nPress 6 : for searching whether a character is present in your codes or not ";
	cout << "\nPress 7 : to EXIT";
	cout << "\n\nEnter your option  :   ";
}

void Operation()
{
	vector<Frequncy> V;
	queue<Huffman_Node*> Q;
	Huffman_Node *H;
	string Sentence;
	string Encoded_String;

	char ch = '\0';
	int opt = 0;
	int C1 = 0, C2 = 0;
	while (1)
	{
		Menu();
		cin >> opt;
		if (opt == 1)
		{
			Read_from_File(Sentence);
			V = Calculate_Frequency(Sentence);
			Copy_Into_Queue(Q, V);
			Create_Tree(Q);
			H = Q.front();
			Generate_Codes(H, V);
			Encode_Data_File(H, V, Sentence);
			C1++;
		}
		else if (opt == 2)
		{
			if (Sentence.size() == 0)
			{
				cout << "\n\nPress 1 to read and show codes a word :  ";
			}
			Print_Codes(V);
		}
		else if (opt == 3)
		{
			if (C1==0)
			{
				cout << "\n\nPress 1 to Get a code  :  \n";
				continue;
			}
			else
			{
				ifstream fin;
				fin.open("Endoded.txt");
				if (!fin)
				{
					cout << "\n\nFile not found !!!!.....\n";
					break;
				}
				else
				{
					getline(fin, Encoded_String);
					cout << "\n\nYour Encoded message is : " << Encoded_String;
				}
				C2++;
			}
		}
		else if (opt == 4)
		{
			if (C1==0&&C2==0)
			{
				cout << "\n\nPress 1 or 3 to read codes and Get codes 0s or 1s :  \n\n";
				continue;
			}
			H = Q.front();
			string S=Decode(H, V,Encoded_String);
			cout << "Your decode text is :  " << S;
		}
		else if (opt == 5)
		{
			if (Sentence.size() == 0)
			{
				cout << "\n\nPress 1 or 3 to read and perform  encode and decode a word :  \n\n";
				continue;
			}
			H = Q.front();
			string S;
			cout << "\n\nEnter a code to :   ";
			cin >> S;
			char ch = Check_Code(S, V);
			if (ch == '$')
			{
				cout << "Code  " << S << " Does not exist : ";
			}
			else
			{
				cout << "\n\nCode is present\n";
			}
		}
		else if (opt == 6)
		{
			if (Sentence.size() == 0)
			{
				cout << "\n\nPress 1 to read and perform  encode and decode a word :  \n\n";
				continue;
			}
			H = Q.front();
			char S;
			cout << "\n\nEnter a Character to :   ";
			cin >> S;
			
			if (!Already_Exist(V,S))
			{
				cout << "Character  " << S << " exist : ";
			}
			else
			{
				cout << "\n\nCharacter is not present\n";
			}
		}
		else
		{
			cout << "Wrong option:  ";
		}

		cout << "\n\nDo u want to perform again opeation : ";
		cout << "\nEnter y or yes or N for No  :     ";
		cin >> ch;
		if (ch == 'n' || ch == 'N')
		{
			system("CLS");
			break;
		}
		system("CLS");
	}

}

int main()
{
	Operation();
	return 0;
}