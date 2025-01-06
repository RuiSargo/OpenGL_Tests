#version 330 core

out vec4 FragColor;
in vec2 fragCoord;

uniform vec2 iResolution;
uniform float iTime;

// Função de paleta (https://iquilezles.org/articles/palettes/)
vec3 palette(float t) {
    /*
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    */

    vec3 a = vec3(0.448, 0.948, 0.498);
    vec3 b = vec3(0.198, 0.078, -0.362);
    vec3 c = vec3(1.608, 0.998, 0.998);
    vec3 d = vec3(0.698, 1.000, 0.000);

    return a + b * cos(6.28318 * (c * t + d));
}

// Função para aplicar rotação nas coordenadas UV
vec2 rotate(vec2 uv, float angle) {
    mat2 rotation = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    return rotation * uv;
}

void main() {
    // Coordenadas normalizadas de pixel com o centro da tela em (0,0)
    vec2 uv = fragCoord * iResolution - vec2(1.0);  // From 0 -> 1 to 0 to resolution size
    // Ajustar as coordenadas UV
    uv = (uv * 2.0 - iResolution) / iResolution.y;

    //uv = rotate(uv, iTime*4.0);

    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    float d = length(uv);
    d = 0.5+cos(d*2*3.14*2.0-iTime)*0.5;
    //d = pow(0.1/d,1.2);
    finalColor = vec3(0.0,d,d);

    //if (pow(pow(uv0.x-sin(iTime*3.0)*(1.0-0.2),2.0)+pow(uv0.y-cos(iTime*3.0)*(1.0-0.2),2.0), 0.5) < 0.1) 
    if (length( vec2( uv.x+sin(iTime*3.0*d)*(1.0-0.2), uv.y+cos(iTime*3.0*d)*(1.0-0.2) ) ) < 0.7 + 0.3*sin(iTime))
        finalColor = palette(iTime*0.3);

    FragColor = vec4(finalColor, 1.0);
    //FragColor = vec4(uv, 0.0, 1.0);
}
