#include <siho/Material.h>

using namespace siho;

Material::Material(Engine& engine)
	:mEngine(engine), mDefaultInstance(engine, this, "default")
{
	mShader = new Shader("shaders\\common.vs", "shaders\\common.fs");
}

MaterialInstance* Material::createInstance(const char* name) const noexcept
{
	return MaterialInstance::duplicate(&mDefaultInstance, name);
}

