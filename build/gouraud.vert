#version 450 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 norm;

out vec4 color;

uniform mat4 projectionMatrix;
uniform mat4 viewingMatrix;
uniform mat4 modelMatrix;

uniform vec3 lpos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
	vec3 amb, diff, spec;

	vec3 N = normalize(norm.xyz);
	vec3 L = normalize(lpos.xyz-position.xyz);
	vec3 E = -normalize(position.xyz);
	vec3 H = normalize(L+E);
	vec4 transformedPosition = projectionMatrix*viewingMatrix*modelMatrix*vec4(position,1.0f);
    	gl_Position = transformedPosition;

	float shine = 1.0f;
	float ambprod = 0.5f;
	float diffprod = 0.6f;
	float specprod = 0.7f;

	float kd = max(dot(L,N),0.0f);
	float ks = pow(max(dot(N,H), 0.0f), shine);

	amb=ambprod*lightColor;
	diff=kd*diffprod*lightColor;
	spec=ks*specprod*lightColor;
	color = vec4((amb+diff+spec).rgb*objectColor, 1.0f);
}
