#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragColor; // Color to pass to the fragment shader

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    fragColor = vec3(1.0, 0.0, 0.0); // Set the color to red
}
