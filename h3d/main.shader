context GBUFFER
{
	
}

[[GBUFFER_VS]]

	attribute vec3 vertPos;
	attribute vec2 texCoords0;
	attribute vec3 normal;
	
	varying vec3 pos;
	varying vec2 uv;
	varying vec3 norm;

	void main()
	{
		// these are passed in by horde3d
		pos = vertPos;
		uv = texCoords0;
		norm = normal;
		
		gl_Position = viewProjMat * vec4(vertPos.x, vertPos.y, vertPos.z, 1);
	}


[[GBUFFER_FS]]

	varying vec3 pos;
	varying vec2 uv;
	varying vec3 norm;
	
	void main()
	{
		gl_FragColor = 