#version 450 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 norm;
layout (location=2) in vec2 text;

uniform mat4 projectionMatrix;
uniform mat4 viewingMatrix;
uniform mat4 modelMatrix;

out vec3 N; out vec3 pos; out vec3 E; out vec2 textcoord;

void main() {
	pos = position;
	N=norm.xyz;
	E=vec3(viewingMatrix[0]);
	textcoord = vec2(text.x, text.y);
	vec4 transformedPosition = projectionMatrix*viewingMatrix*modelMatrix*vec4(position,1.0f);
    	gl_Position = transformedPosition;
}
