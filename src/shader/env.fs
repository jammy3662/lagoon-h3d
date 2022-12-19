#version 330

in vec4 fragPos;
in vec4 fragSun; // frag position in sun space
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

out vec4 finalColor;

uniform vec3 eye; // camera position

uniform vec3 sunDir;
uniform vec4 sunColor;

uniform float ambient = 0.15; // min light

void main()
{
	vec4 color;
	
	color = colDiffuse * fragColor * texture(texture0, fragCoord);
	
	float light;
	
	vec3 lightv = normalize(sunDir);
	light = (dot(fragNormal, lightv) + 1) * 0.5;
	light = (light * (1 - ambient)) + ambient;
	
	color.rgb *= sunColor.rgb * light;
	
	finalColor = color;
}