
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 

//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Selector::Draw(cairo_t* cr)
{
	if (integer) values[current_value] = int(values[current_value]);
	Convert_Scaled_To_Value();

	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;

	cairo_set_source_rgb(cr, 0.15,0,0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_fill(cr);


	// SHOW CURRENT WAVEFORM
	
	
	cairo_set_source_rgb(cr, 0.6,0.6,0.6);		
	
	switch (int(values[current_value]))
	{
		case 0:
			// SAW
			cairo_move_to(cr,x+(w/4), y+(h/1.25));
			cairo_line_to(cr, x+(w-(w/4)), y+(h/2.75));
			cairo_stroke(cr);
			break;
		case 1:
				
			// SQUARE
			cairo_move_to(cr, x+(w/4), y+(h/1.25));
			cairo_line_to(cr, x+(w/4), y+(h/2.75));
			cairo_line_to(cr, x+(w-(w/4)), y+(h/2.75));
			cairo_line_to(cr, x+(w-(w/4)), y+(h/1.25));
			cairo_stroke(cr);
			break;
		case 2:
			// SINE 
			cairo_move_to(cr, x+(w/4), y+(h/2.75));
	  		cairo_curve_to(cr, x+(w/2), y+(h*1.5), x+(w/2),y-(h/2), x+(w-(w/4)),y+(h/1.25));
			cairo_stroke(cr);
			break;
			
		case 3:
			// S&H
			cairo_move_to(cr, x+(w/8), y+(h/2));
			for (int shx=(w/8)+(w/16); shx<(w-(w/8))-(w/16); shx+=(w/8))
			{
				float shy = (rand() % int(h)/4) - (2*(rand() % int(h)/4));  		
				cairo_line_to(cr, x+shx, y+((h/1.75) + (shy/2)));			
			}
			cairo_line_to(cr, x+(w-(w/8)), y+(h/2));
			cairo_stroke(cr);
			break;
	}
		

	// DRAW LABEL

	cairo_set_source_rgb(cr, 0,0,0);
	cairo_rectangle(cr, x, y, w,font_size);
	cairo_fill(cr);

	if (hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);

	}


	cairo_text_extents_t extents;
	cairo_set_font_size(cr, font_size);
	float x_text_centred; 
	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (x + w / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, y+font_size);
	cairo_show_text(cr, label.c_str());



}

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Selector::Left_Button_Press(int xm, int ym)
{

	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;
		
	toggle_mode = 1 - toggle_mode;

	if (toggle_mode==0) 
	{
		values[current_value]++;
		if (values[current_value]>6) values[current_value]=0;
		Convert_Scaled_To_Value();
		cout << values[current_value] << endl;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Widget_Selector::Mouse_Over(int xm, int ym)
{
}




