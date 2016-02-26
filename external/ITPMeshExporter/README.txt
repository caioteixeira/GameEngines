Simple plugin to export Static Meshes, Skeletal Meshes/Skeletons, and
Animation sequences to ITP formats

To use this plugin, follow these steps:
1. In the C++ Unreal 4 project directory, create a "Plugins" directory
2. Place the "ITPMeshExporter" directory inside "Plugins"
3. Right click on your .uproject and regenerate the project files
4. Now in the editor, you can right click on a static mesh and select
   "Asset Actions>Export...". From the file dropdown, select
   .itpmesh2, and you should be set.

A Note on Mesh Orientation:
The plug-in does not fix-up or transform the asset in any way. You
should make sure your asset is facing down the X-axis with Z-up
before you export it.

A Note on Textures:
Right now, the exporter will select the FIRST texture associated with
the "BaseColor" of the FIRST material assigned to the mesh. Multiple
materials/textures will be ignored.

Also, it assumes that you will follow the convention where the texture
file is in Textures/NameOfTexture. So when it exports, you'll get both
an .itpmesh2 and a .bmp file in the same directory, but you need
to place the .itpmesh2 in Assets/Meshes and the .bmp in Assets/Textures.

A Note on Shaders:
By default, static meshes will be exported and refer to the "BasicMesh"
shader. If you want to change this, for now you have to just manually edit
the .itpmesh2 file to change the shader. Similarly, SkeletalMeshes will
export referring to the "Skinned" shader.

A Note on Skeletal Meshes/Skeletons and Animations:
Skeletal Meshes will export their textures as static meshes do, and will
additionally export a .itpskel file in the same directory as the skeletal
mesh exports.

For animations, it is assumed that there is no scale applied -- bones
only export their rotation and translation.
