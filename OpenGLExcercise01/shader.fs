#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D   texture1;
uniform sampler2D   texture2;
uniform float ourGreen;

vec2 TexCoordOffset=vec2(-TexCoord.x,TexCoord.y);
void main()
{
 FragColor = mix(texture(texture1, TexCoord), texture(texture2,TexCoordOffset ), ourGreen);
  // FragColor = mix(texture(ourtexture1, TexCoord), texture(ourtexture2, TexCoord), ourGreen);
}