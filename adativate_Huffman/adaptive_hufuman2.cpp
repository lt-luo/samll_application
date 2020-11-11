#include <iostream>
#include <algorithm>
#include <string>
#include <queue>
#include <sstream>
#include <fstream>
#include <cstring>
#include<windows.h>
using namespace std;

class Node{
	public:
		int weight;
		//�ַ������ͱ�ʾ�����������ʱ������Ҷ�ڵ� 
		int data;				
		Node *lchild;
		Node *rchild;
		Node *parent;
		string code;
		Node(){
			weight = 0;
			lchild = NULL;
			rchild = NULL;
			parent = NULL;
			code = "";
		}
};

class HufumanTree{
	private:
		Node *root;
		Node *NTY;
		char data[130];			//ASCLL��128���ַ� 
		int  numOfchar;
	public:
		HufumanTree();
		Node *getRoot();
		void isIndata(char ch, Node *T, Node **p);
		void insert(char ch);
		void getBalance(Node *current);
		void print(char ch, ofstream&);
		void swap(Node *p1, Node *p2);
		void get_huffuman_code(Node *T);
		Node* find_same_weight(Node *p);
		void Decode(string str);
};

//���캯�����Գ�Ա��ʼ�� 
HufumanTree::HufumanTree()
{
	root = new Node();
	root->data = 130;
	NTY = root;	
	numOfchar  = 0; 
}

Node* HufumanTree::getRoot()
{
	return root;
}

//DFS���������������ַ��Ƿ�������          
void HufumanTree::isIndata(char ch, Node *T, Node **p)  
{
	if(T)
	{
		if(T->data == (int)ch)
		{
			*p = T; 
			return;
		}
		
		isIndata(ch, T->lchild, p);
		isIndata(ch, T->rchild, p);
	}

}

void HufumanTree::getBalance(Node *current)
{
	Node *same_weight_node = new Node();
	Node *p = current;
	//����������һ���Ȩ
	while(p!=root)			
	{
		same_weight_node = find_same_weight(p);
		if(same_weight_node!=p && p->parent!=same_weight_node){                //������������Ҳ������NULL   p->parent!=same_weight��Ϊ�˷�ֹparent�Ǹ��ڵ����� 
			swap(same_weight_node, p);
		}
		p = p->parent;								
	}
	//�����Ȩ 
	p = current;
	while(p)
	{
		p->weight++;
		p = p->parent;
	}
}

//����������ַ����뵽�շ������� 
void HufumanTree::insert(char ch)
{
	Node *cur = NULL;
	isIndata(ch, root, &cur);   //�ж��ַ��Ƿ���Huffmna����
	if(cur)						//pָ���ַ������е�Ҷ�ڵ�λ�� 
	{
		//�����շ����� 
		getBalance(cur);
	}
	else
	{
		Node *s = NTY;
		Node *NewNode = new Node();
		Node *NTY_lchild = new Node();
		
		NewNode->data = (int)ch;
		NewNode->weight = 1;
		NTY_lchild->data = 130;
		NTY_lchild->weight = 0;
		
		
		s->rchild = NewNode;
		s->lchild = NTY_lchild;
		NewNode->parent = s;
		NTY_lchild->parent = s;
		
		getBalance(NTY);						//������С��� 
					
		
		//NTYת����һ���������λ���� 
		NTY = NTY->lchild;			 
	}

}

//DFS�õ���������б���
void HufumanTree::get_huffuman_code(Node *T)
{
	if(T->lchild == NULL && T->rchild == NULL)
	{
		if(T->data!=130)
		{
			if(T->parent->lchild == NTY && T->parent!=root)
				T->code = T->parent->code;
			cout<<(char)T->data<<" "<<" Ȩֵ��"<<T->weight<<" ���룺"<<T->code<<endl;
		}
		
		return;
	} 
	T->lchild->code = (T->code)+"0";  
    get_huffuman_code(T->lchild);  
    T->rchild->code = (T->code)+"1";  
    get_huffuman_code(T->rchild);  
    return ;  //�����ݹ�
}

//��ӡ�շ������룬��ȡ�����ַ����� 
void HufumanTree::print(char ch, ofstream &out)
{
	char code[100];
	int cnt = 0;
	Node *p = NULL;
	//�ҵ�����ַ������е�λ�� 
	isIndata(ch, root, &p);
	
	//������ֱ���ߵ�������¼·�� 
//	while(p!=root && p!=NULL)
//	{
//		if(p == p->parent->lchild)
//		{
//			p=p->parent;
//			code[cnt++] = '0';
//		}
//		else if(p == p->parent->rchild)
//		{
//			p = p->parent;
//			code[cnt++] = '1';
//		}
//	}
//	code[cnt] = '\0';
//	reverse(code, code+cnt);
	//cout<<ch<<" "<<code<<endl;
	cout<<p->code;	
	out<<p->code;
	
} 

