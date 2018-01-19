

## OpenGL pipeline

```
   |----------------------|
   | Vertex specification |
   |----------------------|
          |
          |
   |----------------------|
   | Vertex Shader        |
   |----------------------| 
          |
          |
   |----------------------|
   | Tesselation          |
   |----------------------|
          |
          |
   |----------------------|
   | Geometry Shader      |
   |----------------------|
          |
          |
   |------------------------|
   | Vertex Post-processing |
   |------------------------|
```

## OpenGL datatypes

- GL_BYTE
- GL_UNSIGNED_BYTE
- GL_SHORT
- GL_UNSIGNED_SHORT
- GL_INT
- GL_FLOAT



## Immediate mode - depricated

```
GLuint list = glGenLists(1);
glNewList()

``


@question Ask about this
```cpp
  const GLfloat dice[points][floatsPerPoint] = {
    { -0.5,  0.5,  0.5 }, // Top left
    {  0.5,  0.5,  0.5 }, // Top right
    {  0.5, -0.5,  0.5 }, // Bottom right 
    { -0.5, -0.5,  0.5 }, // Bottom left
  };
```


The GTL-youtube channel has a race-condition, featuring multiple 
  threads writing to a globally shared variable.





GLuint vertexbuffer;
// Generate one buffer, put the resulting identifier in vertexbuffer
glGenBuffers(1, &vertexbuffer);
// Allocate space and upload the data from CPU to GPU
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
// Give our vertices to OpenGL.
glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


@question - pull requests to the