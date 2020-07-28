
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

void Delirium_UI_Widget_Base::Mouse_Over(int xm, int ym){
}

//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE SCROLL WHEEL

void Delirium_UI_Widget_Base::Mouse_Scroll(float delta)
{
	cout << delta << endl;
}




