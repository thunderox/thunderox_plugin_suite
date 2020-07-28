
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Logo::Draw(cairo_t* cr)
{
	cairo_surface_t *surface_image;

	const char *homedir;

	if ((homedir = getenv("HOME")) == NULL)
	{
	    homedir = getpwuid(getuid())->pw_dir;
	}

	string triceratops_config_path = homedir;
 	string logo_file = triceratops_config_path + "/.config/triceratops/logo.png";

	surface_image = cairo_image_surface_create_from_png (logo_file.c_str());

	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	float x = widget_x_position;
	float y = widget_y_position;
	float w = widget_width;
	float h = widget_height;

	cairo_save(cr);
	cairo_translate(cr, widget_x_position, widget_y_position);
	cairo_scale(cr,1.0/(x_grid_size/width),1.0/(y_grid_size/height));
	cairo_set_source_surface(cr, surface_image,0,0);
	cairo_paint(cr); 
	cairo_restore(cr);
}


//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Logo::Left_Button_Press(int xm, int ym)
{
}



