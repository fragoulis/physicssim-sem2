/*
 * Per-pixel lighting with single texture support
 */

uniform sampler2D colormap;

varying vec3 vPos;
varying vec3 vNormal;

void main()
{
    const vec3 posDiff = gl_LightSource[0].position.xyz - vPos;
    const float dist = length(posDiff);

    // Fragment's attenuation
    //const float att = 1.0 / gl_LightSource[0].constantAttenuation  + 
	   //                     gl_LightSource[0].linearAttenuation    * dist + 
	   //                     gl_LightSource[0].quadraticAttenuation * dist * dist;

    // Fragment's texture color
    const vec4 texture = texture2D( colormap, gl_TexCoord[0].st );

    // Light vectors
    const vec3 L = normalize( posDiff );
    const vec3 N = normalize( vNormal );

    const vec3 R = reflect( -L, N );
    const vec3 V = normalize( -vPos );

    // Fragment's color
    const vec4 ambient  = gl_LightSource[0].ambient  * gl_FrontMaterial.ambient;
    const vec4 diffuse  = gl_LightSource[0].diffuse  * gl_FrontMaterial.diffuse  * max( 0.0, dot( N, L ) );
    const vec4 specular = gl_LightSource[0].specular * gl_FrontMaterial.specular * pow( max( 0.0, dot( R, V ) ), gl_FrontMaterial.shininess );

    // Final color includes fragment's colors and textures, self-emissions and global ambient
    //gl_FragColor = ( gl_LightModel.ambient + ambient + diffuse ) * texture + specular * texture;
    gl_FragColor = ( gl_LightModel.ambient + ambient + diffuse + specular ) * texture;
}