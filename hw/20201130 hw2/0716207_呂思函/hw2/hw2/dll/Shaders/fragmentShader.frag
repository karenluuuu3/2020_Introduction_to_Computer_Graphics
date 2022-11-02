#version 430
//// TODO ////
//
// Hint:
// 1. Recieve texcoord and Normal from vertex shader
// 2. Calculate and return final color to opengl
//

// 在fragment shader著色
// 因為用layout(binding=0)會怪怪的，所以直接用uniform宣告
uniform sampler2D Texture;

// input要接vertexShader的output，名稱要相同!(out->in)
in vec3 outNormal;
in vec2 outTexcoord;

// 最終輸出vertices呈現的顏色， frag_color是固定內建格式
out vec4  frag_color;

void main(){
 frag_color = texture2D(Texture, outTexcoord);
}
// texture2D(Texture, outTexcoord)直接出來就是四維(2d + 2d)
