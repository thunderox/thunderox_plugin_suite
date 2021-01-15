
#include "delirium_ui.hpp"

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Filter::Draw(cairo_t* cr) 
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;

	cairo_text_extents_t extents;
 
	cairo_set_font_size(cr, font_size);
	cairo_text_extents(cr, label.c_str(), &extents);
	float x_text_centred = (wX + wW / 2) - extents.width / 2;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, wX - 1, wY - (font_size * 1.5), wW + 2, wH + (font_size * 1.5) );
	cairo_fill(cr);

	cairo_set_source_rgb(cr, 0.1,0,0);
	cairo_rectangle(cr, wX, wY + (font_size * 0.5), wW, wH);
	cairo_fill(cr);
	
	// -- draw filter waveform
	
	cairo_set_source_rgb(cr, 0.6,0.6,0.6);
	cairo_move_to(cr, wX + 5, wY + (wH/1.5));
	cairo_curve_to(cr, wX + 5, wY + (wH/1.5), wX + (wW * filter_frequency), wY - (wH/3), (wX + wW) - 10, wY + (wH/1.5)  );
	// cairo_curve_to(cr, wX + (wW - (wW / cutoff_frequency)), wY + (wH - (wH / 3.5)), wX + (wW - (wW / 8)), wY + (wH - (wH / 3.5)), wX + (wW - (wW / 8)), wY + (wH - (wH / 3.5)));
	cairo_stroke(cr);

	// -- text

	cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	cairo_move_to(cr,x_text_centred, wY - 1);
	cairo_show_text(cr, label.c_str());
}


//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Filter::Left_Button_Press(int xm, int ym)
{
}



