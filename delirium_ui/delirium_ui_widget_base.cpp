
#include "delirium_ui.hpp"
#include<bits/stdc++.h> 
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

//-------------------------------------------------------------------------------------------



Delirium_UI_Widget_Base::Delirium_UI_Widget_Base()
{

		const char *homedir;

		if ((homedir = getenv("HOME")) == NULL)
		{
		    homedir = getpwuid(getuid())->pw_dir;
		}

		string triceratops_config_path = homedir;
 		string logo_file = triceratops_config_path + "/.config/triceratops/logo.png";
	
		surface_image = cairo_image_surface_create_from_png (logo_file.c_str());

}



//-------------------------------------------------------------------------------------------

Delirium_UI_Widget_Base::~Delirium_UI_Widget_Base()
{
}

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Base::Draw(cairo_t* cr)
{
	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	if (hover) {cairo_set_source_rgba(cr, 1,0,0,1); }
		else {cairo_set_source_rgba(cr, 0,0,0,1); }

	cairo_rectangle(cr, widget_x_position,widget_y_position,widget_width,widget_height);
	cairo_fill(cr);

}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Base::Left_Button_Press(int xm, int ym)
{
}

//-------------------------------------------------------------------------------------------
// USER PRESSED MIDDLE MOUSE BUTTON - RESET WIDGET TO DEFAULT VALUE

void Delirium_UI_Widget_Base::Middle_Button_Press()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Widget_Base::Mouse_Over(int xm, int ym)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE SCROLL WHEEL

void Delirium_UI_Widget_Base::Mouse_Scroll(float delta)
{

	if (type != deliriumUI_Knob)
	{
		if (min < max)
		{
			values[current_value] -= delta * increment;
			if (values[current_value] < min) values[current_value] = min;
			if (values[current_value] > max) values[current_value] = max;
		}
		else
		{
			values[current_value] += delta * increment;		
			if (values[current_value] > min) values[current_value] = min;
			if (values[current_value] < max) values[current_value] = max;
		}
	}
	else
	{
		if (min < max)
		{
			values[current_value] += delta * increment;
			if (values[current_value] < min) values[current_value] = min;
			if (values[current_value] > max) values[current_value] = max;
		}
		else
		{
			values[current_value] -= delta * increment;		
			if (values[current_value] > min) values[current_value] = min;
			if (values[current_value] < max) values[current_value] = max;
		}
	}


	Convert_Scaled_To_Value();
	
}

//----------------------------------------------------------------------------------------------------------------------------------------
// CONVERT NORMALISED 0~1 TO PARAMETERS MIN~MAX RANGE

void Delirium_UI_Widget_Base::Convert_Value_To_Scaled()
{
	float value = normalised_values[current_value];

	values[current_value] = (max-min)-(value * (max - min));

	if (min < 0) values[current_value] -= max;
	if (min > 0) values[current_value] += min;
}

//----------------------------------------------------------------------------------------------------------------------------------------
// CONVERT PARAMETERS MIN~MAX RANGE TO NORMALISED 0~1

void Delirium_UI_Widget_Base::Convert_Scaled_To_Value()

{
	normalised_values[current_value] = (values[current_value] - min) / (max - min);
	if (type == deliriumUI_Knob) { normalised_values[current_value] = 1-normalised_values[current_value]; }
	return;
}







