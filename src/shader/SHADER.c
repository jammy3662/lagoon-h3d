#ifndef HL_COMPILE_RES
extern
#endif
char* depthfsShaderCode
#ifdef HL_COMPILE_RES
= 
"\
#version 330\
\n	\
\n	uniform float near;\
\n	uniform float far;\
\n	\
\n	uniform int projection;\
\n	\
\n	uniform sampler2D tex;\
\n	\
\n	out vec4 finalColor;\
\n	\
\n	void main()\
\n	{\
\n		if (projection == 0)\
\n		{	\
\n			float z = (gl_FragCoord.z * 2.0 - 1.0) / gl_FragCoord.w;\
\n			float depth = (z + near) / (near + far);\
\n			finalColor = vec4(vec3(depth), 1.0);\
\n		}\
\n		else\
\n		{\
\n			finalColor = vec4(vec3(gl_FragCoord.z), 1.0);\
\n		}\
\n	}\
"
#endif
;
#ifndef HL_COMPILE_RES
extern
#endif
char* flyoveroceanfsShaderCode
#ifdef HL_COMPILE_RES
= 
"\
#version 330\
\n	\
\n	// Input vertex attributes (from vertex shader)\
\n	in vec3 fragPosition;\
\n	in vec2 fragTexCoord;\
\n	in vec4 fragColor;\
\n	in vec3 fragNormal;\
\n	\
\n	// Input uniform values\
\n	uniform sampler2D texture0;\
\n	uniform vec4 colDiffuse;\
\n	\
\n	// Output fragment color\
\n	out vec4 finalColor;\
\n	\
\n	const vec3 lightdir = vec3(1,1,0);\
\n	\
\n	const vec4 oceancolor = vec4(0,0.3,0.4,1);\
\n	\
\n	void main()\
\n	{\
\n	    // Texel color fetching from texture sampler\
\n	    vec4 texelColor = texture(texture0, fragTexCoord);\
\n	\
\n	    vec3 ldir = normalize(lightdir);\
\n	    float diffuse = max(dot(fragNormal, ldir), 0.2);\
\n	\
\n	    finalColor = oceancolor * texelColor * colDiffuse * diffuse;\
\n	    finalColor.a = 1;\
\n	}\
\n	\
"
#endif
;
#ifndef HL_COMPILE_RES
extern
#endif
char* painttestfsShaderCode
#ifdef HL_COMPILE_RES
= 
"\
#version 330\
\n	\
\n	// Input vertex attributes (from vertex shader)\
\n	in vec2 fragCoord;\
\n	in vec4 fragColor;\
\n	in vec4 fragPos;\
\n	in vec3 fragNormal;\
\n	in vec4 shadowPos;\
\n	\
\n	// Output fragment color\
\n	out vec4 finalColor;\
\n	\
\n	uniform int fullPaint = 0;\
\n	\
\n	// Input uniform values\
\n	uniform sampler2D texture0;\
\n	uniform vec4 colDiffuse;\
\n	\
\n	uniform mat4 mvp;\
\n	\
\n	// camera position\
\n	uniform vec3 eye;\
\n	\
\n	uniform int time;\
\n	\
\n	uniform sampler2D paintnoise;\
\n	uniform sampler2D paintbump;\
\n	uniform sampler2D paintpattern;\
\n	uniform sampler3D octree;\
\n	uniform sampler2D shadowmap;\
\n	\
\n	uniform samplerCube reflection;\
\n	\
\n	// -- lighting -- //\
\n	uniform vec3 sun_color = vec3(1,1,1);\
\n	uniform vec3 sun_dir = vec3(0.5,1,0.5);\
\n	uniform float sun_dist = 100;\
\n	\
\n	uniform float ambient = 0.3;\
\n	//----//\
\n	\
\n	// -- paint -- //\
\n	uniform vec3 paint_color = vec3(1,0.5,1);\
\n	uniform float paint_glow = 0.1;\
\n	\
\n	uniform float edge_size = 1;\
\n	uniform float edge_intensity = 0.4;\
\n	\
\n	uniform float paint_size = 0.2;\
\n	uniform float scatter = 10.0;\
\n	//----//\
\n	\
\n	uniform vec2 samplept = vec2(0.5,0.5);\
\n	\
\n	const float uvf = 8.0;\
\n	\
\n	vec4 paintcolor(vec2 point)\
\n	{\
\n		vec4 ret = vec4(paint_color, 0.0);\
\n		\
\n		vec3 n = normalize(fragNormal);\
\n		vec2 uv = fragCoord * uvf;\
\n		\
\n		// -- contribution -- //\
\n		float noiseContrib = texture2D(paintnoise, uv).r;\
\n		float paintStrength = distance(fragCoord, point) / -paint_size;\
\n		ret.a = pow((noiseContrib + paintStrength), scatter);\
\n		\
\n		float edge = ret.a;\
\n		ret.a = 1 - ret.a;\
\n		\
\n		// clip low values to kill fuzzy edges\
\n		if (ret.a < 0.5) { ret.a = 0.0; }\
\n		else\
\n		{\
\n			ret.rgb -= edge_intensity * pow(edge, edge_size);\
\n			ret.rgb += paint_glow;\
\n			ret.a = 1.0;\
\n		}\
\n		// -- end contribution -- //\
\n		\
\n		float bumpScale = 0.6;\
\n		n.x += (sin(texture2D(paintbump, uv * bumpScale).r) - 0.5) * 0.4;\
\n		n.z += (sin(texture2D(paintbump, uv.yx * bumpScale).r) - 0.5) * 0.4;\
\n		n = normalize(n);\
\n		\
\n		// -- lighting -- //\
\n		float diffuse, specular, shadow;\
\n		\
\n		vec3 lightv = normalize(sun_dir);\
\n		diffuse = (dot(n, lightv) + 1) * 0.5;\
\n		diffuse = (diffuse * (1 - ambient)) + ambient;\
\n		\
\n		vec3 specLightv = normalize(vec4(normalize(sun_dir), 1.0) * mvp).xyz;\
\n		vec3 viewv = normalize(eye - fragPos.xyz);\
\n		vec3 halfv = normalize(lightv + viewv);\
\n		vec3 reflv = reflect(n, halfv);\
\n		specular = max(pow(dot(halfv, reflv), 16), 0) * 0.4;\
\n		\
\n		vec3 shadowpt = (shadowPos.xyz / shadowPos.w) * 0.5 + 0.5;\
\n		float shadowSample = texture2D(shadowmap, shadowpt.xy).r;\
\n		if (shadowSample < shadowPos.z)\
\n		{\
\n			shadow = 0.2;\
\n		}\
\n		else\
\n		{\
\n			shadow = 1.0;\
\n		}\
\n		\
\n		vec3 cubev = reflect(viewv, reflv);\
\n		vec3 refl = textureCube(reflection, cubev).rgb;\
\n		float reflFac = 0.03;\
\n		\
\n		//specular = 0;\
\n		ret.rgb *= sun_color * (diffuse + specular);\
\n		ret.rgb = ret.rgb + sun_color * (diffuse - 0.5) * 0.5;\
\n		//ret.rgb *= shadow;\
\n		//ret.rgb = (ret.rgb * 1 - reflFac) + (refl * reflFac);	\
\n		\
\n		return ret;\
\n	}\
\n	\
\n	vec3 overlay(vec3 first, vec3 second, float a)\
\n	{\
\n	    return first * a + second * (1.0 - a);\
\n	}\
\n	\
\n	void main()\
\n	{\
\n		// Texel color fetching from texture sampler\
\n		vec4 tex = texture2D(texture0, fragCoord);\
\n		\
\n		vec4 color = tex * colDiffuse;\
\n		\
\n		float diffuse, specular;\
\n		\
\n		vec3 lightv = normalize(sun_dir);\
\n		diffuse = (dot(fragNormal, lightv) + 1) * 0.5;\
\n		diffuse = (diffuse * (1 - ambient)) + ambient;\
\n		\
\n		color.rgb *= sun_color * diffuse;\
\n		color.rgb = color.rgb + sun_color * (diffuse - 0.5) * 0.5;\
\n		\
\n		vec4 paint1 = paintcolor(samplept);\
\n		vec3 paintc = paint1.rgb;\
\n		if (fullPaint == 1)\
\n		{\
\n			paint1.a = 1;\
\n		}\
\n		color.rgb = overlay(paintc, color.rgb, paint1.a);\
\n		\
\n		finalColor.rgb = color.rgb;\
\n		finalColor.a = 1;\
\n	}\
\n	\
"
#endif
;
#ifndef HL_COMPILE_RES
extern
#endif
char* basevsShaderCode
#ifdef HL_COMPILE_RES
= 
"\
#version 330\
\n	\
\n	layout (location = 0) in vec3 vertexPos;\
\n	layout (location = 1) in vec3 vertexNormal; \
\n	layout (location = 2) in vec2 vertexCoord; \
\n	layout (location = 3) in vec2 vertexCoord2;\
\n	layout (location = 4) in vec4 vertexColor;\
\n	\
\n	out vec4 fragPos;\
\n	out vec3 fragNormal;\
\n	out vec2 fragCoord;\
\n	out vec2 fragCoord2;\
\n	out vec4 fragColor;\
\n	\
\n	uniform mat4 mvp;\
\n	uniform mat4 modelMatrix;\
\n	uniform mat4 normalMatrix;\
\n	uniform mat4 lightMatrix;\
\n	\
\n	out vec4 shadowPos;\
\n	\
\n	void calcLightSpace()\
\n	{\
\n		shadowPos = modelMatrix * lightMatrix * vec4(vertexPos, 1.0);\
\n	}\
\n	\
\n	void main()\
\n	{\
\n		 // Send vertex attributes to fragment shader\
\n		fragPos = modelMatrix * vec4(vertexPos, 1.0);\
\n		fragCoord = vertexCoord;\
\n		fragColor = vertexColor;\
\n		fragNormal = (normalMatrix * vec4(vertexNormal, 1.0)).xyz;\
\n		\
\n		// Calculate final vertex position\
\n		gl_Position = mvp*vec4(vertexPos, 1.0);\
\n		\
\n		calcLightSpace();\
\n	}\
"
#endif
;
#ifndef HL_COMPILE_RES
extern
#endif
char* depthvsShaderCode
#ifdef HL_COMPILE_RES
= 
"\
\
"
#endif
;
#ifndef HL_COMPILE_RES
extern
#endif
char* flyoveroceanvsShaderCode
#ifdef HL_COMPILE_RES
= 
"\
#version 330\
\n	\
\n	layout (location = 0) in vec3 vertexPos;\
\n	layout (location = 1) in vec3 vertexNormal; \
\n	layout (location = 2) in vec2 vertexCoord; \
\n	layout (location = 3) in vec2 vertexCoord2;\
\n	layout (location = 4) in vec4 vertexColor;\
\n	\
\n	out vec4 fragPos;\
\n	out vec3 fragNormal;\
\n	out vec2 fragCoord;\
\n	out vec2 fragCoord2;\
\n	out vec4 fragColor;\
\n	\
\n	uniform mat4 mvp;\
\n	uniform mat4 modelMatrix;\
\n	uniform mat4 normalMatrix;\
\n	\
\n	// Input vertex attributes\
\n	in vec3 vertexPosition;\
\n	in vec2 vertexTexCoord;\
\n	in vec2 vertexTexCoord2;\
\n	in vec3 vertexNormal;\
\n	in vec3 vertexTangent;\
\n	in vec4 vertexColor;\
\n	\
\n	// Input uniform values\
\n	uniform mat4 mvp;\
\n	uniform mat4 matView;\
\n	uniform mat4 matProjection;\
\n	uniform mat4 matModel;\
\n	uniform mat4 matNormal;\
\n	\
\n	uniform sampler2D texture1;\
\n	uniform sampler2D texture2;\
\n	\
\n	// Output vertex attributes (to fragment shader)\
\n	out vec3 fragPosition;\
\n	out vec2 fragTexCoord;\
\n	out vec4 fragColor;\
\n	out vec3 fragNormal;\
\n	\
\n	uniform int time;\
\n	\
\n	void main()\
\n	{\
\n	    float tim = time / 100;\
\n	    float waves = 0.1 * (sin(vertexPosition.x) +\
\n	        cos(vertexPosition.z));\
\n	    \
\n	    vec3 pos = vertexPosition;\
\n		pos.y += waves;\
\n	    \
\n	    vec3 norm = vertexNormal;\
\n	    norm.x += waves;\
\n	    norm.z += waves;\
\n	    norm = normalize(norm);\
\n	    \
\n	    // Send vertex attributes to fragment shader\
\n		fragPosition = vec3(matModel*vec4(pos, 1.0));\
\n		fragTexCoord = vertexTexCoord;\
\n		fragColor = vertexColor;\
\n		fragNormal = normalize(vec3(matNormal*vec4(norm, 1.0)));\
\n		\
\n	    // Calculate final vertex position\
\n	    gl_Position = mvp*vec4(pos, 1.0);\
\n	}\
"
#endif
;
