#include <siho/LightManager.h>

UniformObject LightManager::GetLightData() const
{
    UniformObject data;
    for (size_t i = 0; i < m_pointLights.size(); i++) {
        std::string index_str = std::to_string(i);
        data.set("pointLights[" + index_str + "].position", m_pointLights[i].position_);
        data.set("pointLights[" + index_str + "].ambient", m_pointLights[i].ambient_);
        data.set("pointLights[" + index_str + "].diffuse", m_pointLights[i].diffuse_);
        data.set("pointLights[" + index_str + "].specular", m_pointLights[i].specular_);
        data.set("pointLights[" + index_str + "].constant", m_pointLights[i].constant_);
        data.set("pointLights[" + index_str + "].linear", m_pointLights[i].linear_);
        data.set("pointLights[" + index_str + "].quadratic", m_pointLights[i].quadratic_);
    }
    return data;
}