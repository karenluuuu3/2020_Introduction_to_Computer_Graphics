#version 430

uniform sampler2D texture;
uniform vec3 WorldLightPos, Kd;

in vec2 uv;
in vec3 normal;
in vec3 worldPos;
out vec4 frag_color;

vec3 lightDir = normalize(WorldLightPos - worldPos);// 光線要指向光源
vec3 norm = normalize(normal);
float level = dot(lightDir, norm);
float intensity;

void main(){
    vec4 color = texture2D(texture, uv);

    // Decide a level by calculating the included angles between light and normal vectors
    if( level > 0.95 )  intensity = 1;
    else if( level > 0.75 ) intensity = 0.8;
    else if( level > 0.50 )  intensity = 0.6;
    else if( level > 0.25 ) intensity = 0.4;
    else    intensity = 0.2;

    frag_color = vec4(Kd, 1.0) * intensity *color;
} 

