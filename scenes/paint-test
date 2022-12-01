#pragma once

struct PaintTest
{

Texture tex;
Texture tex2;
Texture random;
Texture cubemap;
Mesh mesh;
Material painttest;

Matrix meshm;

Model stage;

int32 time = 0;

float lightheight = 3;

float lightNearClip, lightFarClip;

RenderTexture shadowmap;
//RenderTexture sunmap;

Shader paintshader;
Shader depthshader;

Tween tween;

Mesh plane;

Player player;
Camera camera;
Camera3D lightcam;

int8 paused = false;

int8 fullPaint = 0;

float paint_glow = 0.1;

float edge_size = 1.2;
float edge_intensity = 2.5;

float paint_size = 0.2;
float scatter = 10.0;

Octree octree;

void init()
{
	tex = LoadTexture("res/texture/paintnoise.png");
	tex2 = LoadTexture("res/texture/paintbump.png");
	random = LoadTexture("res/texture/yahshas.png");
	
	cubemap = LoadTextureCubeMap(
		LoadTexture("res/texture/landscape-top.png"),
		LoadTexture("res/texture/landscape-bottom.png"),
		LoadTexture("res/texture/landscape-north.png"),
		LoadTexture("res/texture/landscape-south.png"),
		LoadTexture("res/texture/landscape-east.png"),
		LoadTexture("res/texture/landscape-west.png"));
	
	mesh = GenMeshCube(0.3,0.3,0.3);
	mesh = GenMeshSphere(0.15, 16, 16);
	plane = GenMeshPlane(4, 4, 10, 10);
	
	paintshader = LoadShader("res/shader/painttest.vs", "res/shader/painttest.fs");
	depthshader = LoadShader(0, "res/shader/depth.fs");
	
	painttest = LoadMaterialDefault();
	
	stage = LoadModel("res/mesh/underpass.glb");
	
	player = newPlayer(newCamera(true, false, 100, CAMERA_PERSPECTIVE));
	player.cam.orbitDist = 1;
	player.cam.orbitOffset = {0,player.height/3,0};
	useCamera(&player.cam);
	
	meshm = MatrixIdentity();
	
	octree.create(10, 10, 10);
	
	shadowmap = LoadRenderTextureWithDepthTexture(fwidth, fheight);
	
	SetCameraMode(lightcam, CAMERA_CUSTOM);
	lightcam.position = {0,lightheight,0};
	lightcam.target = {0,0,0};
	lightcam.up = {1,0,0};
	lightcam.fovy = 90;
	lightcam.projection = CAMERA_ORTHOGRAPHIC;
	
	lightNearClip = 0.05;
	lightFarClip = 100;
	
	SetCameraMoveControls(0, 0, 0, 0, 0, 0);
	
	DisableCursor();
}

void update()
{
	time++;
	
	player.update();
	lightcam.position = player.cam.anchor;
	lightcam.position.y += lightheight;
	lightcam.target = player.cam.anchor;
	
	UpdateJobs(tween);
	
	if (IsKeyPressed(KEY_ESCAPE)) 
	{
		paused = !paused;
		player.active = !player.active;
		if (paused) EnableCursor();
		else DisableCursor();
	}
	
	if (IsKeyDown(KEY_E)) meshm = MatrixMultiply(meshm, MatrixRotateY(0.05));
	if (IsKeyDown(KEY_Q)) meshm = MatrixMultiply(meshm, MatrixRotateY(-0.05));
	
	if (IsKeyPressed(KEY_UP))
	{
		paint_size += 0.05;
	}
	if (IsKeyPressed(KEY_DOWN))
	{
		paint_size -= 0.05;
	}
	
	if (IsKeyPressed(KEY_P))
	{
		fullPaint = !fullPaint;
	}
}

void present()
{
	DrawMesh(mesh, painttest, meshm);
	//DrawMesh(plane, painttest, MatrixTranslate(0,-0.5,0));
	DrawModel(stage, {0,-1.5,0}, 1, {255,255,255,255});
}

void render()
{
	int texSlot;
	
	BeginTextureMode(shadowmap);
	ClearBackground({255,255,255,0});
		
		ShaderBind(depthshader);
		painttest.shader = depthshader;
		for (int i = 0; i < stage.materialCount; i++)
		{
			stage.materials[i].shader = depthshader;
		}
		
		BeginShaderMode(depthshader);
		
		ShaderValue(ShaderLoc("near"), &lightNearClip, SHADER_UNIFORM_FLOAT);
		ShaderValue(ShaderLoc("far"), &lightFarClip, SHADER_UNIFORM_FLOAT);
		ShaderValue(ShaderLoc("projection"), &lightcam.projection, SHADER_UNIFORM_INT);
		
		BeginMode3D(activeCamera);
		
			present();
		
		EndMode3D();
		
		ShaderTextureClear();
		EndShaderMode();
	EndTextureMode();
	
	//DrawTextureFlippedY(shadowmap.texture, 0,0, {255,255,255,255});
	
	//BeginTextureMode(colorBuffer);
	//ClearBackground({255,255,255,0});
	
		ShaderBind(paintshader);
		painttest.shader = paintshader;
		for (int i = 0; i < stage.materialCount; i++)
		{
			stage.materials[i].shader = paintshader;
		}
		
		ShaderValue(ShaderLoc("eye"),	&player.cam.position, SHADER_UNIFORM_VEC3);
		ShaderValue(ShaderLoc("edge_size"), &edge_size, SHADER_UNIFORM_FLOAT);
		ShaderValue(ShaderLoc("time"), &time, SHADER_UNIFORM_INT);
		
		ShaderValue(ShaderLoc("paint_size"), &paint_size, SHADER_UNIFORM_FLOAT);
		ShaderValue(ShaderLoc("paint_size"), &paint_size, SHADER_UNIFORM_FLOAT);
		ShaderValue(ShaderLoc("fullPaint"), &fullPaint, SHADER_UNIFORM_INT);
		
		ShaderMatrix(ShaderLoc("matLight"),	MatrixMultiply
		(
			GetCameraMatrix(lightcam),
			GetCameraProjection(lightcam, lightNearClip, lightFarClip)
		));
		
		BeginShaderMode(paintshader);
		
		texSlot = 0;
		ShaderTexture(ShaderLoc("paintnoise"), tex, &texSlot, GL_TEXTURE_2D);
		ShaderTexture(ShaderLoc("paintbump"),	tex2, &texSlot, GL_TEXTURE_2D);
		ShaderTexture(ShaderLoc("paintpattern"), tex2, &texSlot, GL_TEXTURE_2D);
		ShaderTexture(ShaderLoc("octree"), octree.handle, &texSlot, GL_TEXTURE_3D);
		ShaderTexture(ShaderLoc("shadowmap"), shadowmap.texture, &texSlot, GL_TEXTURE_2D);
		ShaderTexture(ShaderLoc("reflection"), cubemap, &texSlot, GL_TEXTURE_CUBE_MAP);
		
		BeginMode3D(player.cam, player.cam.nearClip, player.cam.farClip);
			
			present();
		
		EndMode3D();
		
		ShaderTextureClear();
		EndShaderMode();
	//EndTextureMode();
	
	//DrawTextureFlippedY(colorBuffer.texture, 0,0, {255,255,255,255});
}

};
