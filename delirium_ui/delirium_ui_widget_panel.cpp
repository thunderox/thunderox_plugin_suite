
#include "delirium_ui.hpp"

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Panel::Draw(cairo_t* cr) 
{
	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	float r = 0.1;
	float g = 0;
	float b = 0;

	cairo_text_extents_t extents;
	int font_size = 14;
	cairo_set_font_size(cr, font_size);
	cairo_text_extents(cr, label.c_str(), &extents);
	float x_text_centred = (widget_x_position + widget_width / 2) - extents.width / 2;

	cairo_set_source_rgba(cr, 0.1,0.1,0.1,1);
	cairo_rectangle(cr, widget_x_position-1, widget_y_position-font_size, widget_width+2, widget_height+font_size);
	cairo_fill(cr);

	cairo_pattern_t *linpat;
	linpat = cairo_pattern_create_linear (0,widget_y_position,0,widget_y_position+widget_height);
	cairo_pattern_add_color_stop_rgb(linpat, 0, r+0.2,g,b);
	cairo_pattern_add_color_stop_rgb(linpat, 1, r,g,b);
	cairo_set_source(cr, linpat);

	cairo_rectangle(cr, widget_x_position,widget_y_position,widget_width,widget_height);
	cairo_fill(cr);

	/*

	cairo_rectangle(cr, x_text_centred-(font_size/2)+2, widget_y_position-font_size, extents.width + font_size, font_size);
	cairo_fill(cr);

	cairo_set_source_rgba(cr, 0, 0, 0, 0.75);

	cairo_move_to(cr,widget_x_position, widget_y_position);
	cairo_line_to(cr, widget_x_position + x_text_centred-(font_size/4), widget_y_position);
	cairo_move_to(cr, widget_x_position + x_text_centred + extents.width - font_size , widget_y_position);
	cairo_line_to(cr, widget_x_position + widget_width,widget_y_position);
	cairo_line_to(cr, widget_x_position + widget_width, widget_y_position + widget_height);
	cairo_line_to(cr, widget_x_position, widget_y_position + widget_height);
	cairo_line_to(cr, widget_x_position, widget_y_position);
	cairo_move_to(cr, (widget_x_position + x_text_centred + extents.width) - font_size, widget_y_position);
	cairo_line_to(cr, (widget_x_position + x_text_centred + extents.width) - font_size, widget_y_position - font_size);
	cairo_line_to(cr, widget_x_position + x_text_centred - (font_size/2) + 3, widget_y_position - font_size);
	cairo_line_to(cr, widget_x_position + x_text_centred - (font_size/2) + 3, widget_y_position);
	cairo_stroke(cr);

	*/

	// -- text

	cairo_set_source_rgba(cr, 1,1,1,0.75);
	cairo_move_to(cr,x_text_centred, widget_y_position-1);
	cairo_show_text(cr, label.c_str());
}


//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Panel::Left_Button_Press(int xm, int ym)
{
}



