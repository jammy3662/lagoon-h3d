#version 330

in vec4 fragPos;
in vec2 fragCoord;
in vec2 fragCoord2;
in vec3 fragNormal;
in vec3 fragTangent;
in vec4 fragColor;

uniform mat4 mvp;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;
uniform mat4 matNormal;

uniform vec4 colDiffuse; // input color from raylib
uniform sampler2D texture0; // diffuse
uniform sampler2D texture1; // specular
uniform sampler2D texture2; // normal
uniform sampler2D texture3; // rough
uniform sampler2D texture4; // occlusion
uniform sampler2D texture5; // emission
//uniform sampler2D texture6; // heightmap
uniform sampler2D texture7; // cubemap
uniform sampler2D texture8; // irradiance

out vec4 finalColor;

uniform vec3 eye; // camera position
uniform float near = 0.1; // near clipping plane distance
uniform float far = 1000.0; // far clipping plane distance

uniform int projection;
uniform int renderDepth = 0;
uniform int renderReflection = 1;

vec4 calcLight(vec4 color, vec4 pos, vec3 normal)
{
	
	
	return color;
}

vec4 calcReflection(vec4 color, vec4 pos, vec3 normal)
{
	
	
	return color;
}

void main()
{
	vec4 color;
	color = colDiffuse * texture(texture0, fragCoord);
	
	if (renderDepth == 1)
	{
		if (projection == 0)
		{
			float z = (gl_FragCoord.z * 2.0 - 1.0) / gl_FragCoord.w;
			float depth = (z + near) / (near + far);
			finalColor = vec4(vec3(depth), 1.0);
			finalColor = vec4(fragCoord.x, fragCoord.y, 1, 1);
		}
		if (projection == 1)
		{
			finalColor = vec4(vec3(gl_FragCoord.z), 1.0);
			finalColor = vec4(fragCoord.x, fragCoord.y, 1, 1);
		}
	}
	if (renderDepth == 0)
	{
		color = calcLight(color, fragPos, fragNormal);
		if (renderReflection == 1)
		{
			color = calcReflection(finalColor, fragPos, fragNormal);
		}
		finalColor = color;
	}
	
	finalColor = vec4(fragCoord.x,1,fragCoord.y,1);
}