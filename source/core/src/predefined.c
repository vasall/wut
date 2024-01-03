#include "core/inc/predefined.h"


const u8 fh_ps_num = 3;
char *fh_ps_lst[3][3] = {
	{"flat", "res/predef/shaders/flat.vert", "res/predef/shaders/flat.frag"},
	{"ui", "res/predef/shaders/ui.vert", "res/predef/shaders/ui.frag"},
	{"model", "res/predef/shaders/model.vert", "res/predef/shaders/model.frag"}
};



char *fh_ps_batch_vshader = 
"#version 420 core\n"
"layout(location=0) in vec3 in_position;\n"
"layout(location=1) in int in_index;\n"
""
""
"uniform ivec2   u_frame;"
"uniform ivec4   u_rect[200];"
"uniform vec4    u_color[200];"
"uniform ivec4   u_radius[200];"
"uniform int     u_bwidth[200];"
"uniform vec4    u_bcolor[200];"
""
"flat out vec2   fs_frame;"
"out vec4        fs_color;"
"flat out vec4   fs_sides;"
"flat out vec2   fs_cpoints[4];"
"flat out ivec4  fs_radius;"
"flat out int    fs_bwidth;"
"flat out vec4   fs_bcolor;"
""
"void main() {"
"    vec2 lframe = u_frame * 0.5;"
"    ivec4 lrect = u_rect[in_index];"
"    ivec4 lradius = u_radius[in_index];"
""
"    fs_frame = ivec2(800, 600);"
"    fs_color = u_color[in_index];"
"    fs_sides = vec4(lrect.y, lrect.x+lrect.z, lrect.y+lrect.w, lrect.x);"
"    fs_cpoints = vec2[4]("
"        vec2(lrect.x + lradius[0], lrect.y + lradius[0]),"
"        vec2(lrect.x + lrect.z - lradius[1], lrect.y + lradius[1]),"
"        vec2(lrect.x + lrect.z - lradius[2], lrect.y + lrect.w - lradius[2]),"
"        vec2(lrect.x + lradius[3], lrect.y + lrect.w - lradius[3])"
"    );"
"    fs_radius = lradius;"
"    fs_bwidth = u_bwidth[in_index];"
"    fs_bcolor = vec4(0.0, 0, 0, 1.0);"
""
"    gl_Position = vec4((in_position.x - lframe.x) / lframe.x, (lframe.y - in_position.y) / lframe.y, in_position.z, 1);"
"}";

char *fh_ps_batch_fshader = 
"#version 420 core\n"
""
"flat in vec2    fs_frame;"
"in vec4         fs_color;"
"flat in vec4    fs_sides;"
"flat in vec2    fs_cpoints[4];"
"flat in ivec4   fs_radius;"
"flat in int     fs_bwidth;"
"flat in vec4    fs_bcolor;"
""
"out vec4 out_color;"
""
"void main() {"
"    int corner = -1;"
"    vec2 loc = vec2(gl_FragCoord.x, fs_frame.y - gl_FragCoord.y);"
"    float alpha = 1.0;"
"    vec4 color;"
"    float dist;"
""
"	int scrollbar_width = 10;"
"	int scrollbar_thumb_position = 0;"
"	int scrollbar_thumb_size = 100;"
""
"	if(loc.y > fs_sides[0] + fs_bwidth && loc.x < fs_sides[1] - fs_bwidth && loc.y < fs_sides[2] - fs_bwidth && loc.x > fs_sides[1] - fs_bwidth - scrollbar_width) {"
"		if(loc.y > fs_sides[0] + fs_bwidth + 1 + scrollbar_thumb_position && loc.x < fs_sides[1] - fs_bwidth - 1 && loc.y < fs_sides[0] + scrollbar_thumb_position + scrollbar_thumb_size && loc.x > fs_sides[1] - fs_bwidth - scrollbar_width + 1) {"
"			color = vec4(0, 0, 0, 1);"
"		}"
"		else {"
"			color = vec4(1, 0, 1, 1);"
"		}"
"	}"
"	else {"
"		if(loc.x <= fs_cpoints[0].x && loc.y <= fs_cpoints[0].y) corner = 0;"
"   	else if(loc.x >= fs_cpoints[1].x && loc.y <= fs_cpoints[1].y) corner = 1;"
"   	else if(loc.x >= fs_cpoints[2].x && loc.y >= fs_cpoints[2].y) corner = 2;"
"   	else if(loc.x <= fs_cpoints[3].x && loc.y >= fs_cpoints[3].y) corner = 3;"
""
"   	if(corner < 0) {"
"   		if(loc.y <= fs_sides[0] + fs_bwidth || loc.x >= fs_sides[1] - fs_bwidth || loc.y >= fs_sides[2] - fs_bwidth || loc.x <= fs_sides[3] + fs_bwidth) {"
"				color = fs_bcolor;"
"			}"
"   	 	else {"
"				color = fs_color;"
"			}"
"   	}"
"    	else {"
"			dist = distance(loc, fs_cpoints[corner]);"
"			if(dist >= fs_radius[corner]) alpha = 0;"
"			else if(dist >= (fs_radius[corner] - fs_bwidth)) color = fs_bcolor;"
"			else color = fs_color;"
"		}"
"	}"
""
"	out_color = vec4(color.xyz, color.w * alpha);"
"};";
