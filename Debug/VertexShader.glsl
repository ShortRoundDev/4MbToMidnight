#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoord;
out vec3 dist;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 playerPos;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0f);
    TexCoord = vec2(aTex.x, 1.0 - aTex.y);
    dist = aPos - playerPos;
}