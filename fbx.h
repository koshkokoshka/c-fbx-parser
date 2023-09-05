#ifndef FBX_H
#define FBX_H

#include <stdbool.h>
#include <stdint.h>

#define FBX_NODE_NAME_MAX 0xFF

struct FBXNode;
struct FBXNodeProperty;
struct FBXNodeList;

typedef struct FBXNode FBXNode;
typedef struct FBXNodeProperty FBXNodeProperty;
typedef struct FBXNodeList FBXNodeList;

struct FBXNodeList {
    size_t length;
    size_t capacity;
    struct FBXNode *nodes;
};

struct FBXNode {
    uint32_t end_offset;
    uint32_t properties_count;
    uint32_t properties_list_length;
    uint8_t name_length;
    char name[FBX_NODE_NAME_MAX];
    struct FBXNodeList *subnodes;
};

struct FBXNodeProperty {
    uint8_t type;
    union {
        struct { int16_t value; } Y; // i16
        struct { bool value; } C; // u8
        struct { int32_t value; } I; // i32
        struct { float value; } F; // f32
        struct { double value; } D; // f64
        struct { int64_t value; } L; // i64
        struct { int length; int encoding; int compressed_length; float *value; } f; // f32[]
        struct { int length; int encoding; int compressed_length; double *value; } d; // f64[]
        struct { int length; int encoding; int compressed_length; int64_t *value; } l; // i64[]
        struct { int length; int encoding; int compressed_length; int32_t *value; } i; // i32[]
        struct { int length; int encoding; int compressed_length; bool *value; } b; // u8[]
        struct { int length; char *value; } S; // string
        struct { int length; uint8_t *value; } R; // raw
        void *value;
    };
};

/**
 * Parses an .FBX file and returns a list of root nodes
 *
 * @param path Pointer to a null-terminated string specifying the path to the .FBX file
 *
 * @return If successful, returns a pointer to an `FBXNodeList` containing the root nodes of the parsed .FBX file.
 *         If an error occurs at any step or if the file isn't a valid .FBX file, it returns NULL.
 */
FBXNodeList *FBX_Parse(const char *path);

#endif