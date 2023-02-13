#include <siho/Camera.h>
#include <sihoapp/SihoApp.h>

#include "sihoapp/MeshAssimp.h"

using namespace siho;

static Scene* g_scene = nullptr;

std::unique_ptr<MeshAssimp> g_mesh_set;

static Config g_config;


static void cleanup(Engine* engine, View*, Scene*);
static void setup(Engine* engine, View*, Scene* scene)
{
	g_scene = scene;
	g_mesh_set = std::make_unique<MeshAssimp>(*engine);
}
static void gui(Engine* engine, View*);
static void preRender(Engine* engine, View*, Scene*, Renderer*);

int main(int argc, char *argv[])
{
	SihoApp siho_app;
	siho_app.run(g_config, setup, cleanup, gui, preRender);
}