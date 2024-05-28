#version 450 core

layout(location=0) in vec2 iUv;

layout(location=0) out vec4 oColor;

uniform sampler2D uTexture;


layout(std430, binding=0) buffer SSBO {
    vec3 data;
};

void main() {
    vec3 color = texture(uTexture, iUv).rgb;

    oColor = vec4(
        color.r * data.r,
        color.g * data.g,
        color.b * data.b,
        1.0
    );
}