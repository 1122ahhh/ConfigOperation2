#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Properties.h"

#define KEY_SIZE        128 // key缓冲区大小
#define VALUE_SIZE      1024 // value缓冲区大小

#define LINE_BUF_SIZE   1152 // 读取配置文件中每一行的缓冲区大小

typedef Node* AvlTree;
typedef struct Node {
	char key[KEY_SIZE];
	char value[VALUE_SIZE];
	struct Node* left;
	struct Node* right;
	int height;
}Node;

Node* creat_node(char* key, char* value, Node* left, Node* right) {
	Node *node = (Node*)malloc(sizeof(Node));
	

	if (node == NULL) {
		printf("创建节点失败！！！\n");
		return NULL;
	}

	strcpy_s(node->key, strlen(key) + 1, key);
	strcpy_s(node->value, strlen(value) + 1, value);
	node->left = left;
	node->right = right;
	node->height = 0;

	return node;
}

int Height(Node *node) {
	int height = (node == NULL) ? -1 : node->height;
	return height;
}

int MAX(int a, int b) {
	int max = a >= b ? a : b;
	return max;
}

AvlTree LL_rotation(AvlTree tree) {
	Node *node = tree->left;
	tree->left = node->right;
	node->right = tree;
    // 重新调整树的高度
	tree->height = MAX(Height(tree->left), Height(tree->right)) + 1;
    node->height = MAX(Height(node->left), Height(node->right)) + 1;
	 
	return node;
}

AvlTree RR_rotation(AvlTree tree) {
	Node *node = tree->right;
	tree->right = node->left;
	node->left = tree;
	// 重新调整树的高度
	tree->height = MAX(Height(tree->left), Height(tree->right)) + 1;
	node->height = MAX(Height(node->left),Height(node->right)) + 1;

	return node;
}

//AvlTree LR_rotation(AvlTree tree) {
//	Node *node = tree->left->right, *temp = tree->left;
//	tree->left->right = node->left;
//	tree->left = node->right;
//	node->left = temp;
//	node->right = tree;
//	// 重新调整树的高度
//	tree->height = MAX(getNode_height(tree->left), getNode_height(tree->right)) + 1;
//	temp->height = MAX(getNode_height(temp->left), getNode_height(temp->right)) + 1;
//	node->height = MAX(getNode_height(node->left), getNode_height(node->right)) + 1;
//
//	return node;
//}

AvlTree LR_rotation(AvlTree tree) {
	
	tree->left = RR_rotation(tree->left);
	tree = LL_rotation(tree);
    return tree;
}

AvlTree RL_rotation(AvlTree tree) {

	tree->right = LL_rotation(tree->right);
	tree =RR_rotation(tree);
	return tree;
}

AvlTree Insert(AvlTree tree, char *key, char *value) {
	
	if (tree == NULL) {
		Node *node = creat_node(key, value, NULL, NULL);
		tree = node;
	}

	else if (strcmp(key, tree->key) < 0) {
		tree->left = Insert(tree->left, key, value);
		// 插入新节点后检查平衡性
		if (Height(tree->left) - Height(tree->right) == 2) {
			if (strcmp(key, tree->left->key) < 0)
				tree = LL_rotation(tree);
			else
				tree = LR_rotation(tree);
		}
	}
	else if (strcmp(key, tree->key) > 0) {
		tree->right = Insert(tree->right, key, value);
	    // 插入新节点后检查平衡性
		if (Height(tree->right) - Height(tree->left) == 2) {
			if (strcmp(key, tree->right->key) > 0)
				tree = RR_rotation(tree);
			else
				tree = RL_rotation(tree);
		}
	}
	else {
		printf("Warning!!! Key = %s ，key值出现重复 \n", key);
	}

	tree->height = MAX(Height(tree->left), Height(tree->right)) + 1;

	return tree;
}

Node *Search(AvlTree tree, char *key) {
	if (tree == NULL) return NULL;

	int flag = strcmp(key, tree->key);
	if (flag < 0)
		return Search(tree, key);
	else if (flag > 0)
		return Search(tree, key);
	else return tree;

}

Node* MinNode(AvlTree tree){
	Node* node = tree;
	while (node->left != NULL)
		node = node->left;
	return node;
}

