#include <errno.h>
#include <stdlib.h>
#include "bst.h"

static void bst_free_node(bst_t *tree, bst_node_t *node)
{
	if (node) {
		bst_free_node(tree, node->left);
		bst_free_node(tree, node->right);
		tree->free(node->data);
		free(node);
	}
}

static bst_node_t *bst_alloc_node(void *data)
{
	bst_node_t *node = malloc(sizeof(*node));

	if (ENOMEM == errno) {
		return NULL;
	}

	node->data = data;
	node->left = NULL;
	node->right = NULL;

	return node;
}

bst_t *bst_create(bst_cmp_t cmp, bst_select_t select, bst_free_data_t free)
{
	bst_t *tree = malloc(sizeof(*tree));
	
	if (ENOMEM == errno) {
		return NULL;
	}

	tree->cmp = cmp;
	tree->free = free;
	tree->root = NULL;
	tree->select = select;
	tree->size = 0;

	return tree;
}

static void bst_insert_node(bst_t *tree, bst_node_t *parent, bst_node_t *child)
{
	int comparison = tree->cmp(child->data, parent->data);

	if (0 > comparison) {
		if (parent->left) {
			bst_insert_node(tree, parent->left, child);
		} else {
			parent->left = child;
			++tree->size;
		}
	} else if (0 < comparison) {
		if (parent->right) {
			bst_insert_node(tree, parent->right, child);
		} else {
			parent->right = child;
			++tree->size;
		}
	} else {
		tree->select(parent->data, child->data);
		bst_free_node(tree, child);
	}
}

void bst_insert(bst_t *tree, void *data)
{
	if (!(tree && data)) {
		return;
	}

	bst_node_t *node = bst_alloc_node(data);

	if (!tree->root) {
		tree->root = node;
		++tree->size;
	} else {
		bst_insert_node(tree, tree->root, node);
	}
}

static void bst_foreach_node(const bst_node_t *node, bst_foreach_t foreach, void *user_data)
{
	if (node) {
		bst_foreach_node(node->left, foreach, user_data);
		foreach(node->data, user_data);
		bst_foreach_node(node->right, foreach, user_data);
	}
}

void bst_foreach(const bst_t *tree, bst_foreach_t foreach, void *user_data)
{
	if (tree) {
		bst_foreach_node(tree->root, foreach, user_data);
	}
}

void bst_free(bst_t *tree)
{
	if (tree) {
		bst_free_node(tree, tree->root);
		free(tree);
	}
}

