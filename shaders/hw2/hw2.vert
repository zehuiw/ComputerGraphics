#version 330

layout(location = 0) in vec4 vertexPosition;
uniform float inputTime;

out vec4 color;

void main()
{
    vec4 modifiedVertexPosition = vertexPosition;

    // Insert your code for "Slightly-More Advanced Shaders" here.
    int intpart =  int(inputTime);
    float scaled = inputTime - intpart;
    float color_float = scaled;

    if(scaled < 0.5) {
    	color_float = 2 * scaled;
    	scaled = -2 * scaled;
    }
    else {
    	color_float = -2 * scaled + 2;
    	scaled = 2 * scaled - 2.;
    }

    modifiedVertexPosition.y += scaled;

    color.x = 1;
    color.y = color_float;
    color.z = 1 - color_float;
    color.w = 1;

    gl_Position = modifiedVertexPosition;
}
