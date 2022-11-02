#version 430

uniform sampler2D texture;
uniform vec3 WorldCamPos;

in vec2 uv;
in vec3 normal;
in vec3 worldPos;
out vec4 frag_color;

vec3 viewDir = normalize(WorldCamPos - worldPos);
vec3 norm = normalize(normal);
vec4 color;
vec4 outline = vec4(0.0, 1.0, 1.0, 1.0);

void main(){
    float cos = dot(viewDir, norm) / (length(viewDir) * length(norm));

    if( cos<0.01 && cos>0 ) color = outline * (1 - cos);
    else    color = vec4(0.0, 0.0, 0.0, 0.0);

    frag_color =  color;
} 





