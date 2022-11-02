#version 430
//// TODO ////
//
// Hint:
// 1. Recieve texcoord and Normal from vertex shader
// 2. Calculate and return final color to opengl
//

// �bfragment shader�ۦ�
// �]����layout(binding=0)�|�ǩǪ��A�ҥH������uniform�ŧi
uniform sampler2D Texture;

// input�n��vertexShader��output�A�W�٭n�ۦP!(out->in)
in vec3 outNormal;
in vec2 outTexcoord;

// �̲׿�Xvertices�e�{���C��A frag_color�O�T�w���خ榡
out vec4  frag_color;

void main(){
 frag_color = texture2D(Texture, outTexcoord);
}
// texture2D(Texture, outTexcoord)�����X�ӴN�O�|��(2d + 2d)
