# Game Engines Assigments
A few programming assignments that I developed during my Game Engines class.
I chose some of the interesting pieces of code on the list below. You can also see a demo video clicking [here](https://www.youtube.com/watch?v=rAw8Tdoq0Gw&feature=youtu.be)

**[SIMD Math Library](https://github.com/caioteixeira/GameEngines/blob/master/lab5/Source/SimdMath.h):** 
This class implements a vector math library using SSE SIMD intrinsics. 

**[Level Loader](https://github.com/caioteixeira/GameEngines/blob/master/lab5/Source/LevelLoader.cpp):** 
The class that loads level files written using JSON data format. 

**[Renderer class](https://github.com/caioteixeira/GameEngines/blob/master/lab5/Source/Renderer.cpp):** 
The class that handles all the rendering logic, it actually uses the [GraphicsDriver](https://github.com/caioteixeira/GameEngines/blob/master/lab5/Source/GraphicsDriver.cpp)
class as an abstraction layer for DirectX 11. 

**[Skinned mesh shader](https://github.com/caioteixeira/GameEngines/blob/master/lab5/Assets/Shaders/Skinned.hlsl):** 
The shader that renders skinned meshes. You can also look the [Animation](https://github.com/caioteixeira/GameEngines/blob/master/lab5/Source/Animation.cpp)
class to see how the global pose is calculated and then look the [SkeletalMeshComponent](https://github.com/caioteixeira/GameEngines/blob/master/lab5/Source/SkeletalMeshComponent.cpp) class 
to see how the Matrix Pallete is defined. 

**[Phong shader](https://github.com/caioteixeira/GameEngines/blob/master/lab5/Assets/Shaders/Phong.hlsl):** 
The shader that renders meshes using the Phong Lighting model. 


[![PrintScreen](https://raw.githubusercontent.com/caioteixeira/GameEngines/master/images/demo.png)](https://www.youtube.com/watch?v=rAw8Tdoq0Gw&feature=youtu.be)

## How to Compile
Just import the project on Visual Studio 2013 or XCode and compile it. :)
