
#include "delirium_ui.hpp"

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Filter::Draw(cairo_t* cr) 
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;

	cairo_set_source_rgb(cr, 0.15,0,0);
	cairo_rectangle(cr, wX, wY + (font_size * 0.5), wW, wH);
	cairo_fill(cr);
	
	// -- draw filter waveform
	
	float filter_freq_x_offset = (filter_frequency * 20) - 10;
	
	cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
	cairo_move_to(cr, wX + 5, wY + (wH/1.5));
	cairo_curve_to(cr, wX + 5, wY + (wH / 1.5), wX + (wW / 3) + filter_freq_x_offset, wY + (wH / 3)  + filter_freq_x_offset, (wX + (wW / 2)) + filter_freq_x_offset , wY + (wH / 4)  );	
	cairo_line_to(cr, wX + (wW / 2) + 2.5 + filter_freq_x_offset, wY + (wH / 4) - (filter_resonance * 16));
	cairo_line_to(cr, wX + (wW / 2) + 5 + filter_freq_x_offset , wY + (wH / 3));
	cairo_curve_to(cr,  wX + (wW / 2) + 5 + filter_freq_x_offset, wY + (wH / 3), wX + (wW / 2.2) + 5 + filter_freq_x_offset, wY + (wH / 3), (wX + wW) - 5, wY + (wH / 1.5));
	
	// cairo_curve_to(cr, wX + 5, wY + (wH/1.5), wX + (wW * filter_frequency), (wY - (wH/8)) - (filter_resonance * 30), (wX + wW) - 10, wY + (wH/1.5)  );
	// cairo_curve_to(cr, wX + (wW - (wW / cutoff_frequency)), wY + (wH - (wH / 3.5)), wX + (wW - (wW / 8)), wY + (wH - (wH / 3.5)), wX + (wW - (wW / 8)), wY + (wH - (wH / 3.5)));
	cairo_stroke(cr);

	// -- text
	
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, wX - 1, wY - (font_size * 1.5), wW + 2, (font_size * 2) );
	cairo_fill(cr);
	
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, wX - 1, wY + wH - (font_size * 1.5), wW + 2, (font_size * 1.5) );
	cairo_fill(cr);

	cairo_text_extents_t extents;

	cairo_set_font_size(cr, font_size);
	cairo_text_extents(cr, label.c_str(), &extents);
	float x_text_centred = (wX + wW / 2) - extents.width / 2;

	cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	cairo_move_to(cr,x_text_centred, wY - 1);
	cairo_show_text(cr, label.c_str());
	
	cairo_text_extents(cr, "BANDPASS", &extents);
	x_text_centred = (wX + wW / 2) - extents.width / 2;

	
	cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	cairo_move_to(cr,x_text_centred, wY + wH - 2);
	cairo_show_text(cr, "BANDPASS");

}


//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Filter::Left_Button_Press(int xm, int ym)
{
}



