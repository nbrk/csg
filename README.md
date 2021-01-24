## C Scene Graph library
No-sweat, efficient, minimal dependency pure OpenGL C99 Scene Graph (csg) library.
Useful for quick visualizations and controls of data, simulations, and more, 
once all the stuff gets implemented. 

### Features
Right now implemented features include:
- **Concise, extremely easy-to-use API**
- 3D Affine transformations, cameras
- Multiple windows support with dead-simple controls
- Smooth sharing of geometries, materials, etc. between windows/contexts
- Embedded powerful UI (thanks to `Nuklear`), with no setup required
- Immediate and relative (cursor delta, up-down transition, etc.) input states available each frame
- Various graph traversal patterns: (DFS, BFS, path-to-root, etc.)

### Requirements
The core library depends only on `cglm` and GL/GLEW. The gui_glfw3 adapter 
depends on glfw3.

### NOTE
The library is in the process of active design and development. Only a fraction
of planned functionality is supported for now, but the code is already proved
to be useful in some scenarios.

![window sharing](https://github.com/nbrk/csg/blob/master/doc/1.png?raw=true)
![window sharing](https://github.com/nbrk/csg/blob/master/doc/2.png?raw=true)
