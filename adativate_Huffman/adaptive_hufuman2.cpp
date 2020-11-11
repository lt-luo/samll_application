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
		//字符用整型表示，方便遍历的时候区分叶节点 
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
		char data[130];			//ASCLL有128个字符 
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

//构造函数，对成员初始化 
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

//DFS遍历，检测输入的字符是否在树中          
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
	//先做交换再一起加权
	while(p!=root)			
	{
		same_weight_node = find_same_weight(p);
		if(same_weight_node!=p && p->parent!=same_weight_node){                //理论来讲这里也不会有NULL   p->parent!=same_weight是为了防止parent是根节点的情况 
			swap(same_weight_node, p);
		}
		p = p->parent;								
	}
	//换完加权 
	p = current;
	while(p)
	{
		p->weight++;
		p = p->parent;
	}
}

//对新输入的字符插入到赫夫曼树中 
void HufumanTree::insert(char ch)
{
	Node *cur = NULL;
	isIndata(ch, root, &cur);   //判断字符是否在Huffmna树中
	if(cur)						//p指向字符在树中的叶节点位置 
	{
		//调整赫夫曼树 
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
		
		getBalance(NTY);						//调整减小深度 
					
		
		//NTY转到下一个待插入的位置上 
		NTY = NTY->lchild;			 
	}

}

//DFS得到并输出所有编码
void HufumanTree::get_huffuman_code(Node *T)
{
	if(T->lchild == NULL && T->rchild == NULL)
	{
		if(T->data!=130)
		{
			if(T->parent->lchild == NTY && T->parent!=root)
				T->code = T->parent->code;
			cout<<(char)T->data<<" "<<" 权值："<<T->weight<<" 编码："<<T->code<<endl;
		}
		
		return;
	} 
	T->lchild->code = (T->code)+"0";  
    get_huffuman_code(T->lchild);  
    T->rchild->code = (T->code)+"1";  
    get_huffuman_code(T->rchild);  
    return ;  //结束递归
}

//打印赫夫曼编码，获取单个字符编码 
void HufumanTree::print(char ch, ofstream &out)
{
	char code[100];
	int cnt = 0;
	Node *p = NULL;
	//找到这个字符在树中的位置 
	isIndata(ch, root, &p);
	
	//向上走直到走到根，记录路径 
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

//两个子树的交换
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

//BFS遍历找到第一个权重相同的节点
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
		//这里采用从右往左的方式遍历 
		if(node->rchild)
			qu.push(node->rchild);
		if(node->lchild)
			qu.push(node->lchild);
	}
	return NULL;
}

//译码
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
		
		if(currt->lchild == NTY && currt != root)              //跳过NYT 
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
		cout<<"编码错误"<<endl;
	
}


int main()
{	
	int mode; 
	string str;
	cout<<"请输入运行模式(注意输入英文字符！)：\n1.从键盘读入；\n";
	cout<<"2.从 huffman.txt 读入\n";
	cout<<"3.展现动态过程\n";
	cin>>mode;
	HufumanTree *hf = new HufumanTree();
	
	if (mode==3){
		int    tag=0;
		string ch;
		string front="";
		cout<<"请输入字符：(^号结束)"<<endl;
		getchar();
		while (getline(cin,ch,'\n')){					//支持读入空格 
			if (ch=="^") break;
			tag=0;
			for(int i=0; i<ch.length(); ++i)
			{
				if (ch[i]<0)
				{
					cout<<"请输入合法英文字符!"<<endl; 					//不支持读入中文 
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
			cout<<"请输入字符："<<front;
		}
			
	} else {
		if (mode==1){
			cout<<"请输入字符："<<endl;
			cin.get();
		//	cin>>str;
			getline(cin,str,'\n');                               //支持读入空格 
			if(str[0]<0)
			{
				cout<<"非法输入!"<<endl;
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
			buf.str(""); //清空buff 
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
	 
    
	cout<<"请输入要翻译的编码:"<<endl;
	cin>>str;
	hf->Decode(str);
	
	
	return 0;
} 
