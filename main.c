#include "fbx.h"

#include <stdio.h>

void PrintNodeNames(int gap, FBXNodeList *node_list)
{
    // Loop over each node
    for (int i = 0; i < node_list->length; ++i) {
        FBXNode *node = &node_list->nodes[i];

        // Print node name
        for (int j = 0; j < gap; ++j) {
            printf("\t");
        }
        printf("- %s\n", node->name);

        // Print sub node_list
        if (node->subnodes != NULL) {
            PrintNodeNames(gap + 1, node->subnodes);
        }
    }
}

int main(int argc, char *argv[])
{
    // Get file path
    const char *fbx_path;
    if (argc <= 1) {
        fbx_path = "a.fbx"; // fallback file name
    } else {
        fbx_path = argv[1];
    }

    // Parse .FBX file
    FBXNodeList *node_list = FBX_Parse(fbx_path);
    if (node_list == NULL) {
        printf("Failed to open \"%s\"\n", fbx_path);
        return 1;
    }

    // Print node names
    PrintNodeNames(0, node_list);

    return 0;
}
