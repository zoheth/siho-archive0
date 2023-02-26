#include <siho/Material.h>
#include <siho/MaterialInstance.h>

using namespace siho;

Material::Material(Engine& engine)
	:mEngine(engine)
{
	mShader = new Shader("shaders\\main.vs", "shaders\\main.fs");
}

MaterialInstance* Material::createInstance(const char* name) const noexcept
{
	return MaterialInstance::duplicate(&mDefaultInstance, name);
}

