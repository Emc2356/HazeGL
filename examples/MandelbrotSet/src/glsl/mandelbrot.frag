#version 450 core

// the two coloring options
// #define BW
#define BLUE

layout(location=0) in vec2 iUv;

layout(location=0) out vec4 oColor;


// the window resolution
uniform vec2 uResolution;

// how many times we run through the formula
uniform uint uPrecision;
// a preset point that defines our "infinity" aka when the formula spins too far out of control
uniform float uInfinity;

// real axis (or x axis)
uniform float uReStart;
uniform float uReEnd;
// the imaginary axis (or y axis)
uniform float uImStart;
uniform float uImEnd;


// (a + ib) (c + id) = (ac - bd) + i(ad + bc).
vec2 mulComplexNumber(vec2 a, vec2 b) {
    return vec2(
        a.x*b.x - a.y*b.y,
        a.x*b.y + a.y*b.x
    );
}

uint mandelbrotAt(vec2 c) {
    vec2 z = vec2(0.0, 0.0);
    for (uint i = 0; i < uPrecision; i++) {
        // Zn+1 = Zn^2 + C
        z = mulComplexNumber(z, z) + c;
        
        if (length(z) > uInfinity) {
            return i;
        }
    }
    return uPrecision;
}

vec2 toComplex(float x, float y) {
    return vec2(
        uReStart + (x / uResolution.x) * (uReEnd - uReStart),
        uImStart + (y / uResolution.y) * (uImEnd - uImStart)
    );
}


void main() {
    vec2 complex = toComplex(iUv.x, iUv.y);
    uint iterations = mandelbrotAt(complex);

    if (iterations < uPrecision) {
#ifdef BLUE
        oColor = vec4(
            float(iterations + ((iterations * 20) % 156)) / 255.0, 
            float(100 + ((iterations * 20) % 156)) / 255.0,
            1.0,
            1.0
        );
#elif defined(BW)
        oColor = vec4(1.0, 1.0, 1.0, 1.0);
#else
#error invalid/unsupported color
#endif
    } else {
        oColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
