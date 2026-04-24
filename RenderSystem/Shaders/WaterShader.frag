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
uniform float fTime;
uniform float normalStrength;
uniform float depthFalloff;
uniform float fresnelPower;
uniform float reflectionIntensity;
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

    float fresnel = pow(1.0 - max(dot(viewDir, normal), 0.0), fresnelPower);
    vec3 reflectDir = reflect(-viewDir, normal);
    vec3 reflection = texture(skybox, reflectDir).rgb;

    float depthFactor = clamp(exp(-vertexPos.y * depthFalloff), 0.0, 1.0);
    vec3 waterColor = mix(shallowColor, deepColor, depthFactor);
    vec3 finalColor = mix(waterColor, reflection, fresnel * reflectionIntensity);

    fragColor = vec4(finalColor, 1.0);
}