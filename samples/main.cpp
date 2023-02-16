#include <glad/glad.h>
#include <siho/Camera.h>
#include <sihoapp/SihoApp.h>

#include "sihoapp/MeshAssimp.h"

using namespace siho;

struct Parameters {
	utils::Entity light;
};

static std::vector<std::string> g_filenames;

static Scene* g_scene = nullptr;

std::unique_ptr<MeshAssimp> g_mesh_set;
static std::map<std::string, MaterialInstance*> g_meshMaterialInstances;
static Parameters g_params;

static Config g_config;



static void cleanup(Engine* engine, View*, Scene*)
{
    ;
}
static void setup(Engine* engine, View*, Scene* scene)
{
	g_scene = scene;
	g_mesh_set = std::make_unique<MeshAssimp>(*engine);
	for (auto& filename : g_filenames) {
		g_mesh_set->addFromFile(filename, g_meshMaterialInstances);
	}

	for(auto renderable: g_mesh_set->getRenderables())
	{
		scene->addEntity(renderable);
	}
	scene->addEntity(g_params.light);
}

static void gui(Engine* engine, View*);
static void preRender(Engine* engine, View*, Scene*, Renderer*);

int main(int argc, char *argv[])
{
	SihoApp siho_app;
	siho_app.run(g_config, setup, cleanup, gui, preRender);
}