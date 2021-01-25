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
			
			int group_osc1 = 1;
			int group_osc2 = 2;
			int group_osc3 = 3;

			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19,6.5, "OSCILLATORS", 0);
			
			int widget_logo = Delirium_UI_Create_Widget(GUI, deliriumUI_Logo, 0, panelX + 13.5, panelY + 1.25, 3.5, 6, "", kParametertriceratopsTime);
			
			// OSCILATOR NAVIGATION ONE TO THREE -----------------------------------------------------------------------
			
			GUI->osc_nav1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 0.5, panelY + 0.25, 3, 0.75, "OSC 1", 0);
			GUI->osc_nav2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 4, panelY + 0.25, 3, 0.75, "OSC 2", 0);
			GUI->osc_nav3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Button, 0, panelX + 7.5, panelY + 0.25,3, 0.75, "OSC 3", 0);
			
			// OSCILATOR ONE ------------------------------------------------------------------------------------------

			int widget_osc1_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, group_osc1, panelX + 0.5, panelY + 1.25, 2, 2, "ON", kParametertriceratopsTime);

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

			int widget_osc2_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, group_osc2, panelX + 0.5, panelY + 1.25, 2, 2, "ON", kParametertriceratopsTime);

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


			//-----------------------------------------------------------------------------------

			// LFO PANEL
			
			panelX = 20;
			panelY = 0.5;


			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19.5,6.5, "LFO", 0);

			int widget_lfo1_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 0.5, 2, 2, "ON", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_retrig, 1);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 

			int widget_lfo1_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 0.5, panelY + 3.25, 2, 2.5, "BPM", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_speed, 0,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo1_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo1_speed, true);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 
			
			int widget_lfo_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX + 2.5, panelY + 0.5, 2, 2.5,"WAVE", 0);

			int widget_lfo1_osc1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 5, panelY + 0.5, 1, 5.25, "OSC-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_osc2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 7, panelY + 0.5, 1, 5.25, "OSC-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_osc3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 9, panelY + 0.5, 1, 5.25, "OSC-3", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 11, panelY + 0.5, 1, 5.25, "FILTER", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 13.5, panelY + 0.5, 1, 5.25, "OUT-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 15.5, panelY + 0.5, 1, 5.25, "OUT-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			//--------------------------------------------------------------------------------------------
			// ENVELOPE PANEL
			
			panelX = 0.5;
			panelY = 8;

			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19, 5.75, "ENVELOPES", 0);


			int widget_amp_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 0.5, panelY + 1, 6, 2,"AMP",0); 

			int widget_filter_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 0.5, panelY + 3.5, 6, 2,"FILTER",0);
 
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_env1_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 8, panelY + 0.5, 1, 5.25, "AMP", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env1_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env1_route1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_env1_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 10, panelY + 0.5, 1, 5.25, "OSC-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env1_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env1_route2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			//--------------------------------------------------------------------------------------------
			// GENERAL PANEL - INCLUDING VOLUME
			
			panelX = 20;
			panelY = 8;

			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19.5,5.75, "AMP, FILTER & FX", 0);

			
			// FILTER CUTOFF FREQUENCY
			
			int widget_filter_cutoff = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 6, panelY + 0.5, 1, 5.25, "CUTOFF",kParametertriceratops_FilterFrequency);
				
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_cutoff, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_cutoff, 0.5);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;
			
			// FILTER RESONANCE AMOUNT
			
			int widget_filter_resonance = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 8, panelY + 0.5, 1, 5.25, "RES", kParametertriceratops_FilterResonance);
				
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_resonance, 1,0);

			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;
			
			// FILTER TYPE SELECTOR
			
			int widget_filter_type = Delirium_UI_Create_Widget(GUI, deliriumUI_Filter, 0, panelX + 0.5, panelY + 1, 2.5, 4, "FILTER", kParametertriceratops_FilterType);
			fParameters_widget_number[kParametertriceratops_FilterFrequency] = widget_filter_type;	
			
			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[widget_filter_type];
			
			wdg->filter_frequency = 0.5;
			wdg->filter_resonance = 0;
			
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_cutoff, wdg->filter_frequency);
			Delirium_UI_Widget_Set_Value(GUI, wdg->filter_resonance, 0);

			// FILTER FOLLOW KEYBOARD
			
			int widget_filter_key_follow = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 11.5, panelY + 0.5, 1, 5.25, "KEYF", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_key_follow, 1,-1);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_key_follow, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;
			
			int widget_master_tune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 13.5, panelY + 0.5, 1, 5.25, "TUNE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_master_tune, -7,7);
			Delirium_UI_Widget_Set_Value(GUI, widget_master_tune, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 15, panelY + 1, 4, 4, "VOLUME", kParametertriceratopsVolume);
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


