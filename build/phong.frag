#version 450 core

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 outColor;

in vec3 N; in vec3 L; in vec3 E;

void main() {
	vec3 NN = normalize(N);
	vec3 EE = normalize(E);
	vec3 LL = normalize(L);

	float ambprod = 0.2f;
	float specprod = 0.0f;

	vec3 amb, diff, spec;

	vec3 H = normalize(LL+EE);

	float kd = max(dot(L,N), 0.0f);
	float ks = pow(max(dot(N,H), 0.0f), 128);

	amb = ambprod * lightColor;
	diff = kd*lightColor;
	spec = ks*specprod*lightColor;

	outColor = vec4((amb+diff+spec) * objectColor, 1.0f);
}
