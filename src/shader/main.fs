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
uniform float far = 100.0; // far clipping plane distance

uniform int projection;
uniform int renderDepth = 0;
uniform int renderReflection = 1;

vec3 calcLight(vec3 color, vec4 pos, vec3 normal)
{
	
	
	return color;
}

vec3 calcReflection(vec3 color, vec4 pos, vec3 normal)
{
	
	
	return color;
}

void main()
{
	vec3 color;
	color = colDiffuse.rgb * texture(texture0, fragCoord).rgb;
	
	float depth;
	
	if (projection == 0)
	{
		float z = (gl_FragCoord.z * 2.0 - 1.0) / gl_FragCoord.w;
		depth = (z + near) / (near + far);
	}
	if (projection == 1)
	{
		depth = gl_FragCoord.z;
	}
	
	//color = calcLight(color, fragPos, fragNormal);
	
	if (renderReflection == 1)
	{
		//color = calcReflection(color, fragPos, fragNormal);
	}
	
	finalColor.rgb = color;
	finalColor.a = depth;
	finalColor.a = 1;
}