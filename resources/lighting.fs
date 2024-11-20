#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D texture0;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    vec3 viewDir = normalize(cameraPosition - fragPosition);

    // Ambient component
    vec3 ambient = vec3(0.2);

    // Diffuse component
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.8);

    // Specular component
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * vec3(0.5);

    // Combine components
    vec3 lighting = ambient + diffuse + specular;
    vec3 textureColor = texture(texture0, fragTexCoord).rgb;
    fragColor = vec4(textureColor * lighting, 1.0);
}