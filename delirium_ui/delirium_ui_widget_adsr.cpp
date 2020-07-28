
#include "delirium_ui.hpp"

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_ADSR::Draw(cairo_t* cr)
{
	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;

	float sx = x;
	float sy = y + (h/6);
	float sw = w;
	float sh = h * 0.8;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.25);
	cairo_paint(cr);

	// DRAW LABEL

	cairo_text_extents_t extents;
	float x_text_centred; 
	cairo_set_source_rgb(cr, 0.8,0.8,0.8);
	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (x + w / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, y);
	cairo_show_text(cr, label.c_str());

	// ----------

	if (hover)
	{
		// cout << x << endl;
	}

	// ----------

	cairo_set_line_width(cr, w/80);
	cairo_move_to(cr, sx+(w/5), sy);

	float yp;

	cairo_set_source_rgba(cr, 0.7, 0.7, 0.7, 1);

	for (int xc=1; xc<5; xc++)
	{
		float xcol = xc * (w / 5);
		
		if (xc > 1) yp = (values[xc-1] * (h/2));
			else yp = (h/2) - (values[xc-1] * (h/2));

		if (xc > 1)
		{
			cairo_line_to(cr,  sx+(xcol), sy+yp);
			cairo_stroke(cr);
		}
				
		if (hover && xc==current_value+1 )
		{
			cairo_set_source_rgba(cr, 0.9, 0.9, 0.9, 1);
			cairo_arc(cr, sx+(xcol), sy+yp, (h/20), 0.0, 2 * M_PI);
		} else
		{
			cairo_set_source_rgba(cr, 0.7, 0.7, 0.7, 1);
			cairo_arc(cr, sx+(xcol), sy+yp, (h/24), 0.0, 2 * M_PI);
		}

		cairo_fill(cr);


		cairo_move_to(cr, sx+xcol, sy+yp);

	}

}

//-------------------------------------------------------------------------------------------
// USER PRESSED LEFT MOUSE BUTTON

void Delirium_UI_Widget_ADSR::Left_Button_Press(int xm, int ym)
{

}


//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Widget_ADSR::Mouse_Over(int xm, int ym)
{
	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;

	int new_current_value = int((xm-x)/(w/5))-1;

	if (new_current_value >= 0 && new_current_value <= 3) current_value = new_current_value;

	cout << new_current_value << endl;
	
}





