/*
 * Per-pixel lighting with single texture support
 */

varying vec3 vPos;
varying vec3 vNormal;

void main()
{	
    
    vPos = (gl_ModelViewMatrix * gl_Vertex).xyz;
    vNormal = gl_NormalMatrix * gl_Normal;

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}