
#include "delirium_ui.hpp"

//-------------------------------------------------------------------------------------------



Delirium_UI_Widget_Base::Delirium_UI_Widget_Base()
{
	cairo_surface_t *surface;
	surface = cairo_image_surface_create_from_png ("/home/cypher_thor/programming/audio/synths/triceratops-v1/delirium_ui/logo.png");
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
			scaled_value -= delta * increment;
			if (scaled_value < min) scaled_value = min;
			if (scaled_value > max) scaled_value = max;
		}
		else
		{
			scaled_value += delta * increment;		
			if (scaled_value > min) scaled_value = min;
			if (scaled_value < max) scaled_value = max;
		}
	}
	else
	{
		if (min < max)
		{
			scaled_value += delta * increment;
			if (scaled_value < min) scaled_value = min;
			if (scaled_value > max) scaled_value = max;
		}
		else
		{
			scaled_value -= delta * increment;		
			if (scaled_value > min) scaled_value = min;
			if (scaled_value < max) scaled_value = max;
		}
	}


	Convert_Scaled_To_Value();
	
}

//----------------------------------------------------------------------------------------------------------------------------------------
// CONVERT 0..1 TO MIN MAX RANGE

void Delirium_UI_Widget_Base::Convert_Value_To_Scaled()
{

	float value = values[current_value];

	scaled_value = (value * (max - min));
	scaled_value = (max-min)-(value * (max - min));

	if (min < 0) scaled_value -= max;
	if (min > 0) scaled_value += min;


}

//----------------------------------------------------------------------------------------------------------------------------------------
// CONVERT MIN MAX RANGE TO VALUE 0..1

void Delirium_UI_Widget_Base::Convert_Scaled_To_Value()
{

	if (min > 0) scaled_value  -= min;
	
	float value = scaled_value / ((max - min));

	if ( min < 0) value += 0.5;

	if (type == deliriumUI_Knob) { value = 1-value; }


	values[current_value] = value;
}




