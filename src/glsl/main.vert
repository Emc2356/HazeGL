#version 450 core

layout(location=0) in vec2 iVert;
layout(location=1) in vec2 iTexCoords;

layout(location=0) out vec2 oTexCoords;

uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * vec4(iVert, 0.0, 1.0);

    oTexCoords = iTexCoords;
}