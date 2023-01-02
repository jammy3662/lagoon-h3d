#version 330

/*
Render fluid particles as
spheres using quad point-sprites
*/

in vec4 fragPos;
in vec2 fragCoord;
in vec4 fragColor;

uniform mat4 mvp;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;
uniform mat4 matNormal;

out vec4 finalColor;

uniform float colDiffuse;
uniform float sphereRadius = 0.1;

void main()
{
	vec3 normal;
	normal.xy = fragCoord * 2.0 - 1.0;
	float radius = dot(normal.xy, normal.xy); // square of the length
	normal.z = -sqrt(1.0 - radius);
	
	if (radius > 1.0)
	{
		finalColor = vec4(0.0, 0.0, 0.0, 0.0); // clear pixels outside of circle
		gl_FragDepth = 1.0;
	}
	
	// simulate 3d volume by calculating position in 3d space
	// and writing respective depth to buffer
	vec4 viewPos = matView * fragPos;
	viewPos.xyz = viewPos.xyz + (normal * sphereRadius);
	vec4 clipPos = matProjection * viewPos;
	
	gl_FragDepth = clipPos.z / clipPos.w;
	
	finalColor = colDiffuse * fragColor;
}