/*
 * Per-pixel lighting with single texture support
 */

uniform sampler2D colormap;

varying vec3 vPos;
varying vec3 vNormal;

const float MAX_DIST = 1.3;

void main()
{
    const vec3 posDiff = gl_LightSource[0].position.xyz - vPos;
    const float dist = length(posDiff);

    // Fragment's texture color
    const vec4 texture = texture2D( colormap, gl_TexCoord[0].st );

    if( dist > MAX_DIST )
    {
        // Out of light's reach
        gl_FragColor = ( gl_FrontMaterial.emission + gl_LightModel.ambient ) * texture;
        return;
    }

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
    gl_FragColor = ( gl_FrontMaterial.emission + gl_LightModel.ambient + ambient + diffuse + specular ) * texture;
}