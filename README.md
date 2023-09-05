# Compact .FBX parser

The .FBX is a proprietary file format used to store 3D scene data in Autodesk software.
Usually, you need to include the fbx-sdk or [assimp](https://github.com/assimp/assimp) in your project to read *.fbx files. However, often you might just want something straightforward, and that's what this code provides.

Please note that this code is only for parsing nodes and properties. Reading and processing the scene and model data is up to you.

The research papers I used to write this project:
- https://code.blender.org/2013/08/fbx-binary-file-format-specification
- https://liangz0707.github.io/whoimi/blogs/3DBasic/FBXFileFormat.html
