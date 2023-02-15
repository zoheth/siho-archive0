#include <siho/Material.h>

using namespace siho;

MaterialInstance* Material::createInstance(const char* name) const noexcept
{
	return MaterialInstance::duplicate(&mDefaultInstance, name);
}

