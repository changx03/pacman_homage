#version 410

subroutine vec4 subColor();
subroutine uniform subColor Color;

uniform vec3 modelColor;
uniform sampler2D mTexture;

uniform mat4 modelMat;
uniform vec3 lightPos_eye;	// light position in eye coordinates
uniform vec3 ambientCoefficient = vec3(0.3);
uniform vec3 lightIntensity = vec3(1.0);	// color of the light
uniform vec3 specularColor = vec3(0.6);
uniform float specularPower = 0.9;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 finalColor;

subroutine(subColor)
vec4 UseModelColor(){
	return vec4(modelColor, 1.0);
}

subroutine(subColor)
vec4 UseTexture(){
	return texture(mTexture, fragTexCoord);
}

void main(){
	vec4 surfaceColor = Color();
	
		// raise the vertex position and normal vector to eye space
	mat3 normalMatrix = transpose(inverse(mat3(modelMat)));
	vec4 pos_eye = modelMat * vec4(fragVert, 1.0);
	vec3 norm_eye = normalMatrix * fragNormal;

	//calculate the light and view direction vectors
	vec3 light_eye = lightPos_eye - pos_eye.xyz;
	vec3 view_eye = -pos_eye.xyz;

	//normalise the direction vectors
	norm_eye = normalize(norm_eye);
	light_eye = normalize(light_eye);
	view_eye = normalize(view_eye);

	// calculate ambient
	vec3 ambient = ambientCoefficient  * surfaceColor.rgb;
	
	//calculate the diffuse intensity
	float diffuseCoefficient = max(dot(light_eye, norm_eye), 0.0);
	vec3 diffuse = diffuseCoefficient * surfaceColor.rgb;

	//calculate the specular intensity
	float specularCoefficient = 0.0;
	vec3 refect_eye = reflect(-light_eye, norm_eye);
	specularCoefficient = pow(max(dot(view_eye, refect_eye), 0.0), specularPower);
	//vec3 specular = specularCoefficient * specularColor;
	vec3 specular = specularCoefficient * (surfaceColor.rgb * 1.3);
	
	// Phong model
	vec3 color = (ambient + diffuse + specular) * lightIntensity;
	
	//vec3 gamma = vec3(1.0/2.2);
	//finalColor = vec4(pow(color, gamma), 1.0);
	finalColor = vec4(color, surfaceColor.a);
}