//���������Ľ���
void HufumanTree::swap(Node *p1, Node *p2)
{
	Node *tmp; 
	if(p1 == p1->parent->lchild){
		if(p2 == p2->parent->lchild){
			tmp = p1->parent;
			p1->parent = p2->parent;
			p2->parent = tmp;	
			p1->parent->lchild = p1;
			p2->parent->lchild = p2;		 
		}
		else if(p2 == p2->parent->rchild){
			tmp = p1->parent;
			p1->parent = p2->parent;
			p2->parent = tmp;	
			p1->parent->rchild = p1;
			p2->parent->lchild = p2;
		}
	}
	else if(p1 == p1->parent->rchild){
		if(p2 == p2->parent->lchild){
			tmp = p1->parent;
			p1->parent = p2->parent;
			p2->parent = tmp;	
			p1->parent->lchild = p1;
			p2->parent->rchild = p2;		 
		}
		else if(p2 == p2->parent->rchild){
			tmp = p1->parent;
			p1->parent = p2->parent;
			p2->parent = tmp;	
			p1->parent->rchild = p1;
			p2->parent->rchild = p2;
		}
	} 
}

//BFS�����ҵ���һ��Ȩ����ͬ�Ľڵ�
Node* HufumanTree::find_same_weight(Node *p)
{
	queue<Node*> qu;
	qu.push(root);
	Node *node;
	while(!qu.empty())
	{
		node = qu.front();
		if(node->weight == p->weight &&node != root)
			return node;
		qu.pop();
		//������ô�������ķ�ʽ���� 
		if(node->rchild)
			qu.push(node->rchild);
		if(node->lchild)
			qu.push(node->lchild);
	}
	return NULL;
}

//����
void  HufumanTree::Decode(string str)         
{
	Node *currt = root;
	string txt;
	for(int i=0; i<str.length(); ++i)
	{
		if(str[i] == '0')
			currt = currt->lchild;
		else if(str[i] == '1')
			currt = currt->rchild;
		
		if(currt->lchild == NTY && currt != root)              //����NYT 
			currt = currt->rchild;
		
		if(currt->lchild == NULL && currt->rchild == NULL)
		{
			if(currt->data!=130)
				txt+=(char)currt->data;
		//	cout<<txt<<endl;	
			currt = root;
		}
			
	}
	if(currt == root)
		cout<<txt<<endl;
	else
		cout<<"�������"<<endl;
	
}


int main()
{	
	int mode; 
	string str;
	cout<<"����������ģʽ(ע������Ӣ���ַ���)��\n1.�Ӽ��̶��룻\n";
	cout<<"2.�� huffman.txt ����\n";
	cout<<"3.չ�ֶ�̬����\n";
	cin>>mode;
	HufumanTree *hf = new HufumanTree();
	
	if (mode==3){
		int    tag=0;
		string ch;
		string front="";
		cout<<"�������ַ���(^�Ž���)"<<endl;
		getchar();
		while (getline(cin,ch,'\n')){					//֧�ֶ���ո� 
			if (ch=="^") break;
			tag=0;
			for(int i=0; i<ch.length(); ++i)
			{
				if (ch[i]<0)
				{
					cout<<"������Ϸ�Ӣ���ַ�!"<<endl; 					//��֧�ֶ������� 
					tag=1;
					break;
				}
				hf->insert(ch[i]);
			}
			if(tag)
				continue;
			front+=ch;
			hf->get_huffuman_code(hf->getRoot());
			ofstream out("result.txt", ios::out | ios::binary); 
			for(int i=0; i<front.length(); ++i)
			{
				hf->print(front[i], out);
			}
			cout<<endl;
			out.close();
			cout<<"�������ַ���"<<front;
		}
			
	} else {
		if (mode==1){
			cout<<"�������ַ���"<<endl;
			cin.get();
		//	cin>>str;
			getline(cin,str,'\n');                               //֧�ֶ���ո� 
			if(str[0]<0)
			{
				cout<<"�Ƿ�����!"<<endl;
				return -1; 
			}	
			
		} else if(mode==2){
			ifstream fin("huffman.txt", ios::in);
			ostringstream buf;
			char ch;
			while(buf && fin.get(ch))
				buf.put(ch);
			str = buf.str();
			fin.close();
			buf.str(""); //���buff 
		} 
		
		for(int i=0; i<str.length(); ++i)
		{
			hf->insert(str[i]);
		}

			
		hf->get_huffuman_code(hf->getRoot());
    
		ofstream out("result.txt", ios::out | ios::binary); 
		for(int i=0; i<str.length(); ++i)
		{
			hf->print(str[i], out);
		}
		cout<<endl;
		out.close();
		
	}
	 
    
	cout<<"������Ҫ����ı���:"<<endl;
	cin>>str;
	hf->Decode(str);
	
	
	return 0;
} 
