#version 330 core

out vec4 FragColor;
in vec2 fragCoord;

uniform vec2 iResolution;
uniform float iTime;

// Função de paleta (https://iquilezles.org/articles/palettes/)
vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

// Função para aplicar rotação nas coordenadas UV
vec2 rotate(vec2 uv, float angle) {
    mat2 rotation = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    return rotation * uv;
}

void main() {
    // Coordenadas normalizadas de pixel com o centro da tela em (0,0)
    vec2 uv = (fragCoord * iResolution) - vec2(1.0, 1.0);  // Corrige o centro da tela

    // Ajustar as coordenadas UV
    uv = (uv * 2.0 - iResolution) / iResolution.y;

    uv = rotate(uv, iTime);

    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i * 0.4 + iTime * 0.4);

        d = sin(d * 8.0 + iTime) / 8.0;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }
    
    FragColor = vec4(finalColor, 1.0);
}
