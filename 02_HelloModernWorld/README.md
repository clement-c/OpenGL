## HelloModernWorld

Shaders, Array buffers, vertex buffers


Buffers need to be generated with `glGenBuffers`

### ArrayBuffers

Standard buffer that can be bound and drawn using `glDrawArrays` and `glDrawElements` (after binding).

Steps:

1. Generate `glGenBuffers`
2. Bind`glBindBuffer` (type and id)
3. Describe `glBufferData` (type, size, pointer if available, usage - static or dynamic)

```cpp
Triangle triangle {
	{-0.5f, -0.5f},
	{0.0f, 0.5f},
	{0.5f, -0.5f}
};

// Generate buffer
uint32_t triBuffer;

// Generate a new buffer
glGenBuffers(1, &triBuffer);

// Bind the buffer in order to modify it
glBindBuffer(GL_ARRAY_BUFFER, triBuffer);

// Declare the data layout, and upload it at the same time
glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), &triangle, GL_STATIC_DRAW);
```


### VertexBuffers

All the data (= **attributes**) for each vertex one after the other. 
Each attribute has an **index** which is to be used in the shaders.

Attribute 0 is the position by default.

OpenGL needs an idea of the Layout of our data, so we describe an 
attribute using `glArrayAttribPointer`.

`glArrayAttribPointer` can be confusing.

> !!! `glArrayAttribPointer` is disabled by default, all attribPointers 
> need to be ENABLED before configuring them using `glEnableVertexAttribArray`

If a vertex is:
```cpp
struct Vec3 { float x, y, z; } // 3 * 4 bytes = 12 bytes
struct Vec2 { float u, v } // 2 * 4 bytes = 8 bytes

struct Vertex {
	Vec3 position;
	Vec2 textCoord;
}; // 12 + 8 = 24 bytes

Vertex vertices[NUM_VERTS] = {...};
```

Then we have 2 attributes to describe to OpenGL: `position` and `textCoord`.

```cpp
// First enable the vertex array ptr (glEnableVertexAttribArray(index))
void glVertexAttribPointer(
  	GLuint index,		// index in the shader
  	GLint size,		// number of values for an attribute (1, 2, 3, 4)
  	GLenum type,		// GL_FLOAT
  	GLboolean normalized,	// if int for example, 255->1.0f
  	GLsizei stride,
  	const GLvoid * pointer);
```

The argument `stride` is the **size of all the data for 1 vertex** (`= 24 bytes` in 
our case)

The last argument, `pointer` is a pointer **relative to the start of the vertex data** 
to the attribute. So for `position`, it's `(void*)0`, for `textCoord`, it's `(void*)12`;
