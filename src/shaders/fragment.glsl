#version 420 core

out vec4 fragment_colour;

in vec2 uv;
uniform sampler2D image;

void main()
{
        fragment_colour = texture( image, uv );
}
