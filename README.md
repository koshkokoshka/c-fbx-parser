# Compact .FBX parser

The .FBX is a proprietary file format used to store 3D scene data in Autodesk software.
Usually, you need to include the fbx-sdk or [assimp](https://github.com/assimp/assimp) library in your project to read *.fbx files. However, often you might just want something straightforward, and that's what this code provides.

Please note that this code is only for parsing nodes and properties. Processing the scene data is up to you.

Referenced research papers for this project:
- https://code.blender.org/2013/08/fbx-binary-file-format-specification
- https://liangz0707.github.io/whoimi/blogs/3DBasic/FBXFileFormat.html
