#include <siho/Camera.h>
#include <sihoapp/SihoApp.h>

using namespace siho;

static Config g_config;

static void cleanup(Engine* engine, View*, Scene*);
static void setup(Engine* engine, View*, Scene*);
static void gui(Engine* engine, View*);
static void preRender(Engine* engine, View*, Scene*, Renderer*);

int main(int argc, char *argv[])
{
	SihoApp siho_app;
	siho_app.run(g_config, setup, cleanup, gui, preRender);
}