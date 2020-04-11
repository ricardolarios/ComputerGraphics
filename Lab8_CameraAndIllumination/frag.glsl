#version 330

// Take in our texture coordinate from our vertex shader
in vec2 texCoords;
// And our normal
in vec3 norm;
// And our fragment position for lighting
in vec3 fragPos;

// We always define a fragment color that we output.
out vec4 fragColor;

// Define our light(s)
struct PointLight {
    vec3 color;
    vec3 position;
    float ambientIntensity;
    float specularIntensity;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Maintain our uniforms.
uniform sampler2D tex;              // our primary texture
uniform mat4 view;                  // we need the view matrix for highlights
uniform PointLight pointLights[3];  // Our lights

void main() {
  // Set our output fragment color to whatever we pull from our input texture (Note, change 'tex' to whatever the sampler is named)
  fragColor = texture(tex, texCoords);

  // TODO:  Implement some form of lighting.
   // Compute the normal direction
    vec3 my_norm = normalize(norm);

    vec3 diffuse_color;
    diffuse_color = texture(tex, texCoords).rgb;

    // TODO: Figure out how to actually make this work with multiple lights
    //for (int i = 0; i < 3; i++){}

    // ambient light
    vec3 ambient = pointLights[0].ambientIntensity * pointLights[0].color;

    // diffuse
    vec3 light_dir = normalize(pointLights[0].position - fragPos);
    float diff_impact = max(dot(norm, light_dir), 0.0);
    vec3 diffuse_lighting = diff_impact * pointLights[0].color;

    vec3 view_pos = vec3(0.0, 0.0, 0.0);
    vec3 view_dir = normalize(view_pos - fragPos);
    vec3 reflectDir = reflect(-light_dir, norm);

    float spec = pow(max(dot(view_dir, reflectDir), 0.0), 32);
    vec3 specular = pointLights[0].specularIntensity * spec * pointLights[0].color;

    // Attenuation
    // Distance and lighting

    // Get lighting
    vec3 lighting = diffuse_lighting + ambient + specular;

    if(gl_FrontFacing)
    {
        fragColor = vec4(diffuse_color * lighting, 1.0);
    }
    else
    {
        fragColor = vec4(diffuse_color * lighting, 1.0);
    }
}