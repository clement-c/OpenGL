## Working with shaders

http://antongerdelan.net/opengl/shaders.html


### Shaders steps

1. `glCreateProgram` __Create the *program*__
2. **Add shaders** - For each shader:
   1. [`glCreateShader`](http://docs.gl/gl4/glCreateShader): **Create** shader, specifying its type
   2. [`glShaderSource`](http://docs.gl/gl4/glShaderSource): **Set the source** of the shader
   3. [`glCompileShader`](http://docs.gl/gl4/glCompileShader): **Compile** the shader
   4. `glGetShaderiv`: **Check status** of the shader (`GL_COMPILE_STATUS`, `GL_INFO_LOG`)
   5. [`glAttachShader`](http://docs.gl/gl4/glAttachShader): **Attach** shader to program
3. [`glLinkProgram`](http://docs.gl/gl4/glLinkProgram) **Link** the program
4. [`glValidateProgram`](http://docs.gl/gl4/glValidateProgram) **Validate** the program
5. [`glDeleteShader`](http://docs.gl/gl4/glDeleteShader) Delete the shaders
6. [`glUseProgram`](http://docs.gl/gl4/glUseProgram) Activate the program with the shaders
7. After all is said and done (on exit) `glDeleteProgam`.

### Check the status of the program

1. [`glGetShaderiv`](http://docs.gl/gl4/glGetShader) with `GL_COMPILE_STATUS` to see if the compiling errored (failed if `== GL_FALSE`)
1. [`glGetShaderiv`](http://docs.gl/gl4/glGetShader) with `GL_INFO_LOG_LENGTH` to get the length of the message
3. `char* msg = new[msgLength]` - !!!! DO NOT FORGET `delete[] msg`
4. Use [`glGetShaderInfoLog`](http://docs.gl/gl4/glGetShaderInfoLog) to retrieve the message


### What's in the shader

`uniform` -> comes from the CPU, set with [`glUniformXXX`](http://docs.gl/gl4/glUniform) functions.

### Always there:

```c
#version 330 core
```

First line, we require OpenGL 3.30, and the code is using core profile (no legacy).

```c
void main() {}
```

### Getting attributes

Get the position from the attribPointer of index 0:
```c
layout(location = 0) in vec4 position; // even though we pass vec2, automatically parse
```

### Inputs and outputs

You can either retrieve attributes using their layout location (`layout(location = 0)` for example).

Or you can directly bind to variable names in the C++ `glBindAttribLocation(program, 0, "position"); `

But, this must be done: **AFTER COMPILING**, **BEFORE LINKING**

**Vertex shader**:

- outputs:
  - `gl_Position`: position in normalized screen coordinates - this needs t be the **exact** name.

**Fragment shader**:

- outputs:
  - `color` - apparently, any output of type vec4 will be interpreted as color, no matter the name.

For custom attributes passed from vertex to frag, no need to specify layout
```c
// VERT:
out vec4 vertColour;

// FRAG
in vec4 vertColour;
```