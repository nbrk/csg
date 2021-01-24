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
- Draw, transforms, etc. may be used straight-away inside any OpenGL context (no need of setup, even the `csg_gui` adapters are fully optional).
- Various graph traversal patterns: (DFS, BFS, path-to-root, etc.)

### Requirements
The core library depends only on `cglm` and GL/GLEW. The gui_glfw3 adapter 
depends on glfw3.

### Example
This is how a 'input-update-render' loop may look like with the API:

``` c
  csg_gui_adapter_t adapter = csg_gui_adapter_create(
      csg_gui_glfw3_adapter_ops(), 0, 0, 1024, 768, 0, NULL);

  // nodes
  csg_node_t* root = csg_node_create(NULL, NULL);
  for (size_t i = 0; i <= 16; i += 2) {
    csg_node_t* node = csg_node_create(root, NULL);
    node->transform.translation.x = i - 8;
    node->geometry = csg_geometry_create_cube();
    node->geometry->material = csg_material_create();
    node->geometry->material.diffuse_color = (csg_vec4_t){
        1.0f / (rand() % 10), 1.f / (rand() % 10), 1.f / (rand() % 10), 1.0f};
  }

  csg_camera_t camera = csg_camera_default();

  while ((adapter.flags & CSG_GUI_FLAG_WANT_CLOSE) == 0) {
    // INPUTS
    csg_gui_adapter_update(&adapter);

    if (adapter.keyboard[CSG_GUI_KEY_ESCAPE] == CSG_GUI_PRESS)
      adapter.flags |= CSG_GUI_FLAG_WANT_CLOSE;

    if (adapter.keyboard[CSG_GUI_KEY_W] == CSG_GUI_PRESS) {
      camera.position.z--;
      camera.target.z--;
    }
    if (adapter.keyboard[CSG_GUI_KEY_S] == CSG_GUI_PRESS) {
      camera.position.z++;
      camera.target.z++;
    }

    if (adapter.mouse[CSG_GUI_MOUSE_BUTTON_RIGHT]) {
      if (adapter.mouse_deltax != 0)
        camera.target.x += 0.1f * adapter.mouse_deltax;
      if (adapter.mouse_deltay != 0)
        camera.target.y -= 0.1f * adapter.mouse_deltay;
    }
    if (adapter.mouse[CSG_GUI_MOUSE_BUTTON_LEFT]) {
      if (adapter.mouse_deltax != 0)
        root->transform.rotation.y += 0.01f * adapter.mouse_deltax;
      if (adapter.mouse_deltay != 0)
        root->transform.rotation.x += 0.01f * adapter.mouse_deltay;
    }

    // RENDER
    csg_gui_adapter_begin_frame(&adapter);
    {
      // scene
      csg_render(root, camera, (csg_vec4_t){0.2f, 0.2f, 0.3f, 1.0f});
      
      // ui
      if (nk_begin(adapter.nk, "Model transform", nk_rect(0, 0, 320, 240),
                   NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE |
                       NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                       NK_WINDOW_MINIMIZABLE)) {
        nk_layout_row_dynamic(adapter2.nk, 25, 1);
        nk_property_float(adapter.nk, "Rotation along X", -2.0f * M_PI,
                          &root->transform.rotation.x, 2.0f * M_PI, 0.10f,
                          0.01f);
        nk_property_float(adapter.nk, "Rotation along Y", -2.0f * M_PI,
                          &root->transform.rotation.y, 2.0f * M_PI, 0.10f,
                          0.01f);
        nk_property_float(adapter.nk, "Rotation along Z", -2.0f * M_PI,
                          &root->transform.rotation.z, 2.0f * M_PI, 0.10f,
                          0.01f);
      }
      nk_end(adapter.nk);
    }
    csg_gui_adapter_end_frame(&adapter);
  }

```

### NOTE
The library is in the process of active design and development. Only a fraction
of planned functionality is supported for now, but the code is already proved
to be useful in some scenarios.

![window sharing](https://github.com/nbrk/csg/blob/master/doc/1.png?raw=true)
![window sharing](https://github.com/nbrk/csg/blob/master/doc/2.png?raw=true)
