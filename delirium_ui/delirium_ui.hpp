
#include <iostream>
#include <vector>
#include <cairo/cairo.h>
#include "DistrhoUI.hpp"

#ifndef DELIRIUM_UI
#define DELIRIUM_UI

const float font_size = 10;

using namespace std;

typedef enum 
{
	deliriumUI_Button=1,
	deliriumUI_Knob=2,
	deliriumUI_MicroKnob=3,
	deliriumUI_Fader=4, 
	deliriumUI_Switch=5,
	deliriumUI_ADSR=6,
	deliriumUI_Wave_Selector=7,
	deliriumUI_LFO=8,
	deliriumUI_Selector=9,
	deliriumUI_Panel=10,
	deliriumUI_Label=11,
	deliriumUI_Logo=12,
	deliriumUI_Filter=13
} deliriumUI_WidgetType;


class Delirium_UI_Widget_Base 
{

	public:

	Delirium_UI_Widget_Base();
	virtual ~Delirium_UI_Widget_Base();	
	virtual void Draw(cairo_t*);
	virtual void Left_Button_Press(int,int);
	virtual void Middle_Button_Press();
	virtual void Mouse_Over(int,int);
	virtual void Mouse_Scroll(float);
	virtual void Convert_Value_To_Scaled();
	virtual void Convert_Scaled_To_Value();

	float	x_position;
	float	y_position;
	float	width;
	float	height;
	float x_grid_size;
	float y_grid_size;

	bool	pressed;
	bool	hover;
	bool	toggle_mode;
	bool	integer;

	string label;
	int group;
	int parameter_number;
	int type;
	bool inverted;

	float normalised_values[4];
	float min;
	float max;
	float values[4];
	float default_values[4];
	int current_value;
	float increment;

	cairo_surface_t *surface_image;

	private:
 
};

class Delirium_UI_Widget_Button : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
};

class Delirium_UI_Widget_Fader : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Middle_Button_Press();
};

class Delirium_UI_Widget_Knob : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Middle_Button_Press();
};

class Delirium_UI_Widget_Switch : public Delirium_UI_Widget_Base
{
	public: void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Mouse_Scroll(float);
};

class Delirium_UI_Widget_Label : public Delirium_UI_Widget_Base
{
	public: void Draw(cairo_t*);
	void Left_Button_Press(int,int);
};

class Delirium_UI_Widget_Logo : public Delirium_UI_Widget_Base
{
	public: void Draw(cairo_t*);
	void Left_Button_Press(int,int);
};


class Delirium_UI_Widget_Panel : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
};

class Delirium_UI_Widget_ADSR : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Mouse_Over(int,int);
};

class Delirium_UI_Widget_Selector : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Mouse_Over(int,int);
};

class Delirium_UI_Widget_Filter : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	float filter_frequency;
	float filter_resonance;
};

struct Delirium_UI_Surface
{

	int width;
	int height;
	float x_grid_size;
	float y_grid_size;
	int current_widget;
	bool drag;
	bool draw_flag;


	float background_rgb[3];
	float foreground_rgb[3];

	int current_group;
	bool group_visible[32];

	vector <Delirium_UI_Widget_Base*> Widgets;
	int parameter_widget_number[512];
	
	int osc_nav1, osc_nav2, osc_nav3;
	int lfo_nav1, lfo_nav2, lfo_nav3;

};

Delirium_UI_Surface* Delirium_UI_Init(int,int,int,int);

int  Delirium_UI_Create_Widget(Delirium_UI_Surface*, int, int, float, float, float, float, string, int);
void Delirium_UI_Widget_Set_Value(Delirium_UI_Surface*, int, float);
void Delirium_UI_Widget_Set_Increment(Delirium_UI_Surface*, int, float);
void Delirium_UI_Widget_Set_Min_Max(Delirium_UI_Surface*, int, float, float);
float Delirium_UI_Widget_Get_Value(Delirium_UI_Surface*);
void Delirium_UI_Widget_Set_Default_Value(Delirium_UI_Surface*, int, float);
int Delirium_UI_Widget_Get_Parameter_Number(Delirium_UI_Surface*);
void Delirium_UI_Display_All(Delirium_UI_Surface*, cairo_t*);
void Delirium_UI_MouseOver(Delirium_UI_Surface*, cairo_t*, int,int);
void Delirium_UI_Left_Button_Press(Delirium_UI_Surface*, cairo_t*, int,int);
void Delirium_UI_Middle_Button_Press(Delirium_UI_Surface*);
void Delirium_UI_Mouse_Over(Delirium_UI_Surface*, int,int);
void Delirium_UI_Convert_Value_To_Range(Delirium_UI_Surface*, int);
void Delirium_UI_Convert_Range_To_Value(Delirium_UI_Surface*, int);
void Delirium_UI_Cleanup(Delirium_UI_Surface*);
void Delirium_UI_Widget_Set_Integer(Delirium_UI_Surface*, int, bool);


#endif

