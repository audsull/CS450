#version 150

in  vec4 color;
out vec4 fColor;
uniform int choose;
uniform vec4 PickingColor;

void main()
{
    fColor = color;
    if(choose == 1) {
        fColor = PickingColor;
    }
}
