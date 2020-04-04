## HelloModernWorld

Shaders, Array buffers, vertex buffers

Buffers are flat arrays of data.

**Vertex Buffer Objects** (VBOs) are Buffers that describe vertices in a sequential manner.
Each vertex has its attributes (position,. UV coords, etc) stored flat in long list common
for all vertices.

If for example vertices have a position `P`, a color `C` and a normal `N`, then a VBO for
3 vertices could be a flat array like so:

```
{
	P0.x, P0.y, P0.z, C0.r, C0.g, C0.b, C0.a, N0.x, N0.y, N0.z,
	P1.x, P1.y, P1.z, C1.r, C1.g, C1.b, C1.a, N1.x, N1.y, N1.z,
	P2.x, P2.y, P2.z, C2.r, C2.g, C2.b, C2.a, N2.x, N2.y, N2.z,
}
```

Then we use VertexAttribPointers in order to specify which attribute corresponds to which
parts of the buffer.

**Element Buffer Objects** (EBOs) are buffers as well. The flat list of indices is used to represent
connectivity (which vert indices compose a triangle).

**Vertex Array Objects** (VAOs) are sort of a parent for buffers (binding to a VAO basically binds
its children, and that's it).



### ArrayBuffers

Standard buffer that can be bound and drawn using `glDrawArrays` and `glDrawElements` (after binding).

Steps:

1. Generate `glGenBuffers`
2. Bind`glBindBuffer` (type and id)
3. Describe `glBufferData` (type, size, pointer if available, usage - static or dynamic)

Then use in the future after re-binding.

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

Types of buffers:

| Enum value | Meaning |
|:---|:---|
|**GL_ARRAY_BUFFER** | Vertex attributes|
|GL_ATOMIC_COUNTER_BUFFER | Atomic counter storage
|GL_COPY_READ_BUFFER | Buffer copy source
|GL_COPY_WRITE_BUFFER | Buffer copy destination
|GL_DISPATCH_INDIRECT_BUFFER | Indirect compute dispatch commands
|GL_DRAW_INDIRECT_BUFFER | Indirect command arguments
|GL_ELEMENT_ARRAY_BUFFER | Vertex array indices
|GL_PIXEL_PACK_BUFFER | Pixel read target
|GL_PIXEL_UNPACK_BUFFER | Texture data source
|GL_QUERY_BUFFER | Query result buffer
|GL_SHADER_STORAGE_BUFFER | Read-write storage for shaders
|GL_TEXTURE_BUFFER | Texture data buffer
|GL_TRANSFORM_FEEDBACK_BUFFER | Transform feedback buffer
|GL_UNIFORM_BUFFER | Uniform block storage

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


### Vertex Array Object (VAOs)

If a VAO is a "parent" for multiple attributes pointers. Binding a VAO restores the bindings 
and activation of its attrs.

Steps:

1. Create vertex buffers
2. [`glGenVertexArrays`](http://docs.gl/gl4/glGenVertexArrays) Create the VAO(s)
3. [`glBindVertexArray`](http://docs.gl/gl4/glBindVertexArray) Binds to the VAO
4. For each "child" buffer (created in `1`, or now)
   - [`glBindBuffer`](http://docs.gl/gl4/glBindBuffer) Bind the buffer
   - [`glVertexAttribPointer`](http://docs.gl/gl4/glVertexAttribPointer) Define attributes
   - [`glEnableVertexAttribArray`](http://docs.gl/gl4/glEnableVertexAttribArray) ENABLE attributes

Then, we can bind a specific VAO when needed.
