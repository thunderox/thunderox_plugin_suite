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
		// Parameters
		float fParameters[kParameterCount];
		
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
			cout << fParameters[TRICERATOPS_OSC1_PULSEWIDTH] << "      -      " << synths[0].synth_params->TRICERATOPS_PULSEWIDTH_ONE[0] << endl;
			// cout << synths[0].synth_params->TRICERATOPS_ATTACK_TWO[0] << "," << fParameters[TRICERATOPS_ADSR2_ATTACK] << endl;
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



		int bpm;
		uint32_t buffer_length;
};

Plugin* createPlugin()
{
	triceratopsPlugin* triceratops = new triceratopsPlugin();
	for (int x=0; x<max_notes; x++)
	{
		triceratops->synths[x].synth_params->TRICERATOPS_MASTER_VOLUME = &triceratops->fParameters[TRICERATOPS_MASTER_VOLUME];
		triceratops->synths[x].synth_params->TRICERATOPS_AMP_DRIVE = &triceratops->fParameters[TRICERATOPS_AMP_DRIVE];
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_MODE = &triceratops->fParameters[TRICERATOPS_FILTER_MODE]				;
		triceratops->synths[x].synth_params->TRICERATOPS_CUTOFF = &triceratops->fParameters[TRICERATOPS_CUTOFF];
		triceratops->synths[x].synth_params->TRICERATOPS_RESONANCE = &triceratops->fParameters[TRICERATOPS_RESONANCE];		
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_KEY_FOLLOW =&triceratops->fParameters[TRICERATOPS_FILTER_KEY_FOLLOW];
		triceratops->synths[x].synth_params->TRICERATOPS_LEGATO = &triceratops->fParameters[TRICERATOPS_LEGATO];
		triceratops->synths[x].synth_params->TRICERATOPS_SYNC = &triceratops->fParameters[TRICERATOPS_SYNC];
		triceratops->synths[x].synth_params->TRICERATOPS_WARMTH = &triceratops->fParameters[TRICERATOPS_WARMTH];		
		triceratops->synths[x].synth_params->TRICERATOPS_FM = &triceratops->fParameters[TRICERATOPS_FM];
		triceratops->synths[x].synth_params->TRICERATOPS_PANIC = &triceratops->fParameters[TRICERATOPS_PANIC];		
		
		triceratops->synths[x].synth_params->TRICERATOPS_ACTIVE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_ACTIVE]; 
		triceratops->synths[x].synth_params->TRICERATOPS_VOLUME_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_VOLUME];
		triceratops->synths[x].synth_params->TRICERATOPS_PULSEWIDTH_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_PULSEWIDTH];
		triceratops->synths[x].synth_params->TRICERATOPS_OCTAVE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_OCTAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_DETUNE];
		triceratops->synths[x].synth_params->TRICERATOPS_OCTAVE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_OCTAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_CENTRE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_DETUNE_CENTRE];
		triceratops->synths[x].synth_params->TRICERATOPS_INERTIA_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_INERTIA];
		
				
		triceratops->synths[x].synth_params->TRICERATOPS_ACTIVE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_ACTIVE]; 
		triceratops->synths[x].synth_params->TRICERATOPS_VOLUME_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_VOLUME];
		triceratops->synths[x].synth_params->TRICERATOPS_PULSEWIDTH_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_PULSEWIDTH];
		triceratops->synths[x].synth_params->TRICERATOPS_OCTAVE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_OCTAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_DETUNE];
		triceratops->synths[x].synth_params->TRICERATOPS_OCTAVE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_OCTAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_CENTRE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_DETUNE_CENTRE];
		triceratops->synths[x].synth_params->TRICERATOPS_INERTIA_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_INERTIA];
		
				
		triceratops->synths[x].synth_params->TRICERATOPS_ACTIVE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_ACTIVE]; 
		triceratops->synths[x].synth_params->TRICERATOPS_VOLUME_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_VOLUME];
		triceratops->synths[x].synth_params->TRICERATOPS_PULSEWIDTH_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_PULSEWIDTH];
		triceratops->synths[x].synth_params->TRICERATOPS_OCTAVE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_OCTAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_DETUNE];
		triceratops->synths[x].synth_params->TRICERATOPS_OCTAVE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_OCTAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_CENTRE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_DETUNE_CENTRE];
		triceratops->synths[x].synth_params->TRICERATOPS_INERTIA_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_INERTIA];
		
		triceratops->synths[x].synth_params->TRICERATOPS_ATTACK_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_ATTACK];
		triceratops->synths[x].synth_params->TRICERATOPS_DECAY_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_SUSTAIN_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_SUSTAIN];
		triceratops->synths[x].synth_params->TRICERATOPS_RELEASE_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_RELEASE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR1_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR1_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_ADSR1_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR1_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_ADSR1_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR1_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_ADSR1_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_ATTACK_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_ATTACK];
		triceratops->synths[x].synth_params->TRICERATOPS_DECAY_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_SUSTAIN_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_SUSTAIN];
		triceratops->synths[x].synth_params->TRICERATOPS_RELEASE_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_RELEASE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR2_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_ADSR2_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR2_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_ADSR2_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR2_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR2_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_ADSR2_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_ATTACK_THREE = &triceratops->fParameters[TRICERATOPS_ADSR3_ATTACK];
		triceratops->synths[x].synth_params->TRICERATOPS_DECAY_THREE = &triceratops->fParameters[TRICERATOPS_ADSR3_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_SUSTAIN_THREE = &triceratops->fParameters[TRICERATOPS_ADSR3_SUSTAIN];
		triceratops->synths[x].synth_params->TRICERATOPS_RELEASE_THREE = &triceratops->fParameters[TRICERATOPS_ADSR3_RELEASE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR3_LFO1_AMOUNT = &triceratops->fParameters[TRICERATOPS_ADSR3_LFO1_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR3_LFO2_AMOUNT = &triceratops->fParameters[TRICERATOPS_ADSR3_LFO2_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR3_LFO3_AMOUNT = &triceratops->fParameters[TRICERATOPS_ADSR3_LFO3_AMOUNT];
		
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_RETRIG = &triceratops->fParameters[TRICERATOPS_LFO1_RETRIG];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_SPEED = &triceratops->fParameters[TRICERATOPS_LFO1_SPEED];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_WAVE = &triceratops->fParameters[TRICERATOPS_LFO1_WAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_OSC1_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO1_OSC1_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_OSC2_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO1_OSC2_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_OSC3_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO1_OSC3_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_FILTER = &triceratops->fParameters[TRICERATOPS_LFO1_FILTER];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_LFO1_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_LFO1_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_LFO1_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_LFO1_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_RETRIG = &triceratops->fParameters[TRICERATOPS_LFO2_RETRIG];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_SPEED = &triceratops->fParameters[TRICERATOPS_LFO2_SPEED];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_WAVE = &triceratops->fParameters[TRICERATOPS_LFO2_WAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_OSC1_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO2_OSC1_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_OSC2_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO2_OSC2_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_OSC3_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO2_OSC3_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_FILTER = &triceratops->fParameters[TRICERATOPS_LFO2_FILTER];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_LFO2_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_LFO2_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_LFO2_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_LFO2_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_RETRIG = &triceratops->fParameters[TRICERATOPS_LFO3_RETRIG];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_SPEED = &triceratops->fParameters[TRICERATOPS_LFO3_SPEED];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_WAVE = &triceratops->fParameters[TRICERATOPS_LFO3_WAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_OSC1_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO3_OSC1_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_OSC2_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO3_OSC2_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_OSC3_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO3_OSC3_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_FILTER = &triceratops->fParameters[TRICERATOPS_LFO3_FILTER];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_LFO3_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_LFO3_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_LFO3_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_LFO3_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_ACTIVE = &triceratops->fParameters[TRICERATOPS_FX_ECHO_ACTIVE];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_SPEED = &triceratops->fParameters[TRICERATOPS_FX_ECHO_SPEED];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_DECAY = &triceratops->fParameters[TRICERATOPS_FX_ECHO_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_EQ_LOW = &triceratops->fParameters[TRICERATOPS_FX_ECHO_EQ_LOW];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_EQ_MID = &triceratops->fParameters[TRICERATOPS_FX_ECHO_EQ_MID];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_EQ_HIGH = &triceratops->fParameters[TRICERATOPS_FX_ECHO_EQ_HIGH];
		
		triceratops->synths[x].synth_params->TRICERATOPS_UNISON_ACTIVATE = &triceratops->fParameters[TRICERATOPS_UNISON_ACTIVATE];
		triceratops->synths[x].synth_params->TRICERATOPS_UNISON_ONE = &triceratops->fParameters[TRICERATOPS_UNISON_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_UNISON_TWO = &triceratops->fParameters[TRICERATOPS_UNISON_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_UNISON_THREE = &triceratops->fParameters[TRICERATOPS_UNISON_THREE];
		
		triceratops->synths[x].synth_params->TRICERATOPS_MODIFIER_DIRT = &triceratops->fParameters[TRICERATOPS_MODIFIER_DIRT];

		triceratops->synths[x].synth_params->TRICERATOPS_FX_REVERB_ACTIVE = &triceratops->fParameters[TRICERATOPS_FX_REVERB_ACTIVE];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_REVERB_DECAY = &triceratops->fParameters[TRICERATOPS_FX_REVERB_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_REVERB_MIX = &triceratops->fParameters[TRICERATOPS_FX_REVERB_MIX];
		
		triceratops->synths[x].synth_params->TRICERATOPS_MODIFIER_STEREO_MODE = &triceratops->fParameters[TRICERATOPS_MODIFIER_STEREO_MODE];
		triceratops->synths[x].synth_params->TRICERATOPS_OSC1_PAN = &triceratops->fParameters[TRICERATOPS_OSC1_PAN];
		triceratops->synths[x].synth_params->TRICERATOPS_OSC2_PAN = &triceratops->fParameters[TRICERATOPS_OSC2_PAN];
		triceratops->synths[x].synth_params->TRICERATOPS_OSC3_PAN = &triceratops->fParameters[TRICERATOPS_OSC3_PAN];
		triceratops->synths[x].synth_params->TRICERATOPS_MODIFIER_RING = &triceratops->fParameters[TRICERATOPS_MODIFIER_RING];
		
		triceratops->synths[x].synth_params->TRICERATOPS_CATEGORY = &triceratops->fParameters[TRICERATOPS_PRESET_CATEGORY];
		triceratops->synths[x].synth_params->TRICERATOPS_PITCH_BEND_RANGE = &triceratops->fParameters[TRICERATOPS_PITCH_BEND_RANGE];
		triceratops->synths[x].synth_params->TRICERATOPS_MIDI_CHANNEL = &triceratops->fParameters[TRICERATOPS_MIDI_CHANNEL];
	}
	return triceratops;
}      

END_NAMESPACE_DISTRHO





