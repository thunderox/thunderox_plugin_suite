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

// ThunderOx Triceratops synth - By Nick S. Bailey

#include "DistrhoPlugin.hpp"
#include <string.h>
#include <string>
#include <iostream>
#include <vector> 
#include <math.h>
#include "synth.h"
#include "lfo.h"
#include "nix-echo.hpp"
#include "JCRev.h"
#include "eq.h"


using namespace std;

const int max_notes = 12;

START_NAMESPACE_DISTRHO

class triceratopsPlugin : public Plugin
{
	public:

		// Audio Struct
		struct audio_stereo
		{ 
			float left,right; 
		};
		float srate;

		int midi_keys[128];
			
		int current_synth;
		int old_synth;
		
		float* pitch_bend;
		float* channel_after_touch;
	
		synth synths[max_notes];
		params* synth_params;

		nixecho* echo;
		noise* nixnoise;
		JCRev* reverb[16];
		
		EQSTATE* eq_right;

		uint32_t buffer_frame;
	
		// triceratops Audio Buffer
		vector <audio_stereo> audio_buffer; 
		
		float PARAM_OUT_LEFT,
		PARAM_OUT_RIGHT,
		PARAM_CONTROL,

		PARAM_MASTER_VOLUME,
		PARAM_MASTER_TUNE,
		PARAM_AMP_DRIVE,
		PARAM_FILTER_MODE,
		PARAM_CUTOFF,
		PARAM_RESONANCE,
		PARAM_FILTER_KEY_FOLLOW,
		PARAM_LEGATO,
		PARAM_SYNC,
		PARAM_WARMTH,
		PARAM_FM,
		PARAM_PANIC,

		PARAM_OSC1_ACTIVE,
		PARAM_OSC1_VOLUME,
		PARAM_OSC1_PULSEWIDTH,
		PARAM_OSC1_WAVE,
		PARAM_OSC1_OCTAVE,
		PARAM_OSC1_DETUNE,
		PARAM_OSC1_DETUNE_CENTRE,
		PARAM_OSC1_INERTIA,

		PARAM_OSC2_ACTIVE,
		PARAM_OSC2_VOLUME,
		PARAM_OSC2_PULSEWIDTH,
		PARAM_OSC2_WAVE,
		PARAM_OSC2_OCTAVE,
		PARAM_OSC2_DETUNE,
		PARAM_OSC2_DETUNE_CENTRE,
		PARAM_OSC2_INERTIA,

		PARAM_OSC3_ACTIVE,
		PARAM_OSC3_VOLUME,
		PARAM_OSC3_PULSEWIDTH,
		PARAM_OSC3_WAVE,
		PARAM_OSC3_OCTAVE,
		PARAM_OSC3_DETUNE,
		PARAM_OSC3_DETUNE_CENTRE,
		PARAM_OSC3_INERTIA,

		PARAM_ADSR1_ATTACK,
		PARAM_ADSR1_DECAY,
		PARAM_ADSR1_SUSTAIN,
		PARAM_ADSR1_RELEASE,
		PARAM_ADSR1_ROUTE_ONE,
		PARAM_ADSR1_ROUTE_ONE_DEST,
		PARAM_ADSR1_ROUTE_TWO,
		PARAM_ADSR1_ROUTE_TWO_DEST,

		PARAM_ADSR2_ATTACK,
		PARAM_ADSR2_DECAY,
		PARAM_ADSR2_SUSTAIN,
		PARAM_ADSR2_RELEASE,
		PARAM_ADSR2_ROUTE_ONE,
		PARAM_ADSR2_ROUTE_ONE_DEST,
		PARAM_ADSR2_ROUTE_TWO,
		PARAM_ADSR2_ROUTE_TWO_DEST,

		PARAM_ADSR3_ATTACK,
		PARAM_ADSR3_DECAY,
		PARAM_ADSR3_SUSTAIN,
		PARAM_ADSR3_RELEASE,
		PARAM_ADSR3_LFO1_AMOUNT,
		PARAM_ADSR3_LFO2_AMOUNT,
		PARAM_ADSR3_LFO3_AMOUNT,

		PARAM_LFO1_RETRIG,
		PARAM_LFO1_SPEED,
		PARAM_LFO1_WAVE,
		PARAM_LFO1_DCO1_PITCH,
		PARAM_LFO1_DCO2_PITCH,
		PARAM_LFO1_DCO3_PITCH,
		PARAM_LFO1_FILTER,
		PARAM_LFO1_ROUTE_ONE,
		PARAM_LFO1_ROUTE_ONE_DEST,
		PARAM_LFO1_ROUTE_TWO,
		PARAM_LFO1_ROUTE_TWO_DEST,

