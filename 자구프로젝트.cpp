#define BLACK 1
#define RED 0
#include <stdio.h>
#include <stdlib.h>
int bh = 0;
int total = 0;
int tr = 0;

typedef struct Node*NodePtr;
struct Node
{
	int val;
	NodePtr parent, left, right;
	int c;
};

NodePtr nil;

NodePtr node_alloc(int newval)
{
	NodePtr T = (NodePtr)malloc(sizeof(struct Node));
	T->val = newval;
	T->parent = nil;
	T->left = nil;
	T->right = nil;
	//T->c = BLACK;
	return T;
}

NodePtr nil_init()
{
	nil = (NodePtr)malloc(sizeof(struct Node));
	nil->val = -1;
	nil->c = BLACK;
	nil->left = nil->right = NULL;

	return nil;

}
typedef struct RBT* RBTPtr;
struct RBT
{
	NodePtr root, NIL;
};


RBTPtr rbt_alloc()
{
	RBTPtr T = (RBTPtr)malloc(sizeof(struct RBT));
	T->NIL = nil;
	T->root = T->NIL;
	return T;
}


void Left_Rotate(RBTPtr T, NodePtr n)
{
	NodePtr y;
	y = n->right;
	n->right = y->left;
	if (y->left != T->NIL)
		y->left->parent = n;
	y->parent = n->parent;
	if (n->parent == T->NIL)
		T->root = y;
	else if (n == n->parent->left)
		n->parent->left = y;
	else
		n->parent->right = y;
	y->left = n;
	n->parent = y;
}


void Right_Rotate(RBTPtr T, NodePtr n)
{
	NodePtr y;
	y = n->left;
	n->left = y->right;
	if (y->right != T->NIL)
		y->right->parent = n;
	y->parent = n->parent;
	if (n->parent == T->NIL)
		T->root = y;
	else if (n == n->parent->left)
		n->parent->left = y;
	else
		n->parent->right = y;
	y->right = n;
	n->parent = y;
}


void rbt_insert_fixup(RBTPtr T, NodePtr n)
{
	NodePtr y;
	while (n->parent->c == RED)
	{
		if (n->parent == n->parent->parent->left)
		{
			y = n->parent->parent->right;
			if (y != T->NIL && y->c == RED)
			{
				n->parent->c = BLACK;
				y->c = BLACK;
				n->parent->parent->c = RED;
				n = n->parent->parent;
			}
			else
			{
				if (n == n->parent->right)
				{
					n = n->parent;
					Left_Rotate(T, n);
				}
				n->parent->c = BLACK;
				n->parent->parent->c = RED;
				Right_Rotate(T, n->parent->parent);
			}
		}
		else
		{
			y = n->parent->parent->left;
			if (y->c == RED)
			{
				n->parent->c = BLACK;
				y->c = BLACK;
				n->parent->parent->c = RED;
				n = n->parent->parent;
			}
			else
			{
				if (n == n->parent->left)
				{
					n = n->parent;
					Right_Rotate(T, n);
				}
				n->parent->c = BLACK;
				n->parent->parent->c = RED;
				Left_Rotate(T, n->parent->parent);
			}
		}
	}
	T->root->c = BLACK;
}


void rbt_insert(RBTPtr T, NodePtr n)
{
	NodePtr y = T->NIL;
	NodePtr x = T->root;
	while (x != T->NIL)
	{
		y = x;
		if (n->val < x->val)
			x = x->left;
		else
			x = x->right;
	}
	n->parent = y;
	if (y == T->NIL)
		T->root = n;
	else if (n->val < y->val)
		y->left = n;
	else
		y->right = n;
	n->left = T->NIL;
	n->right = T->NIL;
	n->c = RED;
	rbt_insert_fixup(T, n);
}
void rbt_transplant(RBTPtr T, NodePtr u, NodePtr v)
{
	if (u->parent == T->NIL)
		T->root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	v->parent = u->parent;
}
NodePtr tree_min(RBTPtr T, NodePtr x)
{
	while (x->left != T->NIL)
		x = x->left;
	return x;
}

NodePtr tree_max(RBTPtr T, NodePtr x) {
	while (x->right != T->NIL)
		x = x->right;
	return x;
}

