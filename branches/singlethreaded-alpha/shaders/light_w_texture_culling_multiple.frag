/*
 * Per-pixel lighting with single texture support
 */

uniform sampler2D colormap;

varying vec3 vPos;
varying vec3 vNormal;

const float MAX_DIST   = 0.5;
const int   MAX_LIGHTS = 6;

void main()
{
    const vec3 N = normalize( vNormal );
    const vec3 V = normalize( -vPos );

    // Fragment's texture color
    const vec4 texture = texture2D( colormap, gl_TexCoord[0].st );

    //vec4 totalAmbient, totalDiffuse, totalSpecular;

    for( int i=0; i<MAX_LIGHTS; ++i )
    {
        const vec3 posDiff = gl_LightSource[i].position.xyz - vPos;
        const float dist = length(posDiff);

        if( dist > MAX_DIST )
        {
            // Out of light's reach
            gl_FragColor += ( gl_FrontMaterial.emission + gl_LightModel.ambient ) * texture;
            continue;
        }

        const float ratio = 1.0 - dist / MAX_DIST;

        // Light vectors
        const vec3 L = normalize( posDiff );
        const vec3 R = reflect( -L, N );

        // Fragment's color
        const vec4 ambient  = gl_LightSource[i].ambient  * gl_FrontMaterial.ambient;
        const vec4 diffuse  = gl_LightSource[i].diffuse  * gl_FrontMaterial.diffuse  * max( 0.0, dot( N, L ) );
        const vec4 specular = gl_LightSource[i].specular * gl_FrontMaterial.specular * pow( max( 0.0, dot( R, V ) ), gl_FrontMaterial.shininess );

        const vec4 finalColor = ( ambient + diffuse + specular ) * ratio;

        // Final color includes fragment's colors and textures, self-emissions and global ambient
        gl_FragColor += ( gl_FrontMaterial.emission + gl_LightModel.ambient + finalColor ) * texture;
    }
}