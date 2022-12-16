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

uniform float nearClip = 0.01;
uniform float farClip = 100;
uniform vec3 eye; // camera position

uniform float ambient = 0.15; // min light

uniform vec4 sunColor = vec4(1,1,1,1);
uniform mat4 sunProjection;
uniform mat4 sunView;
uniform vec3 sunDir;
uniform sampler2D shadowMap;

uniform sampler2D reflMap;

vec4 blur(sampler2D texture, vec2 uv, float kernel, float radius)
{
	vec4 color = vec4(0);
	
	for (float y = -kernel; y <= kernel; y = y + 1) {
	for (float x = -kernel; x <= kernel; x = x + 1)
	{
		color += texture2D(shadowMap, uv + vec2(radius * x, radius * y));
	}}
	
	float cells = 2*kernel + 1;
	color = color / (cells*cells);
	
	return color;
}

float calcShadow()
{
	float shadow = 1.0;
	
	const float bias = 0.0004;
	
	vec3 pos = (fragSun.xyz / fragSun.w) * 0.5 + 0.5;
	
	// closest distance from camera
	float zcam = pos.z * fragSun.w;
	
	// closest distance from sun
	float zsun = texture2D(shadowMap, pos.xy).r;
	zsun += bias; // avoid shadow acne
	
	// distance of frag from nearest surface
	float dist = zcam - zsun;
	
	// apply gradual shadow if pixel obscured from sun
	if (dist >= 0)
	{
		shadow = ambient;
		shadow += ((zcam / zsun) - 1) / 5;
		shadow = min(shadow, max(ambient, 1-ambient-ambient));
	}
	else
	{
		// pixels visible to sun should
		// be unaffected by shadow
		shadow = 1.0;
	}
	
	return shadow;
}

vec4 calcLight(vec4 color)
{
	float diffuse, specular, shadow;
	
	vec3 lightv = normalize(sunDir);
	diffuse = (dot(fragNormal, lightv) + 1) * 0.5;
	diffuse = (diffuse * (1 - ambient)) + ambient;
	
	vec3 viewv = normalize(eye - fragPos.xyz);
	vec3 halfv = normalize(lightv + viewv);
	vec3 reflv = reflect(fragNormal, halfv);
	specular = max(pow(dot(halfv, reflv), 16), 0) * 0.4;
	
	shadow = calcShadow();
	
	float light = (diffuse + specular) * shadow;
	color.rgb *= sunColor.rgb * light;
	
	return color;
}

void main()
{
	vec4 color;
	color = colDiffuse * fragColor * texture(texture0, fragCoord);
	
	color = calcLight(color);
	
	finalColor = color;
}