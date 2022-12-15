#version 330

in vec4 fragPos;
in vec2 fragCoord;
in vec2 fragCoord2;
in vec3 fragNormal;
in vec3 fragTangent;
in vec4 fragColor;
in vec4 sunFragPos;

uniform sampler2D frame;
uniform vec4 colDiffuse;

out vec4 finalColor;

void main()
{
	finalColor.rgb = colDiffuse * fragColor * texture(frame, fragCoord).rgb;
}