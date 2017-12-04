#include<iostream>
using namespace std;
#include<fstream>
#include<cstdlib>
#include<windows.h>
#include<cstring>
#include<cctype>


///////////////////////////////结点类 
class Node
{
private:
	int frequence; //频率 
	char value;    //值 
	Node* parent;  //指向父结点的指针 
	Node* left;    //指向左子节点的指针 
	Node* right;   //指向右子节点的指针 
public:
	Node();     
	Node(int fq);
	void setValue(char ch); //设置结点值 
	void setF(int fq);  //设置结点频率 
	void setCode(char pathcode[]);  //设置值所对应的编码 
	void incrOne();  //在处理值的频率时用于加1的函数 
	int getF();  //获取值所对应频率 
	char getValue(); //获取值 
	void setLeft(Node* left);  //设置左子节点 
	void setRight(Node* right);  //设置右子节点 
	void setParent(Node* parent);  //设置父节点 
	Node* getLeft(); //获取左子节点 
	Node* getRight(); //获取右子节点 
	Node* getParent(); //获取父节点 

};



Node::Node()
{
	frequence = 0;
	parent = left = right = NULL;
}


Node::Node(int fq)
{
	frequence = fq;
	parent = left = right = NULL;
}



void Node::setValue(char ch)
{
	value = ch;
}

void Node::setF(int fq)
{
	frequence = fq;
}




void Node::incrOne()
{
	frequence++;
}

int Node::getF()
{
	return frequence;
}

char Node::getValue()
{
	return value;
}



void Node::setLeft(Node* left)
{
	this->left = left;
}

void Node::setRight(Node* right)
{
	this->right = right;
}


void Node::setParent(Node* parent)
{
	this->parent = parent;
}


Node* Node::getLeft()
{
	return left;
}

Node* Node::getRight()
{
	return right;
}


Node* Node::getParent()
{
	return parent;
}


//////////////////////////////////Huffman树类 
template<class Node>
class Huffman
{
private:
	Node* root;//根节点 
	int sum;  //结点总数 
public:
	char code[26][100]; //26个字母分别对应的编码 
	Huffman();
	void createHuffman(Node* num, int n);//依据数组创建Huffman树 
	void selectNode(Node* node, int& r1, int& r2,int currsize);//对结点数组进行排序，按频率从低到高 
	void findLeaf_Code(Node* temp, char pathcode[], int k);  //对叶节点进行编码 
	bool isLeaf(Node* node);//判断是否为叶节点 
	Node* getRoot(); //返回根节点 
};


template<class Node>
Huffman<Node>::Huffman()
{
	root = NULL;
	sum = 0;
	for(int i=0;i<26;i++)
	{
		memset(code[i],0,sizeof(char)*100);
	}
	
}


template<class Node>
void Huffman<Node>::createHuffman(Node* num, int n)
{
	int k = 0;
	sum = 2 * n - 1;
	while (n + k < sum)
	{
		int r1=0, r2=0;
		selectNode(num, r1, r2,n+k);
		int temp = num[r1].getF() + num[r2].getF();
		num[n + k].setF(temp);
		num[n + k].setLeft(&num[r1]);
		num[n + k].setRight(&num[r2]);
		num[r1].setParent(&num[n + k]);
		num[r2].setParent(&num[n + k]);
		k++;
	}
	root = &num[sum-1];
}



template<class Node>
void Huffman<Node>::selectNode(Node* node, int& r1, int& r2,int currsize)
{
	int k=0;
	while(node[k].getParent()!=NULL)
	{
		k++;
	}
	r1 = r2 = k;
	for (int i = r1; i<currsize-1; i++)
	{
		if (node[i+1].getParent()==NULL&&node[r1].getF()>node[i + 1].getF())
		{
			r1 = i + 1;
		}
	}
	if(r1==r2)
	{
		while(node[++r2].getParent()!=NULL);
	}
	for (int i = r2; i<currsize-1; i++)
	{
		if ((i + 1) != r1&&node[i+1].getParent()==NULL&&node[r2].getF()>node[i + 1].getF())
		{
			r2 = i + 1;
		}
	}
}



