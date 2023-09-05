#include "fbx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Create and initialize an FBXNodeList with a specified initial capacity
 *
 * @param initial_capacity The number of FBXNode items the list can initially hold before a resize may be required
 *
 * @return On successful creation, returns a pointer to the new FBXNodeList
 *         Otherwise, the function returns NULL
 */
FBXNodeList *FBXNodeList_Create(size_t initial_capacity)
{
    FBXNodeList *list = malloc(sizeof(FBXNodeList));
    if (list == NULL) {
        return NULL;
    }

    list->length = 0;
    list->capacity = initial_capacity;

    if (initial_capacity > 0) {
        list->nodes = malloc(sizeof(FBXNode) * initial_capacity);
        if (list->nodes == NULL) {
            free(list);
            return NULL;
        }
    } else {
        list->nodes = NULL;
    }

    return list;
}

/**
 * Resizes the internal nodes array of an FBXNodeList to the specified capacity
 *
 * @param list Pointer to the FBXNodeList that should be resized
 * @param capacity The new capacity for the FBXNodeList nodes array
 *
 * @return Returns true if the resizing operation is successful
 *         Returns false if memory allocation or reallocation fails
 */
bool FBXNodeList_Resize(FBXNodeList *list, size_t capacity)
{
    void *memory_address;
    if (list->nodes == NULL) {
        memory_address = malloc(sizeof(FBXNode));
    } else {
        memory_address = realloc(list->nodes, sizeof(FBXNode) * capacity);
    }
    if (memory_address == NULL) {
        return false;
    }
    list->nodes = memory_address;
    list->capacity = capacity;
    return true;
}

/**
 * Truncates the internal nodes array of an FBXNodeList to its current length
 *
 * @param list Pointer to the FBXNodeList that should be truncated
 *
 * @return Returns true if the truncation operation is successful or if no truncation was needed
 *         Returns false if resizing the memory fails
 */
bool FBXNodeList_Truncate(FBXNodeList *list)
{
    if (list->length == list->capacity) {
        return true;
    }
    return FBXNodeList_Resize(list, list->length);
}

/**
 * Inserts a new node into an FBXNodeList
 *
 * If the list's current capacity is insufficient to accommodate the new node, the function will attempt to grow the capacity
 *
 * @param list Pointer to the FBXNodeList into which the node should be inserted
 * @param node Pointer to the FBXNode that should be inserted into the list
 *
 * @return Returns true if the node is successfully inserted into the list
 *         Returns false if there's a failure during memory resizing
 */
bool FBXNodeList_Insert(FBXNodeList *list, FBXNode *node)
{
    size_t new_length = list->length + 1;
    if (new_length > list->capacity) {

        size_t new_capacity = list->capacity + list->capacity; // grow capacity
        if (new_capacity == 0) {
            new_capacity = 1;
        }

        if (FBXNodeList_Resize(list, new_capacity) == false) {
            return false;
        }
    }

    memcpy(&list->nodes[list->length], node, sizeof(FBXNode));
    list->length = new_length;

    return true;
}

/**
 * Parses a single property of an FBX node from an open .FBX file
 *
 * @param file Pointer to the open .FBX file
 * @param property Pointer to an FBXNodeProperty structure where the parsed property details will be stored
 *
 * @return Returns true if the node property is successfully parsed from the file
 *         Returns false if an error occurs during the reading
 */
bool FBX_ParseNodeProperty(FILE *file, FBXNodeProperty *property)
{
    // Read property type
    fread(&property->type, sizeof(uint8_t), 1, file);

    // Read property value
    switch (property->type) {
        case 'Y': { /* i16 */
            fread(&property->Y.value, sizeof(int16_t), 1, file);
            break;
        }
        case 'C': { /* u8 (bool) */
            fread(&property->C.value, sizeof(bool), 1, file);
            break;
        }
        case 'I': { /* i32 */
            fread(&property->I.value, sizeof(int32_t), 1, file);
            break;
        }
        case 'F': { /* f32 */
            fread(&property->F.value, sizeof(float), 1, file);
            break;
        }
        case 'D': { /* f64 */
            fread(&property->D.value, sizeof(double), 1, file);
            break;
        }
        case 'L': { /* i64 */
            fread(&property->L.value, sizeof(int64_t), 1, file);
            break;
        }
        case 'f': { /* f32[] */
            fread(&property->f.length, sizeof(uint32_t), 1, file);
            fread(&property->f.encoding, sizeof(uint32_t), 1, file);
            fread(&property->f.compressed_length, sizeof(uint32_t), 1, file);
            if (property->f.encoding == 0) {
                fseek(file, sizeof(float) * property->f.length, SEEK_CUR);
            } else {
                fseek(file, property->f.compressed_length, SEEK_CUR);
            }
            break;
        }
        case 'd': { /* f64[] */
            fread(&property->d.length, sizeof(uint32_t), 1, file);
            fread(&property->d.encoding, sizeof(uint32_t), 1, file);
            fread(&property->d.compressed_length, sizeof(uint32_t), 1, file);
            if (property->d.encoding == 0) {
                fseek(file, sizeof(double ) * property->d.length, SEEK_CUR);
            } else {
                fseek(file, property->d.compressed_length, SEEK_CUR);
            }
            break;
        }
        case 'l': { /* i64[] */
            fread(&property->l.length, sizeof(uint32_t), 1, file);
            fread(&property->l.encoding, sizeof(uint32_t), 1, file);
            fread(&property->l.compressed_length, sizeof(uint32_t), 1, file);
            if (property->l.encoding == 0) {
                fseek(file, sizeof(int64_t) * property->l.length, SEEK_CUR);
            } else {
                fseek(file, property->l.compressed_length, SEEK_CUR);
            }
            break;
        }
        case 'i': { /* i32[] */
            fread(&property->i.length, sizeof(uint32_t), 1, file);
            fread(&property->i.encoding, sizeof(uint32_t), 1, file);
            fread(&property->i.compressed_length, sizeof(uint32_t), 1, file);
            if (property->i.encoding == 0) {
                fseek(file, sizeof(int32_t) * property->i.length, SEEK_CUR);
            } else {
                fseek(file, property->i.compressed_length, SEEK_CUR);
            }
            break;
        }
        case 'b': { /* u8[] (bool[]) */
            fread(&property->b.length, sizeof(uint32_t), 1, file);
            fread(&property->b.encoding, sizeof(uint32_t), 1, file);
            fread(&property->b.compressed_length, sizeof(uint32_t), 1, file);
            if (property->b.encoding == 0) {
                fseek(file, sizeof(bool) * property->b.length, SEEK_CUR);
            } else {
                fseek(file, property->b.compressed_length, SEEK_CUR);
            }
            break;
        }
        case 'S': { /* raw string */
            fread(&property->S.length, sizeof(uint32_t), 1, file);
            fseek(file, property->S.length, SEEK_CUR);
            break;
        }
        case 'R': { /* raw binary */
            fread(&property->R.length, sizeof(uint32_t), 1, file);
            fseek(file, property->R.length, SEEK_CUR);
            break;
        }
        default: {
            return false; // invalid property type
        }
    }

    return true;
}

