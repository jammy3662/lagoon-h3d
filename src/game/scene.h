#pragma once

#include "../compile/SHADER.c"
#include "../compile/TEXTURE.c"

#include "../engine/define.h"
#include "../engine/shader.h"

struct Uniform {
	int loc;
	virtual void attach(); };

struct UniformInt: public Uniform {
	int* value;
	inline void attach() {
		SHADER.attach(loc, value, SHADER_UNIFORM_INT); }};

struct UniformFloat: public Uniform {
	float* value;
	inline void attach() {
		SHADER.attach(loc, value, SHADER_UNIFORM_INT); }};

struct UniformVec2: public Uniform {
	vec2* value;
	inline void attach() {
		SHADER.attach(loc, value, SHADER_UNIFORM_VEC2); }};

struct UniformVec3: public Uniform {
	vec3* value;
	inline void attach() {
		SHADER.attach(loc, value, SHADER_UNIFORM_VEC3); }};

struct UniformVec4: public Uniform {
	vec4* value;
	inline void attach() {
		SHADER.attach(loc, value, SHADER_UNIFORM_VEC3); }};

struct UniformMat: public Uniform {
	Matrix* value;
	inline void attach() {
		SHADER.attach(loc, *value); }};

struct UniformTexture: public Uniform {
	Texture texture;
	GLuint type;
	inline void attach() {
		SHADER.attach(loc, texture, type); }};

struct Scene
{
	virtual void _render() {}
};

struct ScenePass {
	
static int nextId;
int id;

ScenePass()
{
	id = nextId;
	nextId += 1;
}

Camera* camera = 0;
Shader* shader = 0;
RenderTexture* frame = 0;

vec4 background; // background color
int clearColor = true;
int clearDepth = true;
int hasDepth = true;
// if false, the depth attachment is not a texture
// but a renderbuffer (cannot attach to shaders)

Scene* scene;

Array<Uniform*> attachments;

void attach(char* uniform, int& _int) {
	UniformInt* unf = (UniformInt*) malloc(sizeof(UniformInt));
	unf->value = &_int;
	attachments.append(unf); }

void attach(char* uniform, float& _float) {
	UniformFloat* unf = (UniformFloat*) malloc(sizeof(UniformFloat));
	unf->value = &_float;
	attachments.append(unf); }

void attach(char* uniform, vec2& _vec2) {
	UniformVec2* unf = (UniformVec2*) malloc(sizeof(UniformVec2));
	unf->value = &_vec2;
	attachments.append(unf); }

void attach(char* uniform, vec3& _vec3) {
	UniformVec3* unf = (UniformVec3*) malloc(sizeof(UniformVec3));
	unf->value = &_vec3;
	attachments.append(unf); }

void attach(char* uniform, vec4& _vec4) {
	UniformVec4* unf = (UniformVec4*) malloc(sizeof(UniformVec4));
	unf->value = &_vec4;
	attachments.append(unf); }

void attach(char* uniform, Matrix& _matrix) {
	UniformMat* unf = (UniformMat*) malloc(sizeof(UniformMat));
	unf->value = &_matrix;
	attachments.append(unf); }

void attach(char* uniform, Texture texture, int type) {
	UniformTexture* unf = (UniformTexture*) malloc(sizeof(UniformTexture));
	unf->texture = texture;
	attachments.append(unf); }
	
~ScenePass()
{
	for (int i = 0; i < attachments.size; i++)
	{ free(attachments[i]); }
}

void render()
{
	if (frame) BeginTextureMode(*frame);
	
	if (clearColor || clearDepth)
	{
		glClearColor(background.x, background.y, background.z, background.w);
		glClear(
			(GL_COLOR_BUFFER_BIT * clearColor) |
			(GL_DEPTH_BUFFER_BIT * clearDepth));
	}
	
	if (shader) SHADER.use(*shader);
	else
	{
		fprintf(stderr, "ERR: Render pass [%i] missing shader\n... Using default shader\n", id);
		SHADER.use(defaultShader);
	}
	
	if (camera) Begin3D(*camera, frame->texture.width, frame->texture.height);
	else
	{
		fprintf(stderr, "ERR: Render pass [%i] missing texture\n", id);
		return;
	}
	
	for (int i = 0; i < attachments.size; i++)
	{
		attachments[i]->attach();
	}
	
	if (camera) EndMode3D();
	
	if (frame) EndTextureMode();
}

} /* ScenePass */ ;

int ScenePass::nextId = 0;