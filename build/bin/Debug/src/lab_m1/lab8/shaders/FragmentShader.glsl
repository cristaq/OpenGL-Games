    #version 330

    // Input
    in vec3 world_position;
    in vec3 world_normal;

    // Uniforms for light properties
    uniform vec3 light_direction;
    uniform vec3 light_position;
    uniform vec3 light_position1;
    uniform vec3 light_direction1;
    uniform vec3 eye_position;

    uniform float material_kd;
    uniform float material_ks;
    uniform int material_shininess;

    // TODO(student): Declare any other uniforms
    uniform float cutoff_angle;
    uniform int type_of_light;

    uniform vec3 object_color;

    // Output
    layout(location = 0) out vec4 out_color;

    float point_light_contribution(vec3 light_position, vec3 light_direction)
{
    vec3 L = normalize( light_position - world_position);
        vec3 V = normalize( eye_position - world_position);
        vec3 H = normalize( L + V );
	float diffuse_light = material_kd * max(dot(world_normal, L), 0);
    float specular_light = 0;
        
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
    }

    float d	= distance(light_position, world_position);
	float attenuation_factor = 1.f / pow(d, 2);
    float light = 0;
    if(type_of_light == 0) {
        light = attenuation_factor * (specular_light + diffuse_light);
    } else {
        float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cutoff_angle);

        if (spot_light > spot_light_limit)
		{	 
			// Quadratic attenuation
			float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			light = attenuation_factor * light_att_factor * (diffuse_light + specular_light);
		}
    }
    return light;
}


    void main()
    {

        vec3 L = normalize( light_position - world_position);
        vec3 V = normalize( eye_position - world_position);
        vec3 H = normalize( L + V );

        // TODO(student): Define ambient, diffuse and specular light components
        float ambient_light = 0.25;
        float diffuse_light = material_kd * max(dot(world_normal, L), 0);
        float specular_light = 0;
        
        if (diffuse_light > 0)
        {
            specular_light = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
        }

        // TODO(student): If (and only if) the light is a spotlight, we need to do
        // some additional things.
        float d	= distance(light_position, world_position);
		float attenuation_factor = 1.f / pow(d, 2);
        float light = 0;
        if(type_of_light == 0) {
            light = ambient_light + attenuation_factor * (specular_light + diffuse_light);
        } else {
            float spot_light = dot(-L, light_direction);
		    float spot_light_limit = cos(cutoff_angle);

            if (spot_light > spot_light_limit)
		    {	 
			    // Quadratic attenuation
			    float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
			    float light_att_factor = pow(linear_att, 2);
			    light = ambient_light + attenuation_factor * light_att_factor * (diffuse_light + specular_light);
		    }
            else
            {
                light = ambient_light;
            }
        }

        light = 0;
        light += ambient_light + point_light_contribution(light_position, light_direction) + point_light_contribution(light_position1, light_direction1);

        // TODO(student): Write pixel out color
        out_color = vec4(object_color * light, 1);

    }
