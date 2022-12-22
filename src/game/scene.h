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

struct Scene {
	
static int _nextId;
int _id;

Scene()
{
	
}

vec4 background = {1,1,1,1}; // background color
int clearColor = true;
int clearDepth = true;

static Scene create()
{
	Scene scn;
	
	scn._id = _nextId;
	_nextId += 1;
	scn._attachments = {.data = 0, .cap = 0, .size = 0};
	
	return scn;
}

Array<Uniform*> _attachments;

void attach(char* uniform, int& _int) {
	UniformInt* unif = (UniformInt*) malloc(sizeof(UniformInt));
	unif->value = &_int;
	_attachments.append(unif); }

void attach(char* uniform, float& _float) {
	UniformFloat* unif = (UniformFloat*) malloc(sizeof(UniformFloat));
	unif->value = &_float;
	_attachments.append(unif); }

void attach(char* uniform, vec2& _vec2) {
	UniformVec2* unif = (UniformVec2*) malloc(sizeof(UniformVec2));
	unif->value = &_vec2;
	_attachments.append(unif); }

void attach(char* uniform, vec3& _vec3) {
	UniformVec3* unif = (UniformVec3*) malloc(sizeof(UniformVec3));
	unif->value = &_vec3;
	_attachments.append(unif); }

void attach(char* uniform, vec4& _vec4) {
	UniformVec4* unif = (UniformVec4*) malloc(sizeof(UniformVec4));
	unif->value = &_vec4;
	_attachments.append(unif); }

void attach(char* uniform, Matrix& _matrix) {
	UniformMat* unif = (UniformMat*) malloc(sizeof(UniformMat));
	unif->value = &_matrix;
	_attachments.append(unif); }

void attach(char* uniform, Texture texture, int type) {
	UniformTexture* unif = (UniformTexture*) malloc(sizeof(UniformTexture));
	unif->texture = texture;
	_attachments.append(unif); }
	
~Scene()
{
	for (int i = 0; i < _attachments.size; i++)
	{ free(_attachments[i]); }
}

struct Handle 
{
	RenderTexture* frame = 0;
	~Handle()
	{
		EndMode3D();
		// always validate here because null
		// means to SKIP binding the framebuffer
		if (frame) EndTextureMode();
	}
};

Handle render(Shader shader, Camera3D camera, RenderTexture* frame)
{
	SHADER.use(shader);
	for (int i = 0; i < _attachments.size; i++)
	{
		_attachments[i]->attach();
	}

	// always validate here because null
	// means to SKIP binding the framebuffer
	if (frame) BeginTextureMode(*frame);
	
	if (clearColor || clearDepth)
	{
		glClearColor(background.x, background.y, background.z, background.w);
		glClear(
			(GL_COLOR_BUFFER_BIT * clearColor) |
			(GL_DEPTH_BUFFER_BIT * clearDepth));
	}
	
	Begin3D(camera, frame->texture.width, frame->texture.height);
	
	return (Handle){.frame = frame};
}

} /* Scene */ ;

typedef Scene::Handle SceneHandle;

int Scene::_nextId = 0;