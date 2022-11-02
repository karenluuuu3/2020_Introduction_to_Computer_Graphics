#version 430

uniform sampler2D texture;
uniform vec3 WorldLightPos, WorldCamPos;
uniform vec3 Ka, La, Kd, Ld, Ks, Ls, gloss;


in vec2 uv;
in vec3 normal;
in vec3 worldPos;
out vec4 frag_color;

vec3 lightDir = normalize(WorldLightPos - worldPos);// 光線要指向光源
vec3 viewDir = normalize(WorldCamPos - worldPos);
vec3 norm = normalize(normal);
vec3 R = normalize(reflect(-lightDir, norm));// 反射角

void main(){
    vec4 color = texture2D(texture, uv);
  
    float ambient = La[0] * Ka[0];
    float diffuse = Ld[0] * Kd[0] * dot(lightDir, norm);// must > 0
    float specular = Ld[0] * Ks[0] * pow(dot(viewDir, R), gloss[0]);

    frag_color = (ambient + diffuse + specular) * color;
} 



