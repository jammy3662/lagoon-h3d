#pragma once

#include <raylib.h>
#include <rlgl.h>

#include <glad/glad.h>

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

Matrix GetCameraProjection(Camera3D camera, double nearClip, double farClip)
{
	float aspect = (float)rlGetFramebufferWidth()/(float)rlGetFramebufferHeight();
	
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
void BeginMode3D(Camera3D camera, double nearClip, double farClip)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    rlPushMatrix();                 // Save previous matrix, which contains the settings for the 2d ortho projection
    rlLoadIdentity();               // Reset current matrix (projection)

    float aspect = (float)rlGetFramebufferWidth()/(float)rlGetFramebufferHeight();

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

#define DrawTextureFlippedY(texture, x, y, tint) DrawTexturePro(texture, \
	{(float)x,(float)y, (float)texture.width, (float)-texture.height}, \
	{(float)x,(float)y, (float)texture.width, (float)texture.height}, \
	{0.0,0.0}, 0, tint)

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
	
	Image faces;
	
	// Convert image data to 6 faces in a vertical column, that's the optimum layout for loading
	faces = GenImageColor(top.width, top.height*6, MAGENTA);
	ImageFormat(&faces, top.format);

	// NOTE: Image formating does not work with compressed textures
	
	ImageDraw(&faces, LoadImageFromTexture(west),
		{(float)0,(float)0,(float) top.width,(float) top.height},
		{(float)0,(float)top.height*0,(float) top.width,(float) top.height},
		{255,255,255,255});
	ImageDraw(&faces, LoadImageFromTexture(east),
		{(float)0,(float)0,(float) top.width,(float) top.height},
		{(float)0,(float)top.height*1,(float) top.width,(float) top.height},
		{255,255,255,255});
	ImageDraw(&faces, LoadImageFromTexture(top),
		{(float)0,(float)0,(float) top.width,(float) top.height},
		{(float)0,(float)top.height*2,(float) top.width,(float) top.height},
		{255,255,255,255});
	ImageDraw(&faces, LoadImageFromTexture(bottom),
		{(float)0,(float)0,(float) top.width,(float) top.height},
		{(float)0,(float)top.height*3,(float) top.width,(float) top.height},
		{255,255,255,255});
	ImageDraw(&faces, LoadImageFromTexture(north),
		{(float)0,(float)0,(float) top.width,(float) top.height},
		{(float)0,(float)top.height*4,(float) top.width,(float) top.height},
		{255,255,255,255});
	ImageDraw(&faces, LoadImageFromTexture(south),
		{(float)0,(float)0,(float) top.width,(float) top.height},
		{(float)0,(float)top.height*5,(float) top.width,(float) top.height},
		{255,255,255,255});
	
	ret.id = rlLoadTextureCubemap(faces.data, top.width, faces.format); 
	if (ret.id == 0) fprintf(stderr, "CUBEMAP ERR: could not load\n");
	
	UnloadImage(faces);
	
	return ret;
}