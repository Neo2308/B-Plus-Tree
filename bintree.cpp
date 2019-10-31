#include<bits/stdc++.h>
using namespace std;
int MAX=5;
class BPTree;
class Node
{
	public:
		bool leaf;
		int *key,size;
		Node **ptr;
		Node();
};
class BPTree
{
	public:
		Node *root;
		BPTree();
		void search(int);
		void insert(int);
		void insertInternal(int,Node*,Node*);
		Node *findParent(Node*,Node*);
		void display(Node *);
		void remove(int);
		void removeInternal(int,Node*);
};
Node::Node()
{
	key=new int[MAX];
	ptr= new Node*[MAX+1];
}
BPTree::BPTree()
{
	root=NULL;
}
Node * BPTree::findParent(Node *cursor,Node *child)
{
	if(cursor->leaf==true)
		return NULL;
	Node *parent=NULL;
	for(int i=0;i<cursor->size+1;i++)
	{
		if (cursor->ptr[i]==child)
			return cursor;
		else
		{
			parent=findParent(cursor->ptr[i],child);
			if(parent!=NULL)
				return parent;
		}
	}
	return NULL;
}
void BPTree::search(int x)
{
	if(root==NULL)
		cout<<"Tree is Empty\n";
	else
	{
		Node * cursor=root;
		while(cursor->leaf==false)
		{
			for(int i=0;i<cursor->size;i++)
			{
				if(x<cursor->key[i])
				{
					cursor=cursor->ptr[i];
					break;
				}
				else if(i+1==cursor->size)
				{
					cursor=cursor->ptr[i+1];
					break;
				}
			}
		}
		for(int i=0;i<cursor->size;i++)
			if(cursor->key[i]==x)
			{
				cout<<"Found\n";
				return;
			}
		cout<<"Not Found\n";
	}
}
void BPTree::insert(int x)
{
	if(root==NULL)
	{
		root = new Node;
		root->key[0]=x;
		root->leaf=true;
		root->size=1;
		cout<<"Root created and inserted "<<x<<"\n";
	}
	else
	{
		Node * cursor=root;
		Node *parent;
		while(cursor->leaf==false)
		{
			parent=cursor;
			for(int i=0;i<cursor->size;i++)
			{
				if(x<cursor->key[i])
				{
					cursor=cursor->ptr[i];
					break;
				}
				else if(i+1==cursor->size)
				{
					cursor=cursor->ptr[i+1];
					break;
				}
			}
		}
		if(cursor->size<MAX)
		{
			int i;
			for(i=0;i<cursor->size;i++)
				if(cursor->key[i]>x)
					break;
			for(int j=cursor->size;j>i;j--)
				cursor->key[j]=cursor->key[j-1];
			cursor->key[i]=x;
			cursor->size+=1;
			cursor->ptr[cursor->size]=cursor->ptr[cursor->size-1];
			cursor->ptr[cursor->size-1]=NULL;
			cout<<"Inserted "<<x<<" Successfully\n";
		}
		else
		{
			cout<<"Inserted "<<x<<" Successfully\n";
			cout<<"Overflow! Splitting Leaf\n";
			Node* newleaf=new Node;
			newleaf->leaf=true;
			int i,temp[MAX+1];
			for(i=0;i<MAX;i++)
				temp[i]=cursor->key[i];
			for(i=0;i<MAX;i++)
				if(temp[i]>x)
					break;
			for(int j=MAX;j>i;j--)
				temp[j]=temp[j-1];
			temp[i]=x;
			cursor->size=(MAX+1)/2;
			newleaf->size=MAX+1-(MAX+1)/2;
			for(i=0;i<cursor->size;i++)
				cursor->key[i]=temp[i];
			for(i=0;i<newleaf->size;i++)
				newleaf->key[i]=temp[cursor->size+i];
			cursor->ptr[cursor->size]=newleaf;
			newleaf->ptr[newleaf->size]=cursor->ptr[MAX];
			cursor->ptr[MAX]=NULL;
			if(cursor==root)
			{
				root =new Node;
				root->key[0]=newleaf->key[0];
				root->ptr[0]=cursor;
				root->ptr[1]=newleaf;
				root->ptr[2]=NULL;
				root->leaf=false;
				root->size=1;
				cout<<"Created new node\n";
			}
			else
			{
				insertInternal(newleaf->key[0],parent,newleaf);
			}
		}
	}
}
void BPTree::insertInternal(int x,Node* cursor,Node* child)
{
	if(cursor->size<MAX)
	{
		int i;
		for(i=0;i<cursor->size;i++)
			if(cursor->key[i]>x)
				break;
		for(int j=cursor->size;j>i;j--)
		{
			cursor->key[j]=cursor->key[j-1];
			cursor->ptr[j+1]=cursor->ptr[j];
		}
		cursor->key[i]=x;
		cursor->size++;
		cursor->ptr[i+1]=child;
		// for(i=0;i<cursor->size;i++)
		// 	cout<<cursor->key[i]<<" ";
	}
	else
	{
		cout<<"Overflow! Splitting internal node\n";
		Node* newleaf=new Node;
		newleaf->leaf=false;
		int i,temp[MAX+1];
		Node * temp2[MAX+2];
		temp2[MAX]=cursor->ptr[MAX];
		for(i=0;i<MAX;i++)
		{
			temp[i]=cursor->key[i];
			temp2[i]=cursor->ptr[i];
		}
		for(i=0;i<MAX;i++)
			if(temp[i]>x)
				break;
		for(int j=MAX;j>i;j--)
		{
			temp[j]=temp[j-1];
			temp2[j+1]=temp2[j];
		}
		temp[i]=x;
		temp2[i+1]=child;
		cursor->size=(MAX+1)/2;
		newleaf->size=MAX-(MAX+1)/2;
		for(i=0;i<cursor->size;i++)
		{
			cursor->key[i]=temp[i];
		}
		for(i=0;i<cursor->size+1;i++)
		{
			cursor->ptr[i]=temp2[i];
		}
		for(i=0;i<newleaf->size;i++)
		{
			newleaf->key[i]=temp[cursor->size+1+i];
		}
		for(i=0;i<newleaf->size+1;i++)
		{
			newleaf->ptr[i]=temp2[cursor->size+1+i];
		}
		// cursor->ptr[MAX]=NULL;
		if(cursor==root)
		{
			root =new Node;
			root->key[0]=temp[cursor->size];
			root->ptr[0]=cursor;
			root->ptr[1]=newleaf;
			root->ptr[2]=NULL;
			root->leaf=false;
			root->size=1;
			cout<<"Created new node\n";
		}
		else
		{
			insertInternal(temp[cursor->size],findParent(root,cursor),newleaf);
		}
	}	
}
void BPTree::remove(int x)
{
	if(root==NULL)
	{
		cout<<"Tree is empty\n";
		return;
	}
	int i;
	Node * cursor=root;
	Node * parent=NULL;
	int left=-1;
	int right=-1;
	while(cursor->leaf==false)
	{
		parent=cursor;
		for(i=0;i<cursor->size;i++)
		{
			if(x<cursor->key[i])
			{
				left=i-1;
				right=i+1;
				cursor=cursor->ptr[i];
				break;
			}
			else if(i+1==cursor->size)
			{
				left=i;
				right=i+2;
				cursor=cursor->ptr[i+1];
				break;
			}
		}
	}
	for(i=0;i<cursor->size;i++)
		if(cursor->key[i]==x)
			break;
	if(i==cursor->size)
	{
		cout<<x<<" Not Found\n";
		return;
	}
	cursor->size--;
	for(;i<cursor->size;i++)
		cursor->key[i]=cursor->key[i+1];
	cursor->ptr[cursor->size]=cursor->ptr[cursor->size+1];
	cursor->ptr[cursor->size+1]=NULL;
	cout<<"Deleted "<<x<<" Succesfully\n";
	if(cursor->size<(MAX+1)/2)
	{
		cout<<"Underflow occured!\n";
		cout<<left <<" "<<right<<"\n";
		if(cursor==root)
		{
			if(cursor->size==0)
			{
				Node *temp=root;
				root=NULL;
				delete(temp);
			}
		}
		if(left>=0)
		{
			Node * ls=parent->ptr[left];
			if(ls->size>(MAX+1)/2)
			{
				for(i=cursor->size;i>0;i--)
					cursor->key[i]=cursor->key[i-1];
				cursor->ptr[cursor->size+1]=cursor->ptr[cursor->size];
				cursor->ptr[cursor->size]=NULL;
				cursor->size++;
				ls->size--;
				parent->key[left]=ls->key[ls->size];
				cursor->key[0]=ls->key[ls->size];
				ls->ptr[ls->size]=ls->ptr[ls->size+1];
				ls->ptr[ls->size+1]=NULL;
				cout<<"Adjusted with left sibling\n";
				return;
			}
		}
		if(right<=parent->size)
		{
			Node * rs=parent->ptr[right];
			if(rs->size>(MAX+1)/2)
			{
				cursor->ptr[cursor->size+1]=cursor->ptr[cursor->size];
				cursor->ptr[cursor->size]=NULL;
				cursor->key[cursor->size]=rs->key[0];
				parent->key[right-1]=rs->key[0];
				cursor->size++;
				rs->size--;
				for(i=0;i<rs->size;i++)
					rs->key[i]=rs->key[i+1];
				rs->ptr[rs->size]=rs->ptr[rs->size+1];
				rs->ptr[rs->size+1]=NULL;
				cout<<"Adjusted with right sibling\n";
				return;
			}	
		}
		if(left>=0)
		{
			Node * ls=parent->ptr[left];
			if(ls->size+cursor->size<MAX+1)
			{
				for(i=0;i<cursor->size;i++)
				{
					ls->key[ls->size+i]=cursor->key[i];
				}
				ls->ptr[ls->size]=NULL;
				// ls->
				// for(i=0;i<cursor->size+1;i++)
				// {
				// 	ls->ptr[ls->size+i]=cursor->ptr[i];
				// }
				ls->size=ls->size+cursor->size;
				ls->ptr[ls->size]=cursor->ptr[cursor->size];
				cout<<"Merged with left sibling\n";
				removeInternal(parent->key[left],parent);
				return;
			}
		}
		if(right<=parent->size)
		{
			Node * rs=parent->ptr[right];
			if(cursor->size+rs->size<MAX+1)
			{
				for(i=0;i<rs->size;i++)
				{
					cursor->key[cursor->size+i]=rs->key[i];
				}
				cursor->ptr[cursor->size]=NULL;
				cursor->size=cursor->size+rs->size;
				cursor->ptr[cursor->size]=rs->ptr[rs->size];
				cout<<"Merged with right sibling\n";
				removeInternal(parent->key[right-1],parent);
				return;
			}
		}
		
	}
	return;
}
void BPTree::removeInternal(int x,Node* cursor)
{
	int i;
	for(i=0;i<cursor->size;i++)
		if(cursor->key[i]==x)
			break;
	cursor->size--;
	Node *temp=cursor->ptr[i+1];
	for(int j=i;j<cursor->size;j++)
	{
		cursor->key[j]=cursor->key[j+1];
		cursor->ptr[j+1]=cursor->ptr[j+2];
	}
	delete(temp);
	// display(root);
	// cout<<"\n";
	if(cursor->size<(MAX+1)/2)
	{
		cout<<"Underflow in internal node\n";
		if(cursor==root)
		{
			if(cursor->size==0)
			{
				temp=root;
				root=cursor->ptr[0];
				delete(temp);
			}
		}
		else
		{
			Node * parent=findParent(root,cursor);
			int left=-1;
			int right=-1;
			for(i=0;i<parent->size+1;i++)
			{
				if(cursor==parent->ptr[i])
				{
					left=i-1;
					right=i+1;
					break;
				}
				else if(i+1==parent->size+2)
				{
					left=i;
					right=i+2;
					break;
				}
			}
			// cout<<"Underflow occured!\n";
			cout<<left <<" "<<right<<"\n";
			if(left>=0)
			{
				Node * ls=parent->ptr[left];
				if(ls->size>(MAX+1)/2)
				{
					for(i=cursor->size;i>0;i--)
						cursor->key[i]=cursor->key[i-1];
					for(i=cursor->size+1;i>0;i++)
						cursor->ptr[i]=cursor->ptr[i-1];
					cursor->size++;
					ls->size--;
					parent->key[left]=ls->key[ls->size];
					cursor->key[0]=ls->key[ls->size];
					cursor->ptr[0]=ls->ptr[ls->size+1];
					ls->ptr[ls->size+1]=NULL;
					cout<<"Adjusted with left sibling\n";
					return;
				}
			}
			if(right<=parent->size)
			{
				Node * rs=parent->ptr[right];
				if(rs->size>(MAX+1)/2)
				{
					cursor->key[cursor->size]=parent->key[right-1];
					cursor->ptr[cursor->size+1]=rs->ptr[0];
					parent->key[right-1]=rs->key[0];
					cursor->size++;
					rs->size--;
					for(i=0;i<rs->size;i++)
					{
						rs->key[i]=rs->key[i+1];
					}
					for(i=0;i<rs->size+1;i++)
					{
						rs->ptr[i]=rs->ptr[i+1];
					}
					rs->ptr[rs->size+1]=NULL;
					cout<<"Adjusted with right sibling\n";
					return;
				}	
			}
			if(left>=0)
			{
				Node * ls=parent->ptr[left];
				if(ls->size+cursor->size<MAX)
				{
					ls->key[ls->size]=parent->key[left];
					for(i=0;i<cursor->size+1;i++)
					{
						ls->key[ls->size+1+i]=cursor->key[i];
					}
					for(i=0;i<cursor->size+2;i++)
					{
						ls->ptr[ls->size+1+i]=cursor->ptr[i];
					}
					ls->size=ls->size+cursor->size+1;
					cout<<"Merged with right sibling\n";
					removeInternal(parent->key[left],parent);
					return;
				}
			}
			if(right<=parent->size)
			{
				Node * rs=parent->ptr[right];
				if(cursor->size+rs->size<MAX)
				{
					cursor->key[cursor->size]=parent->key[right-1];
					for(i=0;i<rs->size+1;i++)
					{
						cursor->key[cursor->size+1+i]=rs->key[i];
					}
					for(i=0;i<rs->size+2;i++)
					{
						cursor->ptr[cursor->size+1+i]=rs->ptr[i];
					}
					cursor->size=cursor->size+rs->size+1;
					cout<<"Merged with right sibling\n";
					removeInternal(parent->key[right-1],parent);
					return;
				}
			}	
		}
	}
	return;
}
void BPTree::display(Node * cursor)
{
	if(cursor==NULL)
	{
		cout<<"Tree is empty";
		return;
	}
	cout<<"[ ";
	for(int i=0;i<cursor->size;i++)
		cout<<cursor->key[i]<<" ";
	if(cursor->leaf==false)
	{
		cout<<" : < ";
		for(int i=0;i<cursor->size+1;i++)
		{
			// if(cursor->ptr[i]!=NULL)
				display(cursor->ptr[i]);
		}
		cout<<" > ";
	}
	cout<<" ] ";
}
int main()
{
	BPTree bpt;
	bpt.insert(1);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(2);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(3);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(4);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(5);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(11);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(12);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(13);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(14);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(15);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(21);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(22);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(23);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(24);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(25);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.insert(26);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.remove(25);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.remove(15);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.remove(11);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.remove(14);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.remove(15);
	bpt.display(bpt.root);
	cout<<"\n";
	bpt.remove(4);
	bpt.display(bpt.root);
	cout<<"\n";
	
	return 0;
}
