#version 450 
#define number_lights 3

uniform vec3 lightColours[number_lights];
uniform vec3 lightPositions[number_lights];
uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lpos;
out vec4 outColor;
in vec3 N; in vec3 pos; in vec3 E;

vec4 calcLight(vec3 L, vec3 N, vec3 E, vec3 lpos, vec3 pos, vec3 objectColor, vec3 lightColor);

void main() {
	vec4 result = calcLight(vec3(0.0f), N, E, lightPositions[0], pos, objectColor, lightColours[0]);
	for (int i=1; i<number_lights; i++) {
		result+=calcLight(vec3(0.0f), N, E, lightPositions[i], pos, objectColor, lightColours[i]);	
	}
	outColor=result;
}

vec4 calcLight(vec3 L, vec3 N, vec3 E, vec3 lpos, vec3 pos, vec3 objectColor, vec3 lightColor) {
	L=(lpos-pos);
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

	return vec4((amb+diff+spec).rgb * objectColor, 1.0f);
}
