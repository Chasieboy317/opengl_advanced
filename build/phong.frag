#version 450 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lpos;

out vec4 outColor;

in vec3 N; in vec3 L; in vec3 E;

void main() {
	vec3 NN = normalize(N);
	vec3 EE = normalize(E);
	vec3 LL = normalize(L);

	float ambprod = 0.1f;
	float specprod = 0.7f;

	vec3 amb, diff, spec;

	vec3 H = normalize(LL+EE);

	float kd = max(dot(NN,LL), 0.0f);
	float ks = pow(max(dot(NN,H), 0.0f), 32);

	amb = ambprod * lightColor;
	diff = kd*lightColor;
	spec = ks*specprod*lightColor;

	outColor = vec4((amb+diff+spec).rgb * objectColor, 1.0f);
}
