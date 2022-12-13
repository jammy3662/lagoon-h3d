#version 330

in vec4 fragPos;
in vec2 fragCoord;
in vec2 fragCoord2;
in vec3 fragNormal;
in vec3 fragTangent;
in vec4 fragColor;

out vec4 finalColor;

// clipping planes
uniform float near = 0.01;
uniform float far = 100;

void main()
{
	float depth;
	
	//depth = (gl_FragCoord.z - near) / (far - near);
	depth = gl_FragCoord.z;
	
	finalColor = vec4(depth, depth, depth, 1);
}