#version 330 core

layout(location = 0) in vec3 aPos;
out vec2 fragCoord;

void main()
{
    // Transformação de posição do vértice
    gl_Position = vec4(aPos, 1.0);

    // Convertendo a posição do vértice para coordenadas de fragmento
    fragCoord = aPos.xy * 0.5 + 0.5;
}
