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
			
		int current_synth = 0;
		int old_synth = 0;
		
		float* pitch_bend = new float();
		float* channel_after_touch = new float();
	
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
			
			LFO* lfo1 = new LFO(srate);	
			LFO* lfo2 = new LFO(srate);
			LFO* lfo3 = new LFO(srate);
		
			float* lfo1_out;
			float* lfo2_out;
			float* lfo3_out;
			
			int lfo1_count = 0;
			int lfo2_count = 0;
			int lfo3_count = 0;
		
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
		
			float pitch_bend = 0;
			float channel_after_touch = 0;

			buffer_frame = 0;
			
			for (int x=0; x<16; ++x)
			{
				reverb[x] = new JCRev((float)x/3,srate);
				reverb[x]->clear();
				reverb[x]->setEffectMix(1.0);
			}
			
			for (int x=0; x<128; x++) { midi_keys[x] = -1; }
			
			for (int x=0; x<max_notes; x++)
			{
				synths[x].rate = srate;
				synths[x].lfo1_out = &lfo1_out;			
				synths[x].lfo2_out = &lfo2_out;	
				synths[x].lfo3_out = &lfo3_out;
				synths[x].pitch_bend = &pitch_bend;
				synths[x].channel_after_touch = &channel_after_touch;		
	
			}	
			
			
			// clear all parameters
			std::memset(fParameters, 0, sizeof(float)*kParameterCount);

			// we can know buffer-size right at the start
			fParameters[kParameterBufferSize] = getBufferSize();

		}
		


		const char* getLabel() const
		{
			return "ThunderOx Triceratops";
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


				case TRICERATOPS_MASTER_VOLUME:
					parameter.name   = "Volume";
					parameter.symbol = "master_volume";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.8f;
					fParameters[TRICERATOPS_MASTER_VOLUME] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_MASTER_TUNE:
					parameter.name   = "Master tune";
					parameter.symbol = "master_tune";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -7.0f;
					parameter.ranges.max = 7.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_MASTER_TUNE] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_FILTER_MODE:
					parameter.name   = "Filter mode";
					parameter.symbol = "filter_mode";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 5.0f;
					parameter.ranges.def = 1.0f;
					fParameters[TRICERATOPS_FILTER_MODE] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_FILTER_FREQUENCY:
					parameter.name   = "Filter cutoff";
					parameter.symbol = "cutoff";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.5f;
					fParameters[TRICERATOPS_FILTER_FREQUENCY] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_FILTER_RESONANCE:
					parameter.name   = "Filter resonance";
					parameter.symbol = "resonance";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_FILTER_RESONANCE] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_FILTER_KEY_FOLLOW:
					parameter.name   = "Filter key follow";
					parameter.symbol = "filter_key_follow";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -1.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_FILTER_KEY_FOLLOW] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_LEGATO:
					parameter.name   = "Legato";
					parameter.symbol = "legato";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LEGATO] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_SYNC:
					parameter.name   = "Sync";
					parameter.symbol = "sync";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_SYNC] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_WARMTH:
					parameter.name   = "Sync";
					parameter.symbol = "warmth";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 1.0f;
					fParameters[TRICERATOPS_WARMTH] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_FM:
					parameter.name   = "FM";
					parameter.symbol = "fm";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_FM] = parameter.ranges.def;
					break;
					
				case TRICERATOPS_PANIC:
					parameter.name   = "Panic";
					parameter.symbol = "panic";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_FM] = parameter.ranges.def;
					break;
					
					// -- OSCILATOR ONE
					
					case TRICERATOPS_OSC1_ACTIVE:
					parameter.name   = "Active 1";
					parameter.symbol = "active_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 1.0f;
					fParameters[TRICERATOPS_OSC1_ACTIVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC1_VOLUME:
					parameter.name   = "Volume 1";
					parameter.symbol = "volume_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.8f;
					fParameters[TRICERATOPS_OSC1_VOLUME] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC1_PULSEWIDTH:
					parameter.name   = "pulsewidth 1";
					parameter.symbol = "pulsewidth_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -0.5f;
					parameter.ranges.max = 0.5f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC1_PULSEWIDTH] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC1_WAVE:
					parameter.name   = "Waveform 1";
					parameter.symbol = "wave_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 5.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC1_WAVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC1_OCTAVE:
					parameter.name   = "Octave 1";
					parameter.symbol = "octave_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -5.0f;
					parameter.ranges.max = 5.0f;
					parameter.ranges.def = -2.0f;
					fParameters[TRICERATOPS_OSC1_OCTAVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC1_DETUNE:
					parameter.name   = "Detune 1";
					parameter.symbol = "detune_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -7.0f;
					parameter.ranges.max = 7.0f;
					parameter.ranges.def = -0.0f;
					fParameters[TRICERATOPS_OSC1_DETUNE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC1_INERTIA:
					parameter.name   = "Inertia 1";
					parameter.symbol = "inertia_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC1_INERTIA] = parameter.ranges.def;
					break;
					
					// -- OSCILATOR TWO
					
					case TRICERATOPS_OSC2_ACTIVE:
					parameter.name   = "Active 2";
					parameter.symbol = "active_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC2_ACTIVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC2_VOLUME:
					parameter.name   = "Volume 2";
					parameter.symbol = "volume_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.8f;
					fParameters[TRICERATOPS_OSC2_VOLUME] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC2_PULSEWIDTH:
					parameter.name   = "pulsewidth 2";
					parameter.symbol = "pulsewidth_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -0.5f;
					parameter.ranges.max = 0.5f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC2_PULSEWIDTH] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC2_WAVE:
					parameter.name   = "Waveform 2";
					parameter.symbol = "wave_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 5.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC2_WAVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC2_OCTAVE:
					parameter.name   = "Octave 2";
					parameter.symbol = "octave_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -5.0f;
					parameter.ranges.max = 5.0f;
					parameter.ranges.def = -2.0f;
					fParameters[TRICERATOPS_OSC2_OCTAVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC2_DETUNE:
					parameter.name   = "Detune 2";
					parameter.symbol = "detune_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -7.0f;
					parameter.ranges.max = 7.0f;
					parameter.ranges.def = -0.0f;
					fParameters[TRICERATOPS_OSC2_DETUNE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC2_INERTIA:
					parameter.name   = "Inertia 2";
					parameter.symbol = "inertia_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC2_INERTIA] = parameter.ranges.def;
					break;
					
					// -- OSCILATOR THREE
					
					case TRICERATOPS_OSC3_ACTIVE:
					parameter.name   = "Active 3";
					parameter.symbol = "active_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC3_ACTIVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC3_VOLUME:
					parameter.name   = "Volume 3";
					parameter.symbol = "volume_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.8f;
					fParameters[TRICERATOPS_OSC3_VOLUME] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC3_PULSEWIDTH:
					parameter.name   = "pulsewidth 3";
					parameter.symbol = "pulsewidth_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -0.5f;
					parameter.ranges.max = 0.5f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC3_PULSEWIDTH] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC3_WAVE:
					parameter.name   = "Waveform 3";
					parameter.symbol = "wave_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 5.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC3_WAVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC3_OCTAVE:
					parameter.name   = "Octave 3";
					parameter.symbol = "octave_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -5.0f;
					parameter.ranges.max = 5.0f;
					parameter.ranges.def = -2.0f;
					fParameters[TRICERATOPS_OSC3_OCTAVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC3_DETUNE:
					parameter.name   = "Detune 3";
					parameter.symbol = "detune_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = -7.0f;
					parameter.ranges.max = 7.0f;
					parameter.ranges.def = -0.0f;
					fParameters[TRICERATOPS_OSC3_DETUNE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_OSC3_INERTIA:
					parameter.name   = "Inertia 3";
					parameter.symbol = "inertia_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_OSC3_INERTIA] = parameter.ranges.def;
					break;
					
					// ADSR ONE
					
					case TRICERATOPS_ADSR1_ATTACK:
					parameter.name   = "Attack 1";
					parameter.symbol = "attack_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR1_ATTACK] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR1_DECAY:
					parameter.name   = "Decay 1";
					parameter.symbol = "decay_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.75f;
					fParameters[TRICERATOPS_ADSR1_DECAY] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR1_SUSTAIN:
					parameter.name   = "Sustain 1";
					parameter.symbol = "sustain_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR1_SUSTAIN] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR1_RELEASE:
					parameter.name   = "Release 1";
					parameter.symbol = "release_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.3f;
					fParameters[TRICERATOPS_ADSR1_RELEASE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR1_ROUTE_ONE:
					parameter.name   = "ADSR1 Route one";
					parameter.symbol = "adsr1_route_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 1.0f;
					fParameters[TRICERATOPS_ADSR1_ROUTE_ONE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR1_ROUTE_ONE_DEST:
					parameter.name   = "ADSR1 Route one dest";
					parameter.symbol = "adsr1_route_one_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 14.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR1_ROUTE_ONE_DEST] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR1_ROUTE_TWO:
					parameter.name   = "ADSR1 Route two";
					parameter.symbol = "adsr1_route_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 1.0f;
					fParameters[TRICERATOPS_ADSR1_ROUTE_TWO] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR1_ROUTE_TWO_DEST:
					parameter.name   = "ADSR1 Route two dest";
					parameter.symbol = "adsr1_route_two_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 14.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR1_ROUTE_TWO_DEST] = parameter.ranges.def;
					break;
						
					// ADSR TWO
					
					case TRICERATOPS_ADSR2_ATTACK:
					parameter.name   = "Attack 2";
					parameter.symbol = "attack_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR2_ATTACK] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR2_DECAY:
					parameter.name   = "Decay 2";
					parameter.symbol = "decay_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.75f;
					fParameters[TRICERATOPS_ADSR2_DECAY] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR2_SUSTAIN:
					parameter.name   = "Sustain 2";
					parameter.symbol = "sustain_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR2_SUSTAIN] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR2_RELEASE:
					parameter.name   = "Release 2";
					parameter.symbol = "release_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.3f;
					fParameters[TRICERATOPS_ADSR2_RELEASE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR2_ROUTE_ONE:
					parameter.name   = "ADSR2 Route one";
					parameter.symbol = "adsr2_route_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 1.0f;
					fParameters[TRICERATOPS_ADSR2_ROUTE_ONE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR2_ROUTE_ONE_DEST:
					parameter.name   = "ADSR2 Route one dest";
					parameter.symbol = "adsr2_route_one_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 14.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR2_ROUTE_ONE_DEST] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR2_ROUTE_TWO:
					parameter.name   = "ADSR2 Route two";
					parameter.symbol = "adsr2_route_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 1.0f;
					fParameters[TRICERATOPS_ADSR2_ROUTE_TWO] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR2_ROUTE_TWO_DEST:
					parameter.name   = "ADSR2 Route two dest";
					parameter.symbol = "adsr2_route_two_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 14.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR2_ROUTE_TWO_DEST] = parameter.ranges.def;
					break;
					// ADSR THREE
					
					case TRICERATOPS_ADSR3_ATTACK:
					parameter.name   = "Attack 3";
					parameter.symbol = "attack_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR3_ATTACK] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR3_DECAY:
					parameter.name   = "Decay 3";
					parameter.symbol = "decay_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.75f;
					fParameters[TRICERATOPS_ADSR3_DECAY] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR3_SUSTAIN:
					parameter.name   = "Sustain 3";
					parameter.symbol = "sustain_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR3_SUSTAIN] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR3_RELEASE:
					parameter.name   = "Release 3";
					parameter.symbol = "release_three";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.3f;
					fParameters[TRICERATOPS_ADSR3_RELEASE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR3_LFO1_AMOUNT:
					parameter.name   = "ADSR3 Route one";
					parameter.symbol = "adsr3_route_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR3_LFO1_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR3_LFO2_AMOUNT:
					parameter.name   = "ADSR3 Route one dest";
					parameter.symbol = "adsr3_route_one_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR3_LFO2_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_ADSR3_LFO3_AMOUNT:
					parameter.name   = "ADSR3 Route two";
					parameter.symbol = "adsr3_route_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_ADSR3_LFO3_AMOUNT] = parameter.ranges.def;
					break;
					
					// LFO ONE
					
					case TRICERATOPS_LFO1_RETRIG:
					parameter.name   = "LFO1 RETRIG";
					parameter.symbol = "lfo1_retrig";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO1_RETRIG] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_SPEED:
					parameter.name   = "LFO1 SPEED";
					parameter.symbol = "lfo1_speed";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 600.0f;
					parameter.ranges.def = 120.0f;
					fParameters[TRICERATOPS_LFO1_SPEED] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_WAVE:
					parameter.name   = "LFO1 WAVE";
					parameter.symbol = "lfo1_wave";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 6.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO1_WAVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_OSC1_AMOUNT:
					parameter.name   = "LFO1 TO DCO1 PITCH";
					parameter.symbol = "lfo1_dco1_pitch";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO1_OSC1_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_OSC2_AMOUNT:
					parameter.name   = "LFO1 TO DCO2 PITCH";
					parameter.symbol = "lfo1_dco2_pitch";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO1_OSC2_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_OSC3_AMOUNT:
					parameter.name   = "LFO1 TO DCO3 PITCH";
					parameter.symbol = "lfo1_dco3_pitch";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO1_OSC3_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_FILTER:
					parameter.name   = "LFO1 TO FILTER";
					parameter.symbol = "lfo1_filter";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO1_FILTER] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_ROUTE_ONE:
					parameter.name   = "LFO1 ROUTE ONE";
					parameter.symbol = "lfo1_route_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO1_ROUTE_ONE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_ROUTE_ONE_DEST:
					parameter.name   = "LFO1 ROUTE ONE DEST";
					parameter.symbol = "lfo1_route_one_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 10.0f;
					parameter.ranges.def = 2.0f;
					fParameters[TRICERATOPS_LFO1_ROUTE_ONE_DEST] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_ROUTE_TWO:
					parameter.name   = "LFO1 ROUTE TWO";
					parameter.symbol = "lfo1_route_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO1_ROUTE_TWO] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO1_ROUTE_TWO_DEST:
					parameter.name   = "LFO1 ROUTE TWO DEST";
					parameter.symbol = "lfo1_route_two_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 10.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO1_ROUTE_TWO_DEST] = parameter.ranges.def;
					break;
					
					// LFO TWO
					
					case TRICERATOPS_LFO2_RETRIG:
					parameter.name   = "LFO2 RETRIG";
					parameter.symbol = "lfo2_retrig";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO2_RETRIG] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_SPEED:
					parameter.name   = "LFO2 SPEED";
					parameter.symbol = "lfo2_speed";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 600.0f;
					parameter.ranges.def = 120.0f;
					fParameters[TRICERATOPS_LFO2_SPEED] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_WAVE:
					parameter.name   = "LFO2 WAVE";
					parameter.symbol = "lfo2_wave";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 6.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO2_WAVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_OSC1_AMOUNT:
					parameter.name   = "LFO2 TO DCO1 PITCH";
					parameter.symbol = "lfo2_dco1_pitch";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO2_OSC1_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_OSC2_AMOUNT:
					parameter.name   = "LFO2 TO DCO2 PITCH";
					parameter.symbol = "lfo2_dco2_pitch";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO2_OSC2_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_OSC3_AMOUNT:
					parameter.name   = "LFO2 TO DCO3 PITCH";
					parameter.symbol = "lfo2_dco3_pitch";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO2_OSC3_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_FILTER:
					parameter.name   = "LFO2 TO FILTER";
					parameter.symbol = "lfo2_filter";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO2_FILTER] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_ROUTE_ONE:
					parameter.name   = "LFO2 ROUTE ONE";
					parameter.symbol = "lfo2_route_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO2_ROUTE_ONE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_ROUTE_ONE_DEST:
					parameter.name   = "LFO2 ROUTE ONE DEST";
					parameter.symbol = "lfo2_route_one_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 10.0f;
					parameter.ranges.def = 2.0f;
					fParameters[TRICERATOPS_LFO2_ROUTE_ONE_DEST] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_ROUTE_TWO:
					parameter.name   = "LFO2 ROUTE TWO";
					parameter.symbol = "lfo2_route_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO2_ROUTE_TWO] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO2_ROUTE_TWO_DEST:
					parameter.name   = "LFO2 ROUTE TWO DEST";
					parameter.symbol = "lfo2_route_two_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 10.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO2_ROUTE_TWO_DEST] = parameter.ranges.def;
					break;
					
					// LFO THREE
					
					case TRICERATOPS_LFO3_RETRIG:
					parameter.name   = "LFO3 RETRIG";
					parameter.symbol = "lfo3_retrig";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO3_RETRIG] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_SPEED:
					parameter.name   = "LFO3 SPEED";
					parameter.symbol = "lfo3_speed";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 600.0f;
					parameter.ranges.def = 120.0f;
					fParameters[TRICERATOPS_LFO3_SPEED] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_WAVE:
					parameter.name   = "LFO3 WAVE";
					parameter.symbol = "lfo3_wave";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 6.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO3_WAVE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_OSC1_AMOUNT:
					parameter.name   = "LFO3 TO DCO1 PITCH";
					parameter.symbol = "lfo3_dco1_pitch";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO3_OSC1_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_OSC2_AMOUNT:
					parameter.name   = "LFO3 TO DCO2 PITCH";
					parameter.symbol = "lfo3_dco2_pitch";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO3_OSC2_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_OSC3_AMOUNT:
					parameter.name   = "LFO3 TO DCO3 PITCH";
					parameter.symbol = "lfo3_dco3_pitch";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO3_OSC3_AMOUNT] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_FILTER:
					parameter.name   = "LFO3 TO FILTER";
					parameter.symbol = "lfo3_filter";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO3_FILTER] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_ROUTE_ONE:
					parameter.name   = "LFO3 ROUTE ONE";
					parameter.symbol = "lfo3_route_one";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO3_ROUTE_ONE] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_ROUTE_ONE_DEST:
					parameter.name   = "LFO3 ROUTE ONE DEST";
					parameter.symbol = "lfo3_route_one_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 10.0f;
					parameter.ranges.def = 2.0f;
					fParameters[TRICERATOPS_LFO3_ROUTE_ONE_DEST] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_ROUTE_TWO:
					parameter.name   = "LFO3 ROUTE TWO";
					parameter.symbol = "lfo3_route_two";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO3_ROUTE_TWO] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_LFO3_ROUTE_TWO_DEST:
					parameter.name   = "LFO3 ROUTE TWO DEST";
					parameter.symbol = "lfo3_route_two_dest";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 10.0f;
					parameter.ranges.def = 0.0f;
					fParameters[TRICERATOPS_LFO3_ROUTE_TWO_DEST] = parameter.ranges.def;
					break;
					
					case TRICERATOPS_MIDI_CHANNEL:
					parameter.name   = "Midi channel";
					parameter.symbol = "midi_channel";
					parameter.hints = kParameterIsAutomable;
					parameter.ranges.min = 1.0f;
					parameter.ranges.max = 14.0f;
					parameter.ranges.def = 1.0f;
					fParameters[TRICERATOPS_MIDI_CHANNEL] = parameter.ranges.def;
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
			
			// cout << index << "," << value << endl;
			
			// cout << synths[0].synth_params->TRICERATOPS_DECAY_THREE[0] << "," << fParameters[TRICERATOPS_ADSR3_DECAY] << endl;
		}

		void run(const float** inputs, float** outputs, uint32_t frames,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override
		{
			// cout << getParameterValue(TRICERATOPS_ADSR1_DECAY) << endl;
			// memcpy(outputs[0], inputs[0], frames * sizeof(float));
						
			// cout << midiEvents << endl;
			
			float* out_left = outputs[0];
			float* out_right = outputs[1];
			
			memset( out_left, 0, sizeof(double)*(frames*0.5) );
			memset( out_right, 0, sizeof(double)*(frames*0.5) );
			
			if (fParameters[TRICERATOPS_FX_ECHO_ACTIVE])
			{
					echo->eq->lg = fParameters[TRICERATOPS_FX_ECHO_EQ_LOW];
					echo->eq->mg = fParameters[TRICERATOPS_FX_ECHO_EQ_MID];
					echo->eq->hg = fParameters[TRICERATOPS_FX_ECHO_EQ_HIGH];
					echo->eq2->lg = fParameters[TRICERATOPS_FX_ECHO_EQ_LOW];
					echo->eq2->mg = fParameters[TRICERATOPS_FX_ECHO_EQ_MID];
					echo->eq2->hg = fParameters[TRICERATOPS_FX_ECHO_EQ_HIGH];
	
			}			
			
			// MIDI 
			
			int midi_channel = fParameters[TRICERATOPS_MIDI_CHANNEL] - 1;

		        for (uint32_t i=0; i<midiEventCount; ++i)
		        
		        // PITCHBEND
		        {
        	        	const uint8_t* ev = midiEvents[i].data;
		        
		        	// PITCHBEND
		        	
				if ((int)ev[0] == 0xE0 + midi_channel) 
				{
					pitch_bend[0] = ((float)(((int)ev[2] * 128) + (int)ev[1]) / 8192)-1;		
					
					cout << pitch_bend[0] << endl;
				}
				
				if ((int)ev[0] == 0xD0 + midi_channel)
				{
					channel_after_touch[0] = ((float)((int)ev[1])/64);
				}
			
				// NOTE ON / OFF

		        	if ((int)ev[0]  == 0x90 + midi_channel && (int)ev[2] > 0)
				{
					cout << "NOTE ON!" << endl; 
					
					for (int x=0; x<max_notes; ++x)
					{
						if (synths[x].env_amp_state  == synths[x].env_state_release
							&& synths[x].env_amp_level < 0.5
								|| synths[x].env_amp_state == synths[x].env_state_dormant)
						{
							current_synth = x;
						}
						
						if (fParameters[TRICERATOPS_LEGATO] == 1)
						{
							current_synth = 0;
						}
					
						synths[current_synth].update_counter = -1;
	
						midi_keys[(int)ev[1]] = current_synth;
						synths[current_synth].midi_key = (int)ev[1];
						synths[current_synth].velocity =  (int)ev[2];					
					}
					
					cout << current_synth << endl;
					
				}
				else if ((int)ev[0] == 0x80 + midi_channel || ((int)ev[0]  & 0xF0 == 0x90 && (int)ev[2] == 0))
				{
					cout << "NOTE OFF!" << endl; 
				}
				

		        	
			}
			/*

			float* out_left = outputs[0];
			float* out_right = outputs[1];

			float froq = 0;

			for (uint32_t x=0; x<frames; x++)
			{
				froq += 0.01;
				
				if (froq > 0.9) froq = -9;
				
				out_left[x] = froq;
				out_right[x] = -froq;
			}
			
			char b1 = midiEvents->data[0];
			bool eventType = (b1 == 0x9);
			if (eventType)
				{
				int gerbil;
			
			
			
			// cout << "Yes it is!" << endl;
			}
			
			*/
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
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_MODE = &triceratops->fParameters[TRICERATOPS_FILTER_MODE];
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_FREQUENCY = &triceratops->fParameters[TRICERATOPS_FILTER_FREQUENCY];
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_RESONANCE = &triceratops->fParameters[TRICERATOPS_FILTER_RESONANCE];		
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





