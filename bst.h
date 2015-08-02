#ifndef __H_6C10EE8906BD498CAA7DFC864388DB07__
#define __H_6C10EE8906BD498CAA7DFC864388DB07__

#include <inttypes.h>

typedef struct bst_node_t {
	struct bst_node_t *left;
	struct bst_node_t *right;
	void *data;
} bst_node_t;

typedef int (*bst_cmp_t)(const void *p1, const void *p2);
typedef void (*bst_select_t)(void *selected, void *alternative);
typedef void (*bst_free_data_t)(void *data);
typedef void (*bst_foreach_t)(const void *data, void *user_data);

typedef struct {
	bst_cmp_t cmp;
	bst_free_data_t free;
	bst_node_t *root;
	bst_select_t select;
	uint32_t size;
} bst_t;

inline static uint32_t bst_size(const bst_t *tree)
{
	return tree ? tree->size : 0;
}

bst_t *bst_create(bst_cmp_t cmp, bst_select_t select, bst_free_data_t free);
void bst_free(bst_t *tree);
void bst_insert(bst_t *tree, void *data);
void bst_foreach(const bst_t *tree, bst_foreach_t foreach, void *user_data);

#endif