void rbt_delete_fixup(RBTPtr T, NodePtr n)
{
	NodePtr w;
	while (n != T->root && n->c == BLACK)
	{
		if (n == n->parent->left)
		{
			w = n->parent->right;
			if (w->c == RED)
			{
				w->c = BLACK;
				n->parent->c = RED;
				Left_Rotate(T, n->parent);
				w = n->parent->right;
			}
			if (w->left->c == BLACK && w->right->c == BLACK)
			{
				w->c = RED;
				n = n->parent;
			}
			else
			{
				if (w->right->c == BLACK)
				{
					w->left->c = BLACK;
					w->c = RED;
					Right_Rotate(T, w);
					w = n->parent->right;
				}
				w->c = n->parent->c;
				n->parent->c = BLACK;
				w->right->c = BLACK;
				Left_Rotate(T, n->parent);
				n = T->root;
			}
		}
		else
		{
			w = n->parent->left;
			if (w->c == RED)
			{
				w->c = BLACK;
				n->parent->c = RED;
				Right_Rotate(T, n->parent);
				w = n->parent->left;
			}
			if (w->right->c == BLACK && w->left->c == BLACK)
			{
				w->c = RED;
				n = n->parent;
			}
			else
			{
				if (w->left->c == BLACK)
				{
					w->right->c = BLACK;
					w->c = RED;
					Left_Rotate(T, w);
					w = n->parent->left;
				}
				w->c = n->parent->c;
				n->parent->c = BLACK;
				w->left->c = BLACK;
				Right_Rotate(T, n->parent);
				n = T->root;
			}
		}

	}
	n->c = BLACK;
}
void rbt_delete(RBTPtr T, NodePtr n)
{
	NodePtr y, x;
	y = n;
	int y_ori_color = y->c;
	if (n->left == T->NIL)
	{
		x = n->right;
		rbt_transplant(T, n, n->right);
	}
	else if (n->right == T->NIL)
	{
		x = n->left;
		rbt_transplant(T, n, n->left);
	}
	else
	{
		y = tree_min(T, n->right);
		y_ori_color = y->c;
		x = y->right;
		if (y->parent == n)
			x->parent = y;
		else
		{
			rbt_transplant(T, y, y->right);
			y->right = n->right;
			y->right->parent = y;
		}
		rbt_transplant(T, n, y);
		y->left = n->left;
		y->left->parent = y;
		y->c = n->c;
	}
	if (y_ori_color == BLACK)
		rbt_delete_fixup(T, x);
}

void rbt_print(RBTPtr T, NodePtr tree, int level)
{
	if (tree->right != T->NIL)
		rbt_print(T, tree->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("   ");
	printf("%d%s  \n", tree->val, !(tree->c) ? "R" : "B");
	if (tree->left != T->NIL)
		rbt_print(T, tree->left, level + 1);
}



NodePtr rbt_search(RBTPtr T, NodePtr x, int val)
{
	if (x == T->NIL || x->val == val)
		return x;
	else if (val <= x->val)
		return rbt_search(T, x->left, val);
	else
		return rbt_search(T, x->right, val);
}


NodePtr rbt_successor(RBTPtr self, NodePtr x)
{
	if (x->right != self->NIL)
		return tree_min(self, x->right);

	NodePtr y = x->parent;

	while (y != self->NIL && x == y->right){
		x = y;
		y = y->parent;
	}
	return y;
}


NodePtr rbt_predecessor(RBTPtr self, NodePtr x)
{
	if (x->left != self->NIL)
		return tree_max(self, x->left);

	NodePtr y = x->parent;

	while (y != self->NIL && x == y->left){
		x = y;
		y = y->parent;
	}
	return y;
}


int main()
{
	int data;
	int data2;
	nil = nil_init();
	FILE *fp;
	FILE *fp2;
	FILE *fp3;
	RBTPtr rbt = rbt_alloc();

	fp = fopen("C:\\Users\\김정환\\Desktop\\KU\\자료구조\\prjtest\\prjtest\\test01.txt", "r");

	while (!feof(fp))
	{
		fscanf(fp, "%d", &data);
		if (data > 0)
		{
			rbt_insert(rbt, node_alloc(data));
		}

		else if (data == 0)
			break;

		else if (data < 0)
		{
			NodePtr delnode = rbt_search(rbt, rbt->root, data*-1);
			if (delnode->val != -1)
			{
				rbt_delete(rbt, delnode);
			}
		}
	}
	fclose(fp);
	rbt_print(rbt, rbt->root, 0);
	printf("\n\n\n\n");

	NodePtr search;
	fp2 = fopen("C:\\Users\\김정환\\Desktop\\KU\\자료구조\\prjtest\\prjtest\\search01.txt", "r");
	fp3 = fopen("C:\\Users\\김정환\\Desktop\\KU\\자료구조\\prjtest\\prjtest\\output00.txt", "w");
	while (!feof(fp2))
	{
		fscanf(fp2, "%d", &data2);
		search = rbt_search(rbt, rbt->root, data2);
		if (search->val == -1) {
			rbt_insert(rbt, node_alloc(data2));
			search = rbt_search(rbt, rbt->root, data2);

			NodePtr big = rbt_successor(rbt, search);
			NodePtr small = rbt_predecessor(rbt, search);

			if (small->val == -1)
				fprintf(fp3,"NIL  ");
			else
				fprintf(fp3,"%d  ", small->val);
			fprintf(fp3,"NIL  ");
			if (big->val == -1)
				fprintf(fp3,"NIL  \n\n");
			else
				fprintf(fp3,"%d  \n\n", big->val);
			rbt_delete(rbt, search);
		}
		else {

			NodePtr big = rbt_successor(rbt, search);
			NodePtr small = rbt_predecessor(rbt, search);

			if (small->val == -1)
				fprintf(fp3,"NIL  ");
			else
				fprintf(fp3,"%d  ", small->val);
			fprintf(fp3,"%d  ", search->val);
			if (big->val == -1)
				fprintf(fp3,"NIL  \n\n");
			else
				fprintf(fp3,"%d  \n\n", big->val);

		}
	}
	fclose(fp2);
	fclose(fp3);
	return 0;
}