#version 330

// Input
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_1;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 light_pos[100];
uniform int type[100];
uniform int numberOfLights;
uniform float cutoff_angle;

// Output
layout(location = 0) out vec4 out_color;

vec3 point_light_contribution(vec3 light_position, int type)
{
    vec3 light = vec3(0);
    vec3 color = vec3(1);

    if(type == 0) {
        color = vec3(1, 0, 0);
    } else if(type == 1){
        color = vec3(0, 1, 0);
    }
    
    if(type == 0 || type == 1) {
        light_position += vec3(0, 1, 0);
    } else if(type == 3) {
        light_position += vec3(0.5, 0.7, 0);
    } else if(type == 4) {
        light_position += vec3(-0.5, 0.7, 0);
    }
     vec3 L = normalize( light_position - world_position);
     vec3 V = normalize( eye_position - world_position);
     vec3 H = normalize( L + V );
	 vec3 diffuse_light = material_kd * color * max(dot(world_normal, L), 0);
     vec3 specular_light = vec3(0);
        
     if (diffuse_light != vec3(0)) {
        specular_light = material_ks * color * pow(max(dot(world_normal, H), 0), material_shininess);
     }

     float d = distance(light_position, world_position);
	 vec3 attenuation_factor = vec3(1.f / pow(d, 2));

    if(type == 0 || type == 1) {
        light = attenuation_factor * (specular_light + diffuse_light);
        float d = distance(light_position, world_position);
	    vec3 attenuation_factor = vec3(1.f / pow(d, 2));

    } else {
        float spot_light = dot(-L, vec3(0, -1, 0));
		float spot_light_limit = cos(cutoff_angle);

        if (spot_light > spot_light_limit)
		{	 
			// Quadratic attenuation
			float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			light = light_att_factor * (diffuse_light + specular_light);
		}
    }

    return light;
}

void main()
{
    vec4 color1 = texture2D(texture_1, texcoord);
    
    
    vec3 L = normalize(vec3(0, 3, 10));
    vec3 V = normalize( eye_position - world_position);
    vec3 H = normalize( L + V );
    
    vec3 diffuse_light = material_kd * vec3(1) * max(dot(world_normal, L), 0);
    vec3 specular_light = vec3(0);
       
    if (diffuse_light != vec3(0))
    {
        specular_light = material_ks * vec3(1) * pow(max(dot(world_normal, H), 0), material_shininess);
    }
    vec3 light = diffuse_light + specular_light;
    
    for(int i = 0; i < numberOfLights; i++) {
        light += point_light_contribution(light_pos[i], type[i]);
    }

    out_color = color1;
    if(out_color.a < .5f) {
        discard;
    }
    out_color *= vec4(light, 1);
    
}
