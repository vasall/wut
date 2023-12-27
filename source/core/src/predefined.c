#include "core/inc/predefined.h"


const u8 fh_ps_num = 3;
char *fh_ps_lst[3][3] = {
	{"flat", "res/predef/shaders/flat.vert", "res/predef/shaders/flat.frag"},
	{"ui", "res/predef/shaders/ui.vert", "res/predef/shaders/ui.frag"},
	{"model", "res/predef/shaders/model.vert", "res/predef/shaders/model.frag"}
};



char *fh_ps_batch_vshader = 
"#version 420 core\n"
"layout(location=0) in ivec2 in_position;\n"
"layout(location=1) in vec4 in_color;\n"
"out vec4 fs_color;"
"void main() {\n"
"    vec2 t = vec2(800, 600) * 0.5;\n"
"    fs_color = in_color;\n"
"    gl_Position = vec4((in_position.x - t.x) / t.x, (t.y - in_position.y) / t.y, 0, 1);\n"
"}\0";

char *fh_ps_batch_fshader = 
"#version 420 core\n"
"in vec4 fs_color;\n"
"out vec4 m;         /* color output */\n"
"void main() {\n"
"    m = fs_color;\n"
"};\0";