		PARAM_LFO2_RETRIG,
		PARAM_LFO2_SPEED,
		PARAM_LFO2_WAVE,
		PARAM_LFO2_DCO1_PITCH,
		PARAM_LFO2_DCO2_PITCH,
		PARAM_LFO2_DCO3_PITCH,
		PARAM_LFO2_FILTER,
		PARAM_LFO2_ROUTE_ONE,
		PARAM_LFO2_ROUTE_ONE_DEST,
		PARAM_LFO2_ROUTE_TWO,
		PARAM_LFO2_ROUTE_TWO_DEST,

		PARAM_LFO3_RETRIG,
		PARAM_LFO3_SPEED,
		PARAM_LFO3_WAVE,
		PARAM_LFO3_DCO1_PITCH,
		PARAM_LFO3_DCO2_PITCH,
		PARAM_LFO3_DCO3_PITCH,
		PARAM_LFO3_FILTER,
		PARAM_LFO3_ROUTE_ONE,
		PARAM_LFO3_ROUTE_ONE_DEST,
		PARAM_LFO3_ROUTE_TWO,
		PARAM_LFO3_ROUTE_TWO_DEST,

		PARAM_FX_ECHO_ACTIVE,
		PARAM_FX_ECHO_SPEED,
		PARAM_FX_ECHO_DECAY,
		PARAM_FX_ECHO_EQ_LOW,
		PARAM_FX_ECHO_EQ_MID,
		PARAM_FX_ECHO_EQ_HIGH,

		PARAM_UNISON_ACTIVATE,
		PARAM_UNISON_ONE,
		PARAM_UNISON_TWO,
		PARAM_UNISON_THREE,

		PARAM_MODIFIER_DIRT,

		PARAM_FX_REVERB_ACTIVE,
		PARAM_FX_REVERB_DECAY,
		PARAM_FX_REVERB_MIX,

		PARAM_MODIFIER_STEREO_MODE,
		PARAM_OSC1_PAN,
		PARAM_OSC2_PAN,
		PARAM_OSC3_PAN,
		PARAM_MODIFIER_RING,

		PARAM_PRESET_CATEGORY,
		PARAM_PITCH_BEND_RANGE,
		PARAM_MIDI_CHANNEL;
			
		triceratopsPlugin() : Plugin(kParameterCount, 0, 0)
		{

			srate = getSampleRate();

			audio_buffer.resize(srate*5);

			// Initialize EQs
			
			EQSTATE* eq_left = new EQSTATE();
			init_3band_state(eq_left,220,5000,srate);	
			eq_left->lg = 0.0; // BASS
			eq_left->mg = 1.0; // MIDS
			eq_left->hg = 1.0; // HIGHS
		
			EQSTATE* eq_right = new EQSTATE();
			init_3band_state(eq_right,220,5000,srate);		
			eq_right->lg = 0.0; // BASS
			eq_right->mg = 1.0; // MIDS
			eq_right->hg = 1.0; // HIGHS 
			
			// Initialize LFOs
			
			LFO* lfo1;	
			LFO* lfo2;
			LFO* lfo3;
		
			float* lfo1_out;
			float* lfo2_out;
			float* lfo3_out;
		
			float lfo1_rand;
			float lfo2_rand;
			float lfo3_rand;
		
			lfo1_out = (float*)malloc(sizeof(float)*4096);	
			lfo2_out = (float*)malloc(sizeof(float)*4096);
			lfo3_out = (float*)malloc(sizeof(float)*4096);
		
			memset( lfo1_out, 0, sizeof(float)*4096 );
			memset( lfo2_out, 0, sizeof(float)*4096 );
			memset( lfo3_out, 0, sizeof(float)*4096 );
			
			// Initialise Echo
		
			nixecho* echo = new nixecho();
			echo->set_sample_rate(srate);
		
			//Initialize Noise
		
			noise* nixnoise = new noise();
					
			lfo1_rand = nixnoise->tick();
			lfo2_rand = nixnoise->tick();
			lfo3_rand = nixnoise->tick();
		
			// -----------------
		
			float pitch_bend;
			float channel_after_touch;
			int current_synth = 0;
			int old_synth = 0;

			buffer_frame = 0;
	
			// clear all parameters
			std::memset(fParameters, 0, sizeof(float)*kParameterCount);

			// we can know buffer-size right at the start
			fParameters[kParameterBufferSize] = getBufferSize();

		}
		


		const char* getLabel() const
		{
			return "ThunderOx triceratops";
		}

		const char* getMaker() const
		{
			return "Nick S. Bailey";
		}

		const char* getLicense() const
		{
			return "ISC";
		}

		//Get the plugin version, in hexadecimal.
    
		uint32_t getVersion() const override
		{
			return d_version(1, 0, 0);
		}

