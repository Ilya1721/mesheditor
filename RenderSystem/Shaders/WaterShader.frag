#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexUV;

out vec4 fragColor;

const int MAX_NORMAL_MAP_MOVES = 2;

uniform samplerCube skybox;
uniform sampler2D normalMap;
uniform vec2 normalMapMoves[MAX_NORMAL_MAP_MOVES];
uniform vec3 shallowColor;
uniform vec3 deepColor;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float fTime;
uniform float normalStrength;
uniform float depthFalloff;
uniform float fresnelPower;
uniform float reflectionIntensity;
uniform float minFresnel;
uniform float shininess;
uniform int normalMapMoveCount;

void main()
{
    vec3 tangent = vec3(1, 0, 0);
    vec3 bitangent = vec3(0, 0, 1);
    vec3 normalWS = vec3(0, 1, 0);
    mat3 TBN = mat3(tangent, bitangent, normalWS);

    vec3 combinedNormalTS = vec3(0.0);
    for (int moveIdx = 0; moveIdx < normalMapMoveCount; moveIdx++)
    {
        vec2 uv = vertexUV + fTime * normalMapMoves[moveIdx];
        vec3 normal = texture(normalMap, uv).xyz * 2.0 - 1.0;
        combinedNormalTS += normal;
    }

    combinedNormalTS = normalize(combinedNormalTS);
    vec3 combinedNormalWS = normalize(TBN * combinedNormalTS);
    vec3 normal = normalize(mix(vec3(0,1,0), combinedNormalWS, normalStrength));
    vec3 viewDir = normalize(cameraPos - vertexPos);
    vec3 lightDir = normalize(lightPos - vertexPos);

    float viewDirNormalAlignment = max(dot(viewDir, normal), 0.0);
    float baseFresnel = pow(1.0 - viewDirNormalAlignment, fresnelPower);
    float fresnel = max(baseFresnel, minFresnel);
    float reflectionMixFactor = fresnel * reflectionIntensity;
    vec3 cameraSurfaceReflectDir = reflect(-viewDir, normal);
    vec3 skyboxReflection = texture(skybox, cameraSurfaceReflectDir).rgb;
    vec3 lightSurfaceReflectDir = reflect(-lightDir, normal);
    float viewDirLightReflectAlignment = max(dot(viewDir, lightSurfaceReflectDir), 0.0);
    float specular = pow(viewDirLightReflectAlignment, shininess);
    float depthFactor = clamp(exp(-vertexPos.y * depthFalloff), 0.0, 1.0);

    vec3 waterBaseColor = mix(shallowColor, deepColor, depthFactor);
    vec3 waterWithReflection = mix(waterBaseColor, skyboxReflection, reflectionMixFactor);
    vec3 finalColor = waterWithReflection + lightColor * specular;
    fragColor = vec4(finalColor, 1.0);
}