template<class Node>
void Huffman<Node>::findLeaf_Code(Node* temp, char pathcode[], int k)////初始时 k=-1 
{
	if (k != -1)
	{
		if (isLeaf(temp))
		{
			if (temp->getParent()->getLeft() == temp)
			{
				pathcode[k] = '0';
			}
			else if (temp->getParent()->getRight() == temp)
			{
				pathcode[k] = '1';
			}
			
			for(int i=0;i<k+1;i++)
			{
				code[(int)temp->getValue() - 97][i]=pathcode[i];
			}//(int)temp->getValue()-97:for example, value is a,the result is 0
			
			return;
		}
		else if (temp->getParent()->getLeft() == temp)
		{
			pathcode[k] = '0';
		}
		else
		{
			pathcode[k] = '1';
		}
	}
	findLeaf_Code(temp->getLeft(), pathcode, k + 1);
	findLeaf_Code(temp->getRight(), pathcode, k + 1);
}





template<class Node>
bool Huffman<Node>::isLeaf(Node* node)
{
	if (node->getLeft() == NULL&&node->getRight() == NULL)
	{
		return true;
	}
	return false;
}


template<class Node>
Node* Huffman<Node>::getRoot()
{
	return root;
}







///////////////////////////////////////////////操作类 
template<class Node>
class Operation
{
private:
	Node character[26];//储存文件中的字母频率 
	Node temp[26];//储存去掉频率为0的字母后的字母频率及值 
	int size;//记录频率不为0的字母的种数 
	Huffman<Node> tree;
public:
	Operation();
	void countFrequence(char filename[]);//计算文件中各字符出现频率 
	void cutZero();//将结点数组中频率为0的元素减去，并将剩余元素放在temp数组中 
	void BuildHuffman(); //根据temp数组及size值创建Huffman树 
	void Compress(char sourcefilename[], char aimfilename[]);//编码 
	void Decompress(char sourcefilename[],char aimfilename[]);//解码 
};


template<class Node>
Operation<Node>::Operation()
{
	size = 0;
}


/////////////////////xd///////////////////计算文件中各字符出现频率 
template<class Node>
void Operation<Node>::countFrequence(char filename[])
{
	fstream file;
	file.open(filename, ios::in);
	char ch = '?';
	file >> ch;
	while (!file.eof())
	{
		switch (ch)
		{
		case 'a':
		{
					character[0].incrOne();
					break;
		}
		case 'b':
		{
					character[1].incrOne();
					break;
		}
		case 'c':
		{
					character[2].incrOne();
					break;
		}
		case 'd':
		{
					character[3].incrOne();
					break;
		}
		case 'e':
		{
					character[4].incrOne();
					break;
		}
		case 'f':
		{
					character[5].incrOne();
					break;
		}
		case 'g':
		{
					character[6].incrOne();
					break;
		}
		case 'h':
		{
					character[7].incrOne();
					break;
		}
		case 'i':
		{
					character[8].incrOne();
					break;
		}
		case 'j':
		{
					character[9].incrOne();
					break;
		}

		case 'k':
		{
					character[10].incrOne();
					break;
		}
		case  'l':
		{
					 character[11].incrOne();
					 break;
		}
		case 'm':
		{
					character[12].incrOne();
					break;
		}
		case 'n':
		{
					character[13].incrOne();
					break;
		}
		case 'o':
		{
					character[14].incrOne();
					break;
		}
		case 'p':
		{
					character[15].incrOne();
					break;
		}
		case 'q':
		{
					character[16].incrOne();
					break;
		}
		case 'r':
		{
					character[17].incrOne();
					break;
		}
		case 's':
		{
					character[18].incrOne();
					break;
		}
		case 't':
		{
					character[19].incrOne();
					break;
		}
		case 'u':
		{
					character[20].incrOne();
					break;
		}
		case 'v':
		{
					character[21].incrOne();
					break;
		}
		case 'w':
		{
					character[22].incrOne();
					break;
		}

		case 'x':
		{
					character[23].incrOne();
					break;
		}
		case 'y':
		{
					character[24].incrOne();
					break;
		}
		case 'z':
		{
					character[25].incrOne();
					break;
		}
		default:
		{
				   cout << endl << endl << "Sorry! some content in the file is not supported in this version" << endl;
				   cout << "The program will exit 3 seconds later" << endl;
				   Sleep(3000);
				   exit(-1);
		}
		}
		file >> ch;
	}
	file.close();
}