		int64_t getUniqueId() const
		{
			return 0;
		}

		void initParameter(uint32_t index, Parameter& parameter) override
		{
			switch (index)
			{
				case kParametertriceratopsTime:
					parameter.name   = "triceratops Time";
					parameter.symbol = "triceratops_time";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.5f;
					fParameters[kParametertriceratopsTime] = parameter.ranges.def;
					break;

				case kParametertriceratopsAmount:
					parameter.name   = "triceratops Amount";
					parameter.symbol = "triceratops_amount";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 1.0f;
					fParameters[kParametertriceratopsAmount] = parameter.ranges.def;
					break;

				case kParametertriceratopsVolume:
					parameter.name   = "triceratops Volume";
					parameter.symbol = "triceratops_volume";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.8f;
					fParameters[kParametertriceratopsVolume] = parameter.ranges.def;
					break;
			}

		}

		/* --------------------------------------------------------------------------------------------------------
		* Internal data 
		Get the current value of a parameter.
 		The host may call this function from any context, including realtime processing.
		*/

		float getParameterValue(uint32_t index) const override
		{
			return fParameters[index];

		}

		/**
  		Change a parameter value.
 		The host may call this function from any context, including realtime processing.
		When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
		@note This function will only be called for parameter inputs.
		*/

		void setParameterValue(uint32_t index, float value) override
		{
			fParameters[index] = value;
		}

		void run(const float** inputs, float** outputs, uint32_t frames,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override
		{
			// cout << getParameterValue(TRICERATOPS_OSC1_DETUNE) << endl;
			// memcpy(outputs[0], inputs[0], frames * sizeof(float));

			float* out_left = outputs[0];
			float* out_right = outputs[1];
			



			// cout << timePos.bbt.beatsPerMinute << endl;
		}

		//----------------------------------------------

	private:

		// Parameters
		float fParameters[kParameterCount];

		int bpm;
		uint32_t buffer_length;
};

Plugin* createPlugin()
{
	triceratopsPlugin* triceratops = new triceratopsPlugin();
	for (int x=0; x<max_notes; x++)
	{
		triceratops->synths[x].synth_params->TRICERATOPS_MASTER_VOLUME = &triceratops->PARAM_MASTER_VOLUME;
		triceratops->synths[x].synth_params->TRICERATOPS_AMP_DRIVE = &triceratops->PARAM_AMP_DRIVE;
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_MODE = &triceratops->PARAM_FILTER_MODE;
		triceratops->synths[x].synth_params->TRICERATOPS_CUTOFF = &triceratops->PARAM_CUTOFF;
		triceratops->synths[x].synth_params->TRICERATOPS_RESONANCE = &triceratops->PARAM_RESONANCE;		
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_KEY_FOLLOW = &triceratops->PARAM_FILTER_KEY_FOLLOW;
		triceratops->synths[x].synth_params->TRICERATOPS_LEGATO = &triceratops->PARAM_LEGATO;
		triceratops->synths[x].synth_params->TRICERATOPS_SYNC = &triceratops->PARAM_SYNC;
		triceratops->synths[x].synth_params->TRICERATOPS_WARMTH = &triceratops->PARAM_WARMTH;		
		triceratops->synths[x].synth_params->TRICERATOPS_FM = &triceratops->PARAM_FM;
		triceratops->synths[x].synth_params->TRICERATOPS_PANIC = &triceratops->PARAM_PANIC;		
		
		triceratops->synths[x].synth_params->TRICERATOPS_OSC1_ACTIVE = &triceratops->PARAM_OSC1_ACTIVE;
		triceratops->synths[x].synth_params->TRICERATOPS_OSC1_VOLUME = &triceratops->PARAM_OSC1_VOLUME;
		triceratops->synths[x].synth_params->TRICERATOPS_PULSEWIDTH = &triceratops->PARAM_OSC1_PULSEWIDTH;
		triceratops->synths[x].synth_params->TRICERATOPS_OSC1_WAVE; = &triceratops->PARAM_OSC1_WAVE;
		triceratops->synths[x].synth_params->TRICERATOPS_OSC1_OCTAVE = &triceratops->PARAM_OSC1_OCTAVE;
		triceratops->synths[x].synth_params->TRICERATOPS_OSC1_OCTAVE = &triceratops->PARAM_OSC1_OCTAVE;
		triceratops->synths[x].synth_params->TRICERATOPS_OSC1_DETUNE_CENTRE = &triceratops->PARAMS_OSC1_DETUNE_CENTRE;
		triceratops->synths[x].synth_params->TRICERATOPS_OSC1_INERTIA = &triceratops->PARAM_OSC1_INERTIA;

	}
	return triceratops;
}

END_NAMESPACE_DISTRHO





