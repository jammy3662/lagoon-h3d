#pragma once

#include <raylib.h>
#include <rlgl.h>

#include <glad/glad.h>

#define Rectangle(x,y,w,h) {(float)x,(float)y,(float)w,(float)h}

RenderTexture2D LoadRenderTextureSharedDepth(RenderTexture2D source)
{
	int width = source.texture.width;
	int height = source.texture.height;
	
    RenderTexture2D target = {0};

    target.id = rlLoadFramebuffer(source.texture.width, source.texture.height);   // Load an empty framebuffer

    if (target.id > 0)
    {
        rlEnableFramebuffer(target.id);

        // Create color texture (default to RGBA)
        target.texture.id = rlLoadTexture(NULL, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        target.texture.width = width;
        target.texture.height = height;
        target.texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        target.texture.mipmaps = 1;

        // Create depth texture
        target.depth = source.depth;

        // Attach color texture and depth texture to FBO
        rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);

        // Check if fbo is complete with attachments (valid)
		if (rlFramebufferComplete(target.id)) printf("Framebuffer object %i created successfully\n", target.id);
        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    } 
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}

RenderTexture2D LoadRenderTextureWithDepthTexture(int width, int height)
{
    RenderTexture2D target = {0};

    target.id = rlLoadFramebuffer(width, height);   // Load an empty framebuffer

    if (target.id > 0)
    {
        rlEnableFramebuffer(target.id);

        // Create color texture (default to RGBA)
        target.texture.id = rlLoadTexture(NULL, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        target.texture.width = width;
        target.texture.height = height;
        target.texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        target.texture.mipmaps = 1;

        // Create depth texture
        target.depth.id = rlLoadTextureDepth(width, height, false);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach color texture and depth texture to FBO
        rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        // Check if fbo is complete with attachments (valid)
		if (rlFramebufferComplete(target.id)) printf("Framebuffer object %i created successfully\n", target.id);
        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    } 
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}

Matrix MatrixProjection(Camera3D camera, float w, float h)
{
	float aspect = w / h;
	
	if (camera.projection == CAMERA_ORTHOGRAPHIC)
	{
		double top = camera.fovy / 2.0;
		double right = top * aspect;
		
		return MatrixOrtho(-right, right, -top, top, nearClip, farClip);
	}
	else
	{
		double top = nearClip * tan(camera.fovy * 0.5 * DEG2RAD);
		double right = top * aspect;
		
		return MatrixFrustum(-right, right, -top, top, nearClip, farClip);
	}
}

// Initializes 3D mode with custom camera (3D)
void Begin3D(Camera3D camera, float w, float h)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    rlPushMatrix();                 // Save previous matrix, which contains the settings for the 2d ortho projection
    rlLoadIdentity();               // Reset current matrix (projection)

	float aspect = w / h;
    //float aspect = (float)rlGetFramebufferWidth()/(float)rlGetFramebufferHeight();

    // NOTE: zNear and zFar values are important when computing depth buffer values
    if (camera.projection == CAMERA_PERSPECTIVE)
    {
        // Setup perspective projection
        double top = nearClip * tan(camera.fovy*0.5*DEG2RAD);
		double right = top*aspect;
		
        rlFrustum(-right, right, -top, top, nearClip, farClip);
    }
    else if (camera.projection == CAMERA_ORTHOGRAPHIC)
    {
        // Setup orthographic projection
        double top = camera.fovy/2.0;
        double right = top*aspect;

        rlOrtho(-right, right, -top,top, nearClip, farClip);
    }

    rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
    rlLoadIdentity();               // Reset current matrix (modelview)

    // Setup Camera view
    Matrix matView = MatrixLookAt(camera.position, camera.target, camera.up);
    rlMultMatrixf(MatrixToFloat(matView));      // Multiply modelview matrix by view matrix (camera)

    rlEnableDepthTest();            // Enable DEPTH_TEST for 3D
}

void DrawBillboardXY(Texture tx, Vector3 pos, Vector2 size, Camera3D cam)
{
	rlCheckRenderBatchLimit(10);
	rlPushMatrix();

	// get the camera view matrix
	Matrix mat = MatrixLookAt(cam.position, cam.target, cam.up);
	// peel off just the rotation
	Quaternion quat = QuaternionFromMatrix(mat);
	mat = QuaternionToMatrix(quat);

	// apply just the rotation
	rlMultMatrixf(MatrixToFloat((mat)));

	// translate backwards in the inverse rotated matrix to put the item where it goes
	pos = Vector3Transform(pos, MatrixInvert(mat));
	rlTranslatef(pos.x, pos.y, pos.z);

	// draw the billboard
	float width = size.x/2;
	float height = size.y/2;

	Color color = {255,255,255,255};

	rlSetTexture(tx.id);

	// draw quad
	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);
	// Front Face
	rlNormal3f(0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(-width, -height, 0);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(+width, -height, 0);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(+width, +height, 0);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(-width, +height, 0);  // Top Left Of The Texture and Quad

	rlEnd();
	rlSetTexture(0);
	rlPopMatrix();
}

inline void DrawTextureFlippedY(Texture texture, float x, float y, Color tint)
{
	DrawTexturePro (
		texture,
		{(float)x,(float)y, (float)texture.width, (float)-texture.height},
		{(float)x,(float)y, (float)texture.width, (float)texture.height},
		{0,0}, 0, tint);
}

Texture LoadTexture3D(int width, int height, int depth, uint8* texelbuffer)
{
	Texture ret;
	ret.width = width;
	ret.height = height;
	ret.mipmaps = depth;
	ret.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	
	glGenTextures(1, &ret.id);
	glBindTexture(GL_TEXTURE_3D, ret.id);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA,
		width, height, depth,
		0, GL_RGBA, GL_UNSIGNED_BYTE, texelbuffer);
		
	return ret;
}

TextureCubemap LoadTextureCubeMap(Texture top, Texture bottom, Texture north, Texture south, Texture east, Texture west)
{
	TextureCubemap ret;
	
	// Convert image data to 6 faces in a vertical column, that's the optimum layout for loading
	Image map = GenImageColor(top.width, top.height*6, MAGENTA);
	ImageFormat(&map, top.format);

	Texture faces[] = {west, east, top, bottom, north, south};
	
	for (int i = 0; i < 6; i++)
	{
		ImageDraw(&map, LoadImageFromTexture(faces[i]),
			Rectangle(0, 0, top.width, top.height),
			Rectangle(0, top.height*i, top.width, top.height),
			{255,255,255,255});
	}
	
	ret.id = rlLoadTextureCubemap(map.data, top.width, map.format); 
	if (ret.id == 0) fprintf(stderr, "CUBEMAP ERR: could not load\n");
	
	UnloadImage(map);
	
	return ret;
}
