#version 430

//// TODO: ////
//
// Hint:
// 1. Receive position, normal, texcoord from bind buffer
// 2. Receive Model matrix, View matrix, and Projection matrix from uniform
// 3. Pass texcoord and Normal to fragment shader (normal will not use in this homework)
// 4. Calculate view space by gl_Position (must be vec4)
//

// �bvertex shader���e�n�ϧΪ��I�A�C���I�|���줣�P�����
layout(location = 0) in vec3 position;// the position variable has attribute position 0
layout(location = 1) in vec3 normal;// the  normal variable has attribute position 1
layout(location = 2) in vec2 texcoord;// the  normal variable has attribute position 2

// uniform : ���|�h�ʨ쪺�F��B���|�Q���shader�@�Ϊ��Ѽ�(ex. position or color)�F����global�Ϊk
uniform mat4 M;
uniform mat4 Projection;
uniform mat4 ModelView;

// set output�A�n�ǵ�fragment shader��
out vec3 outNormal;
out vec2 outTexcoord;

void main(){
gl_Position = Projection * ModelView * M * vec4(position, 1.0);// �ন�|����X(�]���n���x�}���k)
outNormal = normal;
outTexcoord = texcoord;
}
// �̫��X��vertices�@�w�n��gl_Position��(���ةT�w�Ϊk)
// ���Wmodel matrix(M)�A�쥬�N�|���n���b�ۤv����m�W
// normal�Mtexcoord�n����(in->out)
// �ڭ��Ӷ̹G�o�Ӱ��D��F�@��ѣ���

	