//////////////////////////////将结点数组中频率为0的元素减去，并将剩余元素放在temp数组中 
template<class Node>
void Operation<Node>::cutZero()
{
	for (int i = 0, k = 0; i<26; i++)
	{
		if (character[i].getF() != 0)
		{
			temp[k].setValue(97 + i);
			temp[k++].setF(character[i].getF());
			size++;
		}
	}

}

/////////////////////////////////////根据temp数组及size的值创建Huffman树 
template<class Node>
void Operation<Node>::BuildHuffman()
{
	tree.createHuffman(temp, size);

}



//////////////////////////////////编码 
template<class Node>
void Operation<Node>::Compress(char sourcefilename[], char aimfilename[])
{
	char pathcode[100];
	countFrequence(sourcefilename);
	cutZero();
	BuildHuffman();
	tree.findLeaf_Code(tree.getRoot(), pathcode, -1);
	fstream sourceFile;
	fstream compressFile;
	sourceFile.open(sourcefilename, ios::in);
	compressFile.open(aimfilename, ios::out);
	char ch;
	sourceFile >> ch;
	while (!sourceFile.eof())
	{
		compressFile << tree.code[(int)ch - 97];
		sourceFile >> ch;
	}
	sourceFile.close();
	compressFile.close();
}



////////////////////////////////解码
template<class Node>
void Operation<Node>::Decompress(char sourcefilename[],char aimfilename[])
{
	fstream sfile;
	fstream afile;
	Node* temp=tree.getRoot();
	sfile.open(sourcefilename,ios::in);
	afile.open(aimfilename,ios::out);
	char ch='0';
	sfile>>ch;
	while(!sfile.eof())
	{
		while(!tree.isLeaf(temp))
		{
			if(ch=='0')
			{
				temp=temp->getLeft();
			}
			else
			{
				temp=temp->getRight();
			}
			sfile>>ch;
		}
		char letter=temp->getValue();
		afile<<letter;
		temp=tree.getRoot();
	}
	sfile.close();
	afile.close();
}






int main(void)
{
	Operation<Node> operate;
	while (1)
	{
		cout << "*" << "1. Compress" << endl;
		cout << "*" << "2. Decompresss" << endl;
		cout << "*" << "3. Exit" << endl << endl;
		cout << "Please choose the selection you need:";
		int flag;
		cin >> flag;
		switch (flag)
		{
			case 1:
			{
				cout << endl << "Please input source file name<size less than 4GB>:";
				char sourcefilename[100];
				char aimfilename[100];
				cin >> sourcefilename;
				cout << endl << "Please aim source file name to store the compressed result:";
				cin >> aimfilename;
				operate.Compress(sourcefilename, aimfilename);
				break;
			}
			case 2:
			{
					cout<<endl<<"Please input source file name<size less than 4GB>:";
					char sourcefilename[100];
					char aimfilename[100];
					cin >> sourcefilename;
					cout << endl << "Please aim source file name to store the decompressed result:";
					cin >> aimfilename;
					operate.Decompress(sourcefilename,aimfilename);
					break;
			}
			case 3:
			{
					  exit(-1);
			}
			default:
			{
				cout << "Select number is not 1 or 2 or 3, please check it and select again" << endl;
				cout << "Please choose the selection you need:";
			}
		}
		cout<<endl<<endl<<"Continue? Y/N:";
		char yorn;
		int hot=1;
		while(hot)
		{
			cin>>yorn;
			yorn=toupper(yorn);
			switch(yorn)
			{
				case'Y':
					{
						system("cls");
						hot=0;
						break;
					}
				case'N':
					{
						exit(-1);
					}
				default:
					{
						cout<<"Sorry, your input character is not correct,please re-input:";
					}
			}
		}
	}

	return 0;
}










