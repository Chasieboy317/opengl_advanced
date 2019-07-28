#version 450 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 norm;

uniform mat4 projectionMatrix;
uniform mat4 viewingMatrix;
uniform mat4 modelMatrix;

uniform vec3 lpos;
out vec3 N; out vec3 L; out vec3 E;

void main() {
	N=norm.xyz;
	L=lpos.xyz-position.xyz;
	E=-position.xyz;
	vec4 transformedPosition = projectionMatrix*viewingMatrix*modelMatrix*vec4(position,1.0f);
    	gl_Position = transformedPosition;
}
