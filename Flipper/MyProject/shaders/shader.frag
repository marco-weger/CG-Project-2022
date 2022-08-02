#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragViewDir;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

/*
vec3 direct_light_dir(vec3 pos) {
    // Directional light direction
    return gubo.lightDir;
}

vec3 direct_light_color(vec3 pos) {
    // Directional light color
    return gubo.lightColor;
}

vec3 point_light_dir(vec3 pos) {
    // Point light direction
    return normalize(gubo.lightPos - pos);
}

vec3 point_light_color(vec3 pos) {
    // Point light color
    // g = gubo.coneInOutDecayExp.z
    // b = gubo.coneInOutDecayExp.w
    
    return gubo.lightColor * pow(vec3(gubo.coneInOutDecayExp.z) / length(gubo.lightPos - pos), vec3(gubo.coneInOutDecayExp.w));
}

vec3 spot_light_dir(vec3 pos) {
    // Spot light direction
    return normalize(gubo.lightPos - pos);
}

vec3 spot_light_color(vec3 pos) {
    // Spot light color
    // g = gubo.coneInOutDecayExp.z
    // b = gubo.coneInOutDecayExp.w
    // c_in = gubo.coneInOutDecayExp.y
    // c_out = gubo.coneInOutDecayExp.x
    
    float dotRet = dot(normalize(gubo.lightPos - pos), gubo.lightDir);
    float clampPar = (dotRet - gubo.coneInOutDecayExp.x) / (gubo.coneInOutDecayExp.y - gubo.coneInOutDecayExp.x);
    float clampRet = clamp(clampPar, 0, 1);
        
    return gubo.lightColor * pow(vec3(gubo.coneInOutDecayExp.z) / length(gubo.lightPos - pos), vec3(gubo.coneInOutDecayExp.w)) * vec3(clampRet);
}
*/

void main() {
	const vec3  diffColor = texture(texSampler, fragTexCoord).rgb;
	const vec3  specColor = vec3(1.0f, 1.0f, 1.0f);
	const float specPower = 150.0f;
	const vec3  L1 = vec3(-0.4830f, 0.8365f, -0.2588f);
    const vec3  L2 = vec3(-0.6830f, 1.3365f, -0.1588f);
    
	vec3 N1 = normalize(fragNorm);
	vec3 R = -reflect(L1, N1);
	vec3 V = normalize(fragViewDir);
	
    vec3 N2 = normalize(vec3(0,20,-100));
                        
	// Lambert diffuse
    vec3 diffuse = 0.1 * diffColor * max(dot(N1,L1), 0.0f) + 0.7 * diffColor * max(dot(N2,L2), 0.0f);
    //diffuse = diffColor * max(dot(N1,L1), 0.0f);
	// Phong specular
	vec3 specular = specColor * pow(max(dot(R,V), 0.0f), specPower);
	// Hemispheric ambient
	vec3 ambient  = (vec3(0.1f,0.1f, 0.1f) * (1.0f + N1.y) + vec3(0.0f,0.0f, 0.1f) * (1.0f - N1.y)) * diffColor;
	
	outColor = vec4(clamp(ambient + diffuse + specular, vec3(0.0f), vec3(1.0f)), 1.0f);
}
