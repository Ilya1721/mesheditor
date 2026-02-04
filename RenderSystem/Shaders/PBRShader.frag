#version 330 core

out vec4 fragColor;

in VS_OUT {
    vec3 fragPos;
    vec2 textureCoords;
    mat3 TBN;
} vsOut;

uniform sampler2D baseColorTexture;
uniform sampler2D normalTexture;
uniform sampler2D metallicRoughnessTexture;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

const float PI = 3.14159265359;

vec3 getNormalFromTexture()
{
    vec3 tangentNormal = texture(normalTexture, vsOut.textureCoords).xyz * 2.0 - 1.0;
    return normalize(vsOut.TBN * tangentNormal);
}

float distributionGGX(vec3 surfaceNormal, vec3 viewLightHalfVector, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH  = max(dot(surfaceNormal, viewLightHalfVector), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);

    return a2 / (PI * denom * denom);
}

float geometrySchlickGGX(float NdotDir, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    return NdotDir / (NdotDir * (1.0 - k) + k);
}

float geometrySmith(vec3 surfaceNormal, vec3 viewDir, vec3 lightDir, float roughness)
{
    float ggx1 = geometrySchlickGGX(max(dot(surfaceNormal, viewDir), 0.0), roughness);
    float ggx2 = geometrySchlickGGX(max(dot(surfaceNormal, lightDir), 0.0), roughness);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float normalHalfViewLightCos, vec3 baseReflectivity)
{
    return baseReflectivity + (1.0 - baseReflectivity) * pow(1.0 - normalHalfViewLightCos, 5.0);
}

void main()
{
    vec3 albedo = pow(texture(baseColorTexture, vsOut.textureCoords).rgb, vec3(2.2));
    vec2 mr = texture(metallicRoughnessTexture, vsOut.textureCoords).bg;
    float metallic = mr.x;
    float roughness = mr.y;

    vec3 N = getNormalFromTexture();
    vec3 V = normalize(cameraPos - vsOut.fragPos);
    vec3 L = normalize(lightPos - vsOut.fragPos);
    vec3 H = normalize(V + L);

    float distToLightPos = length(lightPos - vsOut.fragPos);
    float lightScale = 1.0 / (distToLightPos * distToLightPos);
    vec3 radiance = lightColor * lightScale;

    vec3 baseReflectivity = mix(vec3(0.04), albedo, metallic);
    float D = distributionGGX(N, H, roughness);
    float G = geometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), baseReflectivity);

    vec3 numerator = D * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    vec3 diffuse = (vec3(1.0) - F) * (1.0 - metallic);
    float NdotL = max(dot(N, L), 0.0);
    vec3 reflectedRadiance = (diffuse * albedo / PI + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedo;
    vec3 color = ambient + reflectedRadiance;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    fragColor = vec4(color, 1.0);
}
