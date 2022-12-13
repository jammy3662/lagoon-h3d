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
uniform float far = 1000;

void main()
{
	float depth;
	
	float z = (gl_FragCoord.z * 2.0 - 1.0) / gl_FragCoord.w;
	depth = (z + near) / (near + far);
	
	finalColor = vec4(depth, depth, depth, 1);
}