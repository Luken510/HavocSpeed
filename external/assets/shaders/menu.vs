#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec2 fragTexCoord;

out vec2 texCoord;

void main()
{
	texCoord = vec2(fragTexCoord.x, 1.0-fragTexCoord.y); //inverts image

    gl_Position = vec4(VertexPosition,1.0);
}
