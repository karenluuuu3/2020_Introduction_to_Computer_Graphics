#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 texcoord;


uniform mat4 M, V, P;


out vec2 uv;
out vec3 normal;
out vec3 worldPos;


void main() {
  gl_Position = P * V * M * vec4(in_position, 1.0);// 設定model位置
  normal =  mat3(transpose(inverse(M))) * in_normal;// 計算法向量經過模型變換後值
  uv = texcoord;   
  worldPos = vec3(V * vec4(in_position, 1.0));// 在世界座標指定，傳座標給fragment shader
}


