#version 330 core

// ─── Input from vertex shader ───
in vec3 vColor;

// ─── Output ───
out vec4 FragColor;

void main() {
    FragColor = vec4(vColor, 1.0);
}
