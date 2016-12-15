#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 camera;
uniform mat4 modelMat;

out vec3 fragVert;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main(){
	fragVert = position;
	fragNormal = normal;
	fragTexCoord = texCoord;
	gl_Position = camera * modelMat * vec4(position, 1.0);
}