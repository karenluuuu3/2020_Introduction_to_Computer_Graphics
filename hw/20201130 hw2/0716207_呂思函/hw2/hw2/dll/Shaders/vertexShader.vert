#version 430

//// TODO: ////
//
// Hint:
// 1. Receive position, normal, texcoord from bind buffer
// 2. Receive Model matrix, View matrix, and Projection matrix from uniform
// 3. Pass texcoord and Normal to fragment shader (normal will not use in this homework)
// 4. Calculate view space by gl_Position (must be vec4)
//

// 在vertex shader先畫好圖形的點，每個點會收到不同的資料
layout(location = 0) in vec3 position;// the position variable has attribute position 0
layout(location = 1) in vec3 normal;// the  normal variable has attribute position 1
layout(location = 2) in vec2 texcoord;// the  normal variable has attribute position 2

// uniform : 不會去動到的東西、不會被兩個shader共用的參數(ex. position or color)；類似global用法
uniform mat4 M;
uniform mat4 Projection;
uniform mat4 ModelView;

// set output，要傳給fragment shader的
out vec3 outNormal;
out vec2 outTexcoord;

void main(){
gl_Position = Projection * ModelView * M * vec4(position, 1.0);// 轉成四維輸出(因為要做矩陣乘法)
outNormal = normal;
outTexcoord = texcoord;
}
// 最後輸出的vertices一定要用gl_Position接(內建固定用法)
// 乘上model matrix(M)，伊布就會長好長在自己的位置上
// normal和texcoord要接到(in->out)
// 我哩個傻逼這個問題找了一整天ˋˊ

	


