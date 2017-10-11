#version 330

in vec4 color;
out vec4 fragColor;

void main() 
{
    //vec4 finalColor = vec4(0.5);

    // Insert your code for "Slightly-More Advanced Shaders" here.
    vec4 finalColor = color;

    fragColor = finalColor;
}
