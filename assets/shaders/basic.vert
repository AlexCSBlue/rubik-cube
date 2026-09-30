#version 330 core

// ─── Vertex attributes (from VBO) ───
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// ─── Uniforms (from CPU) ───
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

// ─── Output to fragment shader ───
out vec3 vColor;

void main() {
    // Transform vertex position to clip space
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

    // Pass color through to fragment shader
    vColor = aColor;
}
