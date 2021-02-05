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
	return triceratops;
}

END_NAMESPACE_DISTRHO





