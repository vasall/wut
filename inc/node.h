#ifndef _FH_NODE_H
#define _FH_NODE_H


#include "define.h"
#include "imports.h"


#define FH_NODE_NAME_LIM	128
#define FH_NODE_CHILDREN_LIM	128


enum fh_node_type {
	FH_NODE_BODY,
	FH_NODE_BLOCK,
	FH_NODE_TEXT,
	FH_NODE_BUTTON,
	FH_NODE_INPUT,
	FH_NODE_IMAGE,
	FH_NODE_CUSTOM
};


struct fh_node {
	/* The name of the node */
	char name[FH_NODE_NAME_LIM];

	/* The unique identifier for this node */
	u32 id;

	/* The layer this node is on */
	u32 layer;

	/* The parent node, this node is attached to */
	struct fh_node *parent;

	/* The children nodes that are attached to this node */
	u8 children_num;
	struct fh_node *children[FH_NODE_CHILDREN_LIM];

	/* The type of node */
	enum fh_node_type type;	
};



/*
 * Allocate and create a new node. Then depending on the given type, load
 * template styles.
 *
 * @name: The name of the node
 * @type: The type of node
 *
 * Returns: Either a pointer to the newly created node or NULL if an error
 * 	    occurred
 */
FH_API struct fh_node *fh_node_create(char *name, enum fh_node_type type);


/*
 * Destroy a node and free the allocated memory.
 *
 * @node: Pointer to the node to destroy
 */
FH_API void fh_node_destroy(struct fh_node *node);







#endif /* _FH_NODE_H */