/**
 * @brief Parses a single FBX node from an open .FBX file
 *
 * @param file Pointer to the open .FBX file
 * @param node Pointer to an FBXNode structure where the parsed details will be stored
 *
 * @return Returns true if the node is successfully parsed from the file
 *         Returns false if an error occurs during the reading
 */
bool FBX_ParseNode(FILE *file, FBXNode *node)
{
    // Read node record
    fread(&node->end_offset, sizeof(uint32_t), 1, file);
    if (ferror(file) != 0) {
        return false;
    }
    fread(&node->properties_count, sizeof(uint32_t), 1, file);
    if (ferror(file) != 0) {
        return false;
    }
    fread(&node->properties_list_length, sizeof(uint32_t), 1, file);
    if (ferror(file) != 0) {
        return false;
    }
    fread(&node->name_length, sizeof(uint8_t), 1, file);
    if (ferror(file) != 0) {
        return false; // failed to read a node
    }

    // Read node name
    fread(node->name, 1, node->name_length, file);
    if (ferror(file) != 0) {
        return false; // failed to read a node name
    }
    node->name[node->name_length] = '\0'; // end string with the '\0' character

    // Read node properties
    FBXNodeProperty property;
    for (int i = 0; i < node->properties_count; ++i) {
        if (!FBX_ParseNodeProperty(file, &property)) {
            return false;
        }
    }

    return true;
}

/**
 * Parses nodes list from an .FBX file
 *
 * @param file Pointer to the open .FBX file
 *
 * @return If successful, returns a pointer to an FBXNodeList containing the parsed nodes from the .FBX file
 *         If an error occurs or if there are no more nodes to parse, it returns NULL
 */
FBXNodeList *FBX_ParseNodes(FILE *file)
{
    // Create a node list
    FBXNodeList *list = FBXNodeList_Create(0);
    if (list == NULL) {
        return NULL;
    }

    // Parse nodes
    while (1) {
        FBXNode node;
        memset(&node, 0, sizeof(FBXNode));
        if (FBX_ParseNode(file, &node) == false) {
            break;
        }
        if (node.end_offset == 0) {
            break; // no more nodes
        }

        // If the current file position is not equal to the end offset of the node,
        // this means that the node has child subnodes
        if (ftell(file) != node.end_offset) {
            node.subnodes = FBX_ParseNodes(file); // parse subnodes recursively
        }

        //
        if (FBXNodeList_Insert(list, &node) == false) {
            break;
        }
    }

    // Shrink the array capacity to the actual length. This optimizes memory usage.
    FBXNodeList_Truncate(list);

    return list;
}

/**
 * Parses an .FBX file and returns a list of root nodes
 *
 * @param path Pointer to a null-terminated string specifying the path to the .FBX file
 *
 * @return If successful, returns a pointer to an FBXNodeList containing the root nodes of the parsed .FBX file
 *         If an error occurs at any step or if the file isn't a valid .FBX file, it returns NULL
 */
FBXNodeList *FBX_Parse(const char *path)
{
    FBXNodeList *nodes = NULL;

    // Open file handle
    FILE *file = NULL;
    if (fopen_s(&file, path, "rb") != 0) {
        goto FBX_PARSE_END;
    }

    // Check file signature
    char buffer[21];
    fread(buffer, 21, 1, file);
    if (ferror(file) != 0) {
        goto FBX_PARSE_END;
    }
    const char *FBX_SIGNATURE = "Kaydara FBX Binary  ";
    if (strncmp(buffer, FBX_SIGNATURE, 20) != 0) {
        goto FBX_PARSE_END;
    }

    // Skip to the beginning of the first node
    if (fseek(file, 27, SEEK_SET) != 0) {
        goto FBX_PARSE_END;
    }

    // Read root nodes
    nodes = FBX_ParseNodes(file);

    // Cleanup and exit
FBX_PARSE_END:
    if (file != NULL) {
        fclose(file);
    }

    return nodes;
}