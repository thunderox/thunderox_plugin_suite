/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2019 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "Window.hpp"

#include "../../delirium_ui/delirium_ui.hpp"

START_NAMESPACE_DISTRHO

class triceratopsUI : public UI
{
	public:

		//------------------------------------------------------------------------------------------------------

		triceratopsUI() : UI(960, 460)
		{

			memset(fParameters, 0, sizeof(float)*kParameterCount);
			memset(fParameters_widget_number, -1, sizeof(int)*kParameterCount);

			GUI = Delirium_UI_Init(static_cast<float>(getWidth()),static_cast<float>(getHeight()), 40,30);
			
			// OSCILATOR PANEL
			
			float panelX = 0.5;
			float panelY = 0.5;



			

			
			//------------------------------------------------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------------------------------------------------
			
			
			// OSCILATOR ONE ------------------------------------------------------------------------------------------
			
			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19,6.5, "OSCILLATORS", 0);
			
			int widget_logo = Delirium_UI_Create_Widget(GUI, deliriumUI_Logo, 0, panelX + 13.5, panelY + 1.25, 3.5, 6, "", kParametertriceratopsTime);
			
			// OSCILATOR NAVIGATION ONE TO THREE -----------------------------------------------------------------------
			
			GUI->osc_nav1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 0.5, panelY + 0.25, 3, 0.75, "OSC 1", 0);
			GUI->osc_nav2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 4, panelY + 0.25, 3, 0.75, "OSC 2", 0);
			GUI->osc_nav3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 7.5, panelY + 0.25,3, 0.75, "OSC 3", 0);
			
			int group_osc1 = 1;
			int group_osc2 = 2;
			int group_osc3 = 3;
			
			GUI->group_visible[1] = true;
			GUI->group_visible[2] = false;
			GUI->group_visible[3] = false;
			
			Delirium_UI_Widget_Button* default_osc_nav1 = (Delirium_UI_Widget_Button*)GUI->Widgets[GUI->osc_nav1];
			default_osc_nav1->values[1] = 1;
			
			// OSCILATOR ONE ------------------------------------------------------------------------------------------

			int widget_osc1_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, group_osc1, panelX + 0.5, panelY + 1.25, 2, 2, "OSC-1", kParametertriceratopsTime);

			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_active, 1);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 
	
			int widget_osc1_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc1, panelX + 0.5, panelY + 4, 2, 2.5, "VOL", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_volume, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc1_volume, 0.5);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 

			int widget_osc1_pan = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc1, panelX + 3, panelY + 4, 2, 2.5, "PAN", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_pan, -1,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_pan, 0);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_pan; 

			int widget_triceratops_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_osc1, panelX + 6, panelY + 1.25, 1, 5.25, "DETUNE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_triceratops_amount, -1,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_triceratops_amount, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 
			
			
			int widget_selector = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, group_osc1, panelX + 8, panelY + 1.25, 2, 2.5,"WAVE", 0);

			int widget_osc1_glide = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc1, panelX + 8, panelY + 4, 2, 2.5, "GLIDE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_glide, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_glide, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_osc1_octave = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc1, panelX + 10.5, panelY + 1.25, 2, 2.5, "OCTAVE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_octave, 0,5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_octave, 0);
			Delirium_UI_Widget_Set_Increment(GUI, widget_osc1_octave, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_osc1_octave, true);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_osc1_pulsewidth = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc1, panelX + 10.5, panelY + 4, 2, 2.5, "PW", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_pulsewidth, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_pulsewidth, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc1_pulsewidth, 0.5);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 
			
			
			
			// OSCILATOR TWO ------------------------------------------------------------------------------------------

			int widget_osc2_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, group_osc2, panelX + 0.5, panelY + 1.25, 2, 2, "OSC-2", kParametertriceratopsTime);

			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_active, 1);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc2_volume; 
	
			int widget_osc2_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc2, panelX + 0.5, panelY + 4, 2, 2.5, "VOL", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_volume, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc2_volume, 0.5);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc2_volume; 

			int widget_osc2_pan = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc2, panelX + 3, panelY + 4, 2, 2.5, "PAN", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_pan, -1,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_pan, 0);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc2_pan; 

			int widget_osc2_detune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_osc2, panelX + 6, panelY + 1.25, 1, 5.25, "DETUNE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_detune, -1,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_detune, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 
			
			
			int widget_osc2_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, group_osc2, panelX + 8, panelY + 1.25, 2, 2.5,"WAVE", 0);

			int widget_osc2_glide = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc2, panelX + 8, panelY + 4, 2, 2.5, "GLIDE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_glide, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_glide, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_osc2_octave = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc2, panelX + 10.5, panelY + 1.25, 2, 2.5, "OCTAVE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_octave, 0,5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_octave, 0);
			Delirium_UI_Widget_Set_Increment(GUI, widget_osc2_octave, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_osc2_octave, true);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_osc2_pulsewidth = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc2, panelX + 10.5, panelY + 4, 2, 2.5, "PW", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_pulsewidth, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_pulsewidth, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc2_pulsewidth, 0.5);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 
			
			
			

			// OSCILATOR THREE ------------------------------------------------------------------------------------------

			int widget_osc3_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, group_osc3, panelX + 0.5, panelY + 1.25, 2, 2, "OSC-3", kParametertriceratopsTime);

			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_active, 1);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc3_volume; 
	
			int widget_osc3_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc3, panelX + 0.5, panelY + 4, 2, 2.5, "VOL", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_volume, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc3_volume, 0.5);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc3_volume; 

			int widget_osc3_pan = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc3, panelX + 3, panelY + 4, 2, 2.5, "PAN", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_pan, -1,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_pan, 0);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc3_pan; 

			int widget_osc3_detune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_osc3, panelX + 6, panelY + 1.25, 1, 5.25, "DETUNE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_detune, -1,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_detune, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 
			
			
			int widget_osc3_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, group_osc3, panelX + 8, panelY + 1.25, 2, 2.5,"WAVE", 0);

			int widget_osc3_glide = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc3, panelX + 8, panelY + 4, 2, 2.5, "GLIDE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_glide, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_glide, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_osc3_octave = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc3, panelX + 10.5, panelY + 1.25, 2, 2.5, "OCTAVE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_octave, 0,5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_octave, 0);
			Delirium_UI_Widget_Set_Increment(GUI, widget_osc3_octave, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_osc3_octave, true);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_osc3_pulsewidth = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_osc3, panelX + 10.5, panelY + 4, 2, 2.5, "PW", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_pulsewidth, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_pulsewidth, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc3_pulsewidth, 0.5);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 


			//------------------------------------------------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------------------------------------------------

			// LFO PANEL
			
			panelX = 20;
			panelY = 0.5;


			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19.5,6.5, "LFO", 0);
			
			GUI->lfo_nav1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 0.5, panelY + 0.25, 3, 0.75, "LFO 1", 0);
			GUI->lfo_nav2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 4, panelY + 0.25, 3, 0.75, "LFO 2", 0);
			GUI->lfo_nav3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 7.5, panelY + 0.25,3, 0.75, "LFO 3", 0);
			
			Delirium_UI_Widget_Button* default_lfo_nav = (Delirium_UI_Widget_Button*)GUI->Widgets[GUI->lfo_nav1];	
			
			int group_lfo1 = 4;
			int group_lfo2 = 5;
			int group_lfo3 = 6;
						
			GUI->group_visible[group_lfo1] = true;
			GUI->group_visible[group_lfo2] = false;
			GUI->group_visible[group_lfo3] = false;
			
			// LFO ONE ------------------------------------------------------------------------------------------

			int widget_lfo1_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, group_lfo1, panelX + 0.5, panelY + 1.25, 2, 2, "LFO-1", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_retrig, 1);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 

			int widget_lfo1_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_lfo1, panelX + 0.5, panelY + 4, 2, 2.5, "BPM", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_speed, 0,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo1_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo1_speed, true);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 
			
			int widget_lfo1_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, group_lfo1, panelX + 2.5, panelY + 1.25, 2, 2.5,"WAVE", 0);

			int widget_lfo1_osc1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo1, panelX + 5, panelY + 1.25, 1, 5.25, "OSC-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_osc2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo1, panelX + 7, panelY + 1.25, 1, 5.25, "OSC-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_osc3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo1, panelX + 9, panelY + 1.25, 1, 5.25, "OSC-3", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo1, panelX + 11, panelY + 1.25, 1, 5.25, "FILTER", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo1, panelX + 13.5, panelY + 1.25, 1, 5.25, "OUT-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo1, panelX + 15.5, panelY + 1.25, 1, 5.25, "OUT-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;
			
			
			// LFO TWO ------------------------------------------------------------------------------------------

			int widget_lfo2_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, group_lfo2, panelX + 0.5, panelY + 1.25, 2, 2, "LFO-2", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_retrig, 1);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 

			int widget_lfo2_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_lfo2, panelX + 0.5, panelY + 4, 2, 2.5, "BPM", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_speed, 0,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo2_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo2_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo2_speed, true);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 
			
			int widget_lfo2_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, group_lfo2, panelX + 2.5, panelY + 1.25, 2, 2.5,"WAVE", 0);

			int widget_lfo2_osc1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo2, panelX + 5, panelY + 1.25, 1, 5.25, "OSC-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_osc1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_osc1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo2_osc2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo2, panelX + 7, panelY + 1.25, 1, 5.25, "OSC-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_osc2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_osc2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo2_osc3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo2, panelX + 9, panelY + 1.25, 1, 5.25, "OSC-3", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_osc3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_osc3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo2_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo2, panelX + 11, panelY + 1.25, 1, 5.25, "FILTER", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_route1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo2_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo2, panelX + 13.5, panelY + 1.25, 1, 5.25, "OUT-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_route2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo2_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo2, panelX + 15.5, panelY + 1.25, 1, 5.25, "OUT-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_route3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_route3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;
			
			
			// LFO THREE ------------------------------------------------------------------------------------------

			int widget_lfo3_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, group_lfo3, panelX + 0.5, panelY + 1.25, 2, 2, "LFO-3", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_retrig, 1);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 

			int widget_lfo3_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_lfo3, panelX + 0.5, panelY + 4, 2, 2.5, "BPM", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_speed, 0,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo3_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo3_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo3_speed, true);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 
			
			int widget_lfo3_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, group_lfo3, panelX + 2.5, panelY + 1.25, 2, 2.5,"WAVE", 0);

			int widget_lfo3_osc1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo3, panelX + 5, panelY + 1.25, 1, 5.25, "OSC-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_osc1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_osc1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo3_osc2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo3, panelX + 7, panelY + 1.25, 1, 5.25, "OSC-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_osc2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_osc2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo3_osc3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo3, panelX + 9, panelY + 1.25, 1, 5.25, "OSC-3", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_osc3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_osc3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo3_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo3, panelX + 11, panelY + 1.25, 1, 5.25, "FILTER", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_route1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo3_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo3, panelX + 13.5, panelY + 1.25, 1, 5.25, "OUT-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_route2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo3_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_lfo3, panelX + 15.5, panelY + 1.25, 1, 5.25, "OUT-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_route3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_route3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;
			
			

			//--------------------------------------------------------------------------------------------
			// ENVELOPE PANEL
			
			panelX = 0.5;
			panelY = 7.75;
			
			// --------------
			
			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19, 6.25, "ENVELOPES", 0);

			Delirium_UI_Widget_Button* default_lfo_nav1 = (Delirium_UI_Widget_Button*)GUI->Widgets[GUI->lfo_nav1];
			default_lfo_nav1->values[1] = 1;

			int widget_amp_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 0.25, panelY + 0.75, 5, 2.5,"AMP",0); 

			int widget_filter_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 0.25, panelY + 3.75, 5, 2.5,"FILTER",0);
			
			int widget_mod_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 13.75, panelY + 0.75, 5, 2.5,"MOD",0); 
 
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_env1_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 6, panelY + 1.25, 1, 5.25, "AMP", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env1_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env1_route1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_env1_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 8, panelY + 1.25, 1, 5.25, "OSC-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env1_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env1_route2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 
			
			int widget_env2_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 10, panelY + 1.25, 1, 5.25, "FILTER", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env2_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env2_route1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_env2_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 12, panelY + 1.25, 1, 5.25, "OSC-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env2_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env2_route2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 
			
			int widtet_env3_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 13.5, panelY + 4, 2, 2, "OSC-2", kParametertriceratopsTime);
			int widtet_env3_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 17, panelY + 4, 2, 2, "OSC-3", kParametertriceratopsTime);
			// int widtet_env3_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 18, panelY + 2.75, 2, 2, "OSC-3", kParametertriceratopsTime);

			//--------------------------------------------------------------------------------------------
			// GENERAL PANEL - INCLUDING VOLUME
			
			int group_vol = 7;
			int group_fx = 8;
			int group_mod = 9;
			
			GUI->group_visible[group_vol] = true;
			GUI->group_visible[group_fx] = false;
			GUI->group_visible[group_mod] = false;

			
			panelX = 20;
			panelY = 7.75;

			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19.5,6.25, "AMP, FILTER & FX", 0);
			
			GUI->vol_nav = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 0.5, panelY + 0.25, 3, 0.75, "VOL / FLT", 0);
			GUI->fx_nav = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 4, panelY + 0.25, 3, 0.75, "FX", 0);
			GUI->mod_nav = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 7.5, panelY + 0.25,3, 0.75, "MOD", 0);
									
			Delirium_UI_Widget_Button* default_vol_nav = (Delirium_UI_Widget_Button*)GUI->Widgets[GUI->vol_nav];
			default_vol_nav->values[1] = 1;

			
			// FILTER CUTOFF FREQUENCY
			
			int widget_filter_cutoff = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_vol,
				panelX + 4, panelY + 1.25, 1, 5.25, "CUTOFF",kParametertriceratops_FilterFrequency);
				
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_cutoff, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_cutoff, 0.5);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;
			
			// FILTER RESONANCE AMOUNT
			
			int widget_filter_resonance = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_vol,
				panelX + 6, panelY + 1.25, 1, 5.25, "RES", kParametertriceratops_FilterResonance);
				
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_resonance, 1,0);

			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;
			
			// FILTER TYPE SELECTOR
			
			int widget_filter_type = Delirium_UI_Create_Widget(GUI, deliriumUI_Filter, group_vol, panelX + 0.5, panelY + 1.6, 2.5, 4.5, "FILTER", kParametertriceratops_FilterType);
			fParameters_widget_number[kParametertriceratops_FilterFrequency] = widget_filter_type;	
			
			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[widget_filter_type];
			
			wdg->filter_frequency = 0.5;
			wdg->filter_resonance = 0;
			wdg->values[0] = 2;
			
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_type, 0,4);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_type, 2);
						
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_cutoff, wdg->filter_frequency);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_resonance, wdg->filter_resonance);			

			// FILTER FOLLOW KEYBOARD
			
			int widget_filter_key_follow = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_vol, panelX + 11.5, panelY + 1.25, 1, 5.25, "KEYF", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_key_follow, 1,-1);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_key_follow, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;
			
			int widget_master_tune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, group_vol, panelX + 13.5, panelY + 1.25, 1, 5.25, "TUNE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_master_tune, -7,7);
			Delirium_UI_Widget_Set_Value(GUI, widget_master_tune, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, group_vol, panelX + 15, panelY + 1.75, 4, 4, "VOLUME", kParametertriceratopsVolume);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_volume, 0.6);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_volume, 0.6);
			fParameters_widget_number[kParametertriceratopsVolume] = widget_volume; 

			GUI->draw_flag = true;

			cout << "CAIRO WINDOW = " << getParentWindow().getGraphicsContext().cairo << endl;			
			
		}

		//------------------------------------------------------------------------------------------------------

		~triceratopsUI()
		{
			Delirium_UI_Cleanup(GUI);
		}

		//------------------------------------------------------------------------------------------------------

		void onDisplay()
		{
			cairo_t* cr = getParentWindow().getGraphicsContext().cairo;
			Delirium_UI_Display_All(GUI, cr);
		}

		bool onMouse(const MouseEvent& ev) override
		{
			cairo_t* cr = getParentWindow().getGraphicsContext().cairo;	

			if (ev.button == 1)
			{
				Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());
				Delirium_UI_Left_Button_Press(GUI, cr, ev.pos.getX(), ev.pos.getY());
				repaint();
				
				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
				if (parameter_number > 0)
				{	
					float value = Delirium_UI_Widget_Get_Value(GUI);
					 setParameterValue(parameter_number, value);
					 
					 if (parameter_number == kParametertriceratops_FilterFrequency)
					 {
					 	int wdg_number = GUI->parameter_widget_number[kParametertriceratops_FilterType];
			 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
						wdg->filter_frequency = value;
						GUI->Widgets[wdg_number]->Draw(cr);
					 }

					if (parameter_number == kParametertriceratops_FilterResonance)
					 {
					 	int wdg_number = GUI->parameter_widget_number[kParametertriceratops_FilterType];
			 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
						wdg->filter_resonance = value;
						GUI->Widgets[wdg_number]->Draw(cr);
					 }
				}
				return true;

			}


			if (ev.button == 2)
			{

				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
				if (parameter_number > 0)
				{
	
					setParameterValue(parameter_number, 0);
					Delirium_UI_Middle_Button_Press(GUI);	
					repaint();	
					
					if (parameter_number == kParametertriceratops_FilterFrequency)
					{
						int wdg_number = GUI->parameter_widget_number[kParametertriceratops_FilterType];
						Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
						wdg->filter_frequency = 0;
						GUI->Widgets[wdg_number]->Draw(cr);	
					}
					
					if (parameter_number == kParametertriceratops_FilterResonance)
					 {
					 	int wdg_number = GUI->parameter_widget_number[kParametertriceratops_FilterType];
			 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
						wdg->filter_resonance = 0;
						GUI->Widgets[wdg_number]->Draw(cr);
					 }			
				}
			}


			return true;
		}

		//------------------------------------------------------------------------------------------------------

		bool onMotion(const MotionEvent& ev)
		{
			cairo_t* cr = getParentWindow().getGraphicsContext().cairo;

			Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());

			repaint();	

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
	

			if (parameter_number > 0)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);
				
				 if (parameter_number == kParametertriceratops_FilterFrequency)
				 {
				 	int wdg_number = GUI->parameter_widget_number[kParametertriceratops_FilterType];
					Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
					wdg->filter_frequency = value;
					GUI->Widgets[wdg_number]->Draw(cr);
				 }
				 
				if (parameter_number == kParametertriceratops_FilterResonance)
				 {
				 	int wdg_number = GUI->parameter_widget_number[kParametertriceratops_FilterType];
		 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
					wdg->filter_resonance = value;
					GUI->Widgets[wdg_number]->Draw(cr);
				 }
					 
			}
			return true;

		}

		//------------------------------------------------------------------------------------------------------

		bool onScroll(const ScrollEvent& ev)
		{
			float delta = ev.delta.getY();
			cairo_t* cr = getParentWindow().getGraphicsContext().cairo;

			int current_widget = GUI->current_widget;

			if (current_widget > 0)
			{	
				if (GUI->Widgets[current_widget]->type == deliriumUI_Panel) return true;
			
				GUI->Widgets[current_widget]->Mouse_Scroll(delta);
				Delirium_UI_Convert_Value_To_Range(GUI, current_widget);
				GUI->Widgets[current_widget]->Draw(cr);
				repaint();
			}

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);

			if (parameter_number > 0)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);
				
				if (parameter_number == kParametertriceratops_FilterFrequency)
				{
					int wdg_number = GUI->parameter_widget_number[kParametertriceratops_FilterType];
					Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
					wdg->filter_frequency = value;
					GUI->Widgets[wdg_number]->Draw(cr);	
				}
				
				if (parameter_number == kParametertriceratops_FilterResonance)
				 {
				 	int wdg_number = GUI->parameter_widget_number[kParametertriceratops_FilterType];
		 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
					wdg->filter_resonance = value;
					GUI->Widgets[wdg_number]->Draw(cr);
				 }
			}

			return true;
		}


	protected:

		/* --------------------------------------------------------------------------------------------------------
		* DSP/Plugin Callbacks 
		A parameter has changed on the plugin side.
		This is called by the host to inform the UI about parameter changes.
		*/

		void parameterChanged(uint32_t index, float value) override
		{
			fParameters[index] = value;

			// get widget number connected to this parameter
			int widget_number = fParameters_widget_number[index];

			if (widget_number > 0)
				Delirium_UI_Widget_Set_Value(GUI, widget_number, value);

			// trigger repaint
			repaint();
		}

	private:

		Delirium_UI_Surface* GUI;
		float fParameters[kParameterCount];
		int fParameters_widget_number[kParameterCount];

		/*  Set our UI class as non-copyable and add a leak detector just in case. */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(triceratopsUI)

};

UI* createUI()
{
    return new triceratopsUI;
}

END_NAMESPACE_DISTRHO