AvlTree Delete(AvlTree tree, char *key) {
	Node *node = Search(tree, key);
	if (tree == NULL || node == NULL)
		return tree;

	if (strcmp(key, tree->key) < 0) {
		tree->left = Delete(tree->left, key);

		if (Height(tree->right) - Height(tree->left) == 2) {
			if (Height(tree->right->left) - Height(tree->right->right) >= 1)
				tree = RL_rotation(tree);
			else
				tree = RR_rotation(tree);
		}
	}

	else if (strcmp(key, tree->key) > 0) {
		tree->right = Delete(tree->right, key);

		if (Height(tree->left) - Height(tree->right) == 2) {
			if (Height(tree->left->right) - Height(tree->left->left) >= 1)
				tree = LR_rotation(tree);
			else
				tree = LL_rotation(tree);
		}
	}
	else {
		if (!tree->left && !tree->right) tree = NULL;
		else if (!tree->left) tree = tree->right;
		else if (!tree->right) tree = tree->left;
		else {
			Node* node = MinNode(tree->right);
			strcpy_s(tree->key, strlen(node->key) + 1, node->key);
			strcpy_s(tree->key, strlen(node->key) + 1, node->key);
			tree->right = Delete(tree->right, node->key);
		}
	}
	if(tree)
		tree->height = MAX(Height(tree->left), Height(tree->right)) + 1;

	return tree;
}

char* CreateSavePathName(const char* filepath, const char* add) {
	int i = strlen(filepath) - 1;
	for (; i >= 0; i--)
		if (filepath[i] == '.')
			break;
	char savePath[50];

	strncat_s(savePath, filepath, i);
	strncat_s(savePath, add, strlen(add));
	strncat_s(savePath, filepath + i, strlen(filepath + i));

	return savePath;
}

void PreOrderPrint(AvlTree tree, FILE *fp) {
	if (!tree)
		return;
	printf("%s = %s\n", tree->key, tree->value);
	fprintf(fp, "%s=%s\n", tree->key, tree->value);
	PreOrderPrint(tree->left, fp);
	PreOrderPrint(tree->right, fp);
	return;
}


void PreOrderSave(AvlTree tree, const char* filepath) {

	char* savePath = CreateSavePathName(filepath, "_PreOrder");

	FILE *fp = NULL;
	fopen_s(&fp, savePath, "w");
	PreOrderPrint(tree, fp);
	fclose(fp); // 关闭文件
}

void InOrderPrint(AvlTree tree, FILE *fp) {
	if (!tree)
		return;
	InOrderPrint(tree->left,fp);
	printf("%s = %s\n", tree->key, tree->value);
	fprintf(fp, "%s=%s\n", tree->key, tree->value);
	InOrderPrint(tree->right,fp);
	return;
}
void InOrderSave(AvlTree tree, const char* filepath) {

	char* savePath = CreateSavePathName(filepath, "_InOrder");

	FILE *fp = NULL;
	fopen_s(&fp, savePath, "w");
	InOrderPrint(tree, fp);
	fclose(fp); // 关闭文件
}

void PostOrderPrint(AvlTree tree, FILE *fp) {
	if (!tree)
		return;
	PostOrderPrint(tree->left, fp);
	PostOrderPrint(tree->right, fp);
	printf("%s = %s\n", tree->key, tree->value);
	fprintf(fp, "%s=%s\n", tree->key, tree->value);
	return;
}

void PostOrderSave(AvlTree tree, const char* filepath) {

	char* savePath = CreateSavePathName(filepath, "_PostOrder");

	FILE *fp = NULL;
	fopen_s(&fp, savePath, "w");
	PostOrderPrint(tree, fp);
	fclose(fp); // 关闭文件
}

void ThreeOrderSave(AvlTree tree, const char* filepath) {
	PreOrderSave(tree, filepath);
	InOrderSave(tree, filepath);
	PostOrderSave(tree, filepath);
}

AvlTree BuildAvlTree(void* handle) {
	AvlTree tree=NULL;
	char **keys = NULL, **values = NULL;
	int keyscount, valuescount;

	getKeys(handle, &keys, &keyscount);//获取配置文件key列表
	getValues(handle, &values, &valuescount);//获取配置文件value列表

	for (int i = 0; i < keyscount; i++)
		tree = Insert(tree, keys[i], values[i]);

	return tree;
}

void SaveToAvlTree(void* handle, const char* filepath) {
	AvlTree tree = BuildAvlTree(handle);
	int choice;

	printf("Please input:1 for PreOrderSaveAndPrint\n");
	printf("Please input:2 for InOrderSaveAndPrint\n");
	printf("Please input:3 for PostOrderSaveAndPrint\n");
	printf("Please input:4 for Pre, In, PostOrderSaveAndPrint\n");
	printf("Please Input your Save and PrintOrder choice:");

	
	scanf_s("%d", &choice);
	switch (choice) {
	case 1:PreOrderSave(tree, filepath); break;
	case 2:InOrderSave(tree, filepath);break;
	case 3:PostOrderSave(tree, filepath); break;
	case 4:ThreeOrderSave(tree, filepath); break;
	default:break;
	}

	return;
}