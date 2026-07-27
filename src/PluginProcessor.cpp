// Copyright 2026 tilr

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Globals.h"

class MetaParameterBool : public juce::AudioParameterBool
{
public:
    using AudioParameterBool::AudioParameterBool;
    bool isMetaParameter() const override { return true; }
};

class MetaParameterChoice : public juce::AudioParameterChoice
{
public:
    using AudioParameterChoice::AudioParameterChoice;
    bool isMetaParameter() const override { return true; }
};

class MetaParameterFloat : public juce::AudioParameterFloat
{
public:
    using AudioParameterFloat::AudioParameterFloat;
    bool isMetaParameter() const override { return true; }
};

class MetaParameterInt : public juce::AudioParameterInt
{
public:
    using AudioParameterInt::AudioParameterInt;
    bool isMetaParameter() const override { return true; }
};

static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<AudioParameterInt>("polyphony", "Polyphony", 2, 32, 32));
    layout.add(std::make_unique<AudioParameterBool>("legato", "Legato", false));
    layout.add(std::make_unique<AudioParameterBool>("mono", "Mono", false));
    layout.add(std::make_unique<AudioParameterBool>("mpe", "MPE", false));

    layout.add(std::make_unique<AudioParameterFloat>("glide", "Glide", NormalisableRange<float>(0.0f, 8000.0f, 1.f, 0.5f), 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("glide_tension", "Glide Tension", -1.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("global_time", "Global Time", -1.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("global_pitch", "Global Time", -24.f, 24.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("vel_sense", "Vel Sense", 0.f, 1.f, 1.f));
    layout.add(std::make_unique<AudioParameterInt>("pitch_bend", "Pitch Bend", 1, 48, 2));
    layout.add(std::make_unique<AudioParameterFloat>("master_gain", "Master Gain", NormalisableRange<float>(0.f, 4.f, 0.001f, 0.5f), 1.f));

    layout.add(std::make_unique<AudioParameterChoice>("layout", "FM Layout", StringArray{ "A_B_C_D", "DCBA", "CA_DB",
        "DB_C_A", "DA_DB_DC", "BA_CA_DA", "A_CB_DC", "DC_CA_CB", "DC_DB_BA_CA", "DB_CB_BA", "Custom"}, 2));

    layout.add(std::make_unique<AudioParameterFloat>("fm_ab", "FM AB", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_ac", "FM AC", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_ad", "FM AD", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_aout", "FM AOut", 0.f, 1.f, 1.f));

    layout.add(std::make_unique<AudioParameterFloat>("fm_ba", "FM BA", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_bc", "FM BC", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_bd", "FM BD", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_bout", "FM BOut", 0.f, 1.f, 0.f));

    layout.add(std::make_unique<AudioParameterFloat>("fm_ca", "FM CA", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_cb", "FM CB", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_cd", "FM CD", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_cout", "FM COut", 0.f, 1.f, 0.f));

    layout.add(std::make_unique<AudioParameterFloat>("fm_da", "FM DA", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_db", "FM DB", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_dc", "FM DC", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("fm_dout", "FM DOut", 0.f, 1.f, 0.f));

    layout.add(std::make_unique<AudioParameterFloat>("rm_aa", "RM AA", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_ab", "RM AB", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_ac", "RM AC", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_ad", "RM AD", 0.f, 1.f, 0.f));

    layout.add(std::make_unique<AudioParameterFloat>("rm_ba", "RM BA", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_bb", "RM BB", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_bc", "RM BC", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_bd", "RM BD", 0.f, 1.f, 0.f));

    layout.add(std::make_unique<AudioParameterFloat>("rm_ca", "RM CA", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_cb", "RM CB", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_cc", "RM CC", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_cd", "RM CD", 0.f, 1.f, 0.f));

    layout.add(std::make_unique<AudioParameterFloat>("rm_da", "RM DA", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_db", "RM DB", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_dc", "RM DC", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<AudioParameterFloat>("rm_dd", "RM DD", 0.f, 1.f, 0.f));


    for (int i = 0; i < MAX_OSCILLATORS; ++i)
    {
        auto prefix = String(i == 0 ? "a_" : i == 1 ? "b_" : i == 2 ? "c_" : "d_");
        auto prefixnm = String(i == 0 ? "A " : i == 1 ? "B " : i == 2 ? "C " : "D ");
        layout.add(std::make_unique<AudioParameterBool>(prefix + "on", prefixnm + "On", i < 2));
        layout.add(std::make_unique<AudioParameterBool>(prefix + "fixed", prefixnm + "Fixed", false));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "level", prefixnm + "Level", 0.f, 1.f, i == 0 ? 1.f : 0.f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "pan", prefixnm + "Pan", 0.f, 1.f, 0.5f));
        layout.add(std::make_unique<AudioParameterInt>(prefix + "pitch_semis", prefixnm + "Pitch Semis", -36, 36, 0));
        layout.add(std::make_unique<AudioParameterInt>(prefix + "pitch_cents", prefixnm + "Pitch Cents", -100, 100, 0));
        layout.add(std::make_unique<AudioParameterInt>(prefix + "pitch_oct", prefixnm + "Pitch Cents", -4, 4, 0));
        layout.add(std::make_unique<AudioParameterBool>(prefix + "morph_snap", prefixnm + "Frame Snap", true));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "morph", prefixnm + "Morph", NormalisableRange<float>(0.f, 1.f), 0.f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "phase_offset", prefixnm + "Phase", 0.f, 1.f, 0.0f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "phase_rand", prefixnm + "Phase Rand", 0.f, 1.f, 1.f));
        layout.add(std::make_unique<AudioParameterChoice>(prefix + "phase_dist_mode", prefixnm + "Phase Dist Mode", StringArray{"Off", "Bend", "Skew", "Bias", "Pulse", "Sync", "Formant", "Quantize", "Fold" }, 0));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "phase_dist_amt", prefixnm + "Phase Dist Amt", -1.f, 1.f, 0.f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "feedback", prefixnm + "Feedback", 0.f, 1.f, 0.f));
        layout.add(std::make_unique<AudioParameterInt>(prefix + "unison_voices", prefixnm + "Unison Voices", 1, MAX_UNISON, 1));
        layout.add(std::make_unique<AudioParameterChoice>(prefix + "unison_mode", prefixnm + "Unison Mode", StringArray{ "Unison", "Gaussian", "Alternate", "5ths", "Sub" }, 0));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "unison_detune", prefixnm + "Unison Detune", 0.f, 1.f, 0.25f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "unison_stereo", prefixnm + "Unison Stereo", 0.f, 1.f, 1.f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "unison_spread", prefixnm + "Unison Warp", -1.f, 1.f, 0.3f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "unison_blend", prefixnm + "Unison Blend", 0.f, 1.f, 1.f));
    }

    for (int i = 0; i < MAX_FILTERS; ++i)
    {
        String prefix = i == 0 ? "f1_" : "f2_";
        String prefixnm = i == 0 ? "Filter1 " : "Filter2 ";
        layout.add(std::make_unique<AudioParameterBool>(prefix + "on", prefixnm + "On", false));
        layout.add(std::make_unique<AudioParameterBool>(prefix + "ktrack", prefixnm + "Keytrack", false));
        layout.add(std::make_unique<AudioParameterBool>(prefix + "inA", prefixnm + "In A", i < 1));
        layout.add(std::make_unique<AudioParameterBool>(prefix + "inB", prefixnm + "In B", i < 1));
        layout.add(std::make_unique<AudioParameterBool>(prefix + "inC", prefixnm + "In C", i < 1));
        layout.add(std::make_unique<AudioParameterBool>(prefix + "inD", prefixnm + "In D", i < 1));
        if (i > 0)
            layout.add(std::make_unique<AudioParameterBool>(prefix + "inF1", prefixnm + "In Filter1", true));

        layout.add(std::make_unique<AudioParameterChoice>(prefix + "type", prefixnm + "Type", StringArray{"Digital 12p", "Digital 24p", "Analog 12p", "Analog 24p", "Ladder 12p", "Ladder 24p", "303", "Phaser+", "Phaser-"}, 0));
        layout.add(std::make_unique<AudioParameterChoice>(prefix + "mode", prefixnm + "Mode", StringArray{"LP", "BP", "HP", "BS", "PK"}, 0));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "cut", prefixnm + "Cutoff", NormalisableRange<float>(20.f, 20000.f, 1.f, 0.35f), 2500.f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "res", prefixnm + "Resonance", 0.0, 1.f, 0.f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "drive", prefixnm + "Drive", 0.0, 1.f, 0.f));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "mix", prefixnm + "Mix", 0.0, 1.f, 1.f));
    }

    for (int i = 0; i < MAX_ENVELOPES; ++i)
    {
        layout.add(std::make_unique<AudioParameterChoice>("env" + juce::String(i + 1) + "_mode", "Env" + juce::String(i + 1) + " Mode", StringArray{ "ADSR", "AHD", "DADSR", "DAHDSR" }, 0));
        layout.add(std::make_unique<AudioParameterFloat>("env" + juce::String(i + 1) + "_del", "Env" + juce::String(i + 1) + " Delay", NormalisableRange<float>(0.0f, 5.f, 0.00001f, 0.3f), 0.f));
        layout.add(std::make_unique<AudioParameterFloat>("env" + juce::String(i + 1) + "_att", "Env" + juce::String(i + 1) + " Attack", NormalisableRange<float>(0.0f, 30.f, 0.00001f, 0.3f), .005f));
        layout.add(std::make_unique<AudioParameterFloat>("env" + juce::String(i + 1) + "_hld", "Env" + juce::String(i + 1) + " Hold", NormalisableRange<float>(0.0f, 5.f, 0.00001f, 0.3f), 0.f));
        layout.add(std::make_unique<AudioParameterFloat>("env" + juce::String(i + 1) + "_dec", "Env" + juce::String(i + 1) + " Decay", NormalisableRange<float>(0.0f, 30.f, 0.00001f, 0.3f), 0.15f));
        layout.add(std::make_unique<AudioParameterFloat>("env" + juce::String(i + 1) + "_sus", "Env" + juce::String(i + 1) + " Sustain", NormalisableRange<float>(0.0f, 1.0f), 0.8f));
        layout.add(std::make_unique<AudioParameterFloat>("env" + juce::String(i + 1) + "_rel", "Env" + juce::String(i + 1) + " Release", NormalisableRange<float>(0.001f, 30.f, 0.000001f, 0.3f), 0.015f));
        layout.add(std::make_unique<AudioParameterFloat>("env" + juce::String(i + 1) + "_tenatt", "Env" + juce::String(i + 1) + " Attack Tension", NormalisableRange<float>(-1.0f, 1.f), 0.0f));
        layout.add(std::make_unique<AudioParameterFloat>("env" + juce::String(i + 1) + "_tendec", "Env" + juce::String(i + 1) + " Decay Tension", NormalisableRange<float>(-1.0f, 1.f), -0.25f));
        layout.add(std::make_unique<AudioParameterFloat>("env" + juce::String(i + 1) + "_tenrel", "Env" + juce::String(i + 1) + " Release Tension", NormalisableRange<float>(-1.0f, 1.f), -0.25f));
    }

    layout.add(std::make_unique<AudioParameterInt>("lfo_grid", "LFO Grid", 2, 16, 8));
    layout.add(std::make_unique<AudioParameterBool>("lfo_grid_snap", "LFO Grid Snap", false));

    for (int i = 0; i < MAX_LFOS; ++i) {
        layout.add(std::make_unique<AudioParameterChoice>("lfo" + juce::String(i + 1) + "_mode", "LFO" + juce::String(i + 1) + " Sync", StringArray{ "Trigger", "Sync", "Envelope" }, 0));
        layout.add(std::make_unique<AudioParameterChoice>("lfo" + juce::String(i + 1) + "_sync", "LFO" + juce::String(i + 1) + " Sync", StringArray{ "RateHz", "Straight", "Tripplet", "Dotted" }, 1));
        layout.add(std::make_unique<AudioParameterFloat>("lfo" + juce::String(i + 1) + "_rate", "LFO" + juce::String(i + 1) + " Rate", NormalisableRange<float>(0.01f, 50.f, 0.00001f, 0.3f), 1.f));
        layout.add(std::make_unique<AudioParameterInt>("lfo" + juce::String(i + 1) + "_rate_sync", "LFO" + juce::String(i + 1) + " Rate Sync", 0, 10, 5)); // 16bar, 2bar, 1bar ... 1/64
        layout.add(std::make_unique<AudioParameterFloat>("lfo" + juce::String(i + 1) + "_smooth", "LFO" + juce::String(i + 1) + " Smooth", 0.f, 1.f, 0.f));
        layout.add(std::make_unique<AudioParameterFloat>("lfo" + juce::String(i + 1) + "_delay", "LFO" + juce::String(i + 1) + " Delay", 0.f, 4.f, 0.f));
        layout.add(std::make_unique<AudioParameterInt>("lfo" + juce::String(i + 1) + "_delay_sync", "LFO" + juce::String(i + 1) + " Delay Sync", 0, 11, 0)); // Off 1/64 1/32 1/16 1/8 ... 16Bar
        layout.add(std::make_unique<AudioParameterFloat>("lfo" + juce::String(i + 1) + "_rise", "LFO" + juce::String(i + 1) + " Rise", 0.f, 4.f, 0.f));
        layout.add(std::make_unique<AudioParameterInt>("lfo" + juce::String(i + 1) + "_rise_sync", "LFO" + juce::String(i + 1) + " Rise Sync", 0, 11, 0)); // Off 1/64 1/32 1/16 1/8 ... 16Bar
    }

    for (int i = 0; i < MAX_RNDS; ++i) {
        auto prefix = String("rnd") + String(i + 1);
        layout.add(std::make_unique<AudioParameterChoice>(prefix + "_mode", "Rand" + String(i + 1) + " Mode", StringArray{ "Perlin", "Hold" }, 0));
        layout.add(std::make_unique<AudioParameterBool>(prefix + "_global", "Rand" + String(i + 1) + " Global Sync", false));
        layout.add(std::make_unique<AudioParameterChoice>(prefix + "_sync", "Rand" + juce::String(i + 1) + " Sync", StringArray{ "RateHz", "Straight", "Tripplet", "Dotted" }, 1));
        layout.add(std::make_unique<AudioParameterFloat>(prefix + "_rate", "Rand" + juce::String(i + 1) + " Rate", NormalisableRange<float>(0.01f, 50.f, 0.00001f, 0.3f), 1.f));
        layout.add(std::make_unique<AudioParameterInt>(prefix + "_rate_sync", "Rand" + juce::String(i + 1) + " Rate Sync", 0, 10, 5)); // 16bar, 2bar, 1bar ... 1/64
    }

    for (int i = 0; i < MAX_MODULATIONS; ++i) {
        layout.add(std::make_unique<AudioParameterFloat>("mod" + juce::String(i + 1) + "_amt", "Mod" + juce::String(i + 1) + " Amount", NormalisableRange<float>(-1.f, 1.f, 0.00001f), 0.5f));
        layout.add(std::make_unique<AudioParameterFloat>("mod" + juce::String(i + 1) + "_ten", "Mod" + juce::String(i + 1) + " Curve", -1.f, 1.f, 0.0f));
    }

    for (int i = 0; i < MAX_MACROS; ++i) {
        layout.add(std::make_unique<AudioParameterFloat>("macro" + juce::String(i + 1), "Macro" + juce::String(i + 1), 0.0f, 1.0f, 0.0f));
    }

    // FX PARAMS
    layout.add(std::make_unique<juce::AudioParameterBool>("fx_chorus_on", "FX Chorus On", false));
    layout.add(std::make_unique<juce::AudioParameterChoice>("fx_chorus_voices", "FX Chorus Voices", juce::StringArray{ "2", "4", "8", "12", "16" }, 2));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_chorus_rate", "FX Chorus Rate", juce::NormalisableRange<float>(0.01f, 10.f, 0.01f, 0.3f), .35f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_chorus_depth", "FX Chorus Depth", 0.f, 1.f, 0.5));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_chorus_lowcut", "FX Chorus Lowcut", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.f, 0.3f), 20.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_chorus_highcut", "FX Chorus Highcut", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.f, 0.3f), 20000.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_chorus_feedback", "FX Chorus Feedback", -.95f, .95f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_chorus_mix", "FX Chorus Mix", 0.f, 1.f, 1.0f));

    layout.add(std::make_unique<juce::AudioParameterBool>("fx_delay_on", "FX Delay On", false));
    layout.add(std::make_unique<juce::AudioParameterChoice>("fx_delay_mode", "FX Delay Mode", juce::StringArray{ "Normal", "Ping Pong", "Tap" }, 0));
    layout.add(std::make_unique<MetaParameterBool>("fx_delay_link", "FX Delay Link", true));
    layout.add(std::make_unique<MetaParameterChoice>("fx_delay_sync_l", "FX Delay Sync", juce::StringArray{ "RateHz", "Straight", "Triplet", "Dotted" }, 1));
    layout.add(std::make_unique<MetaParameterChoice>("fx_delay_sync_r", "FX Delay Sync", juce::StringArray{ "RateHz", "Straight", "Triplet", "Dotted" }, 1));
    layout.add(std::make_unique<MetaParameterFloat>("fx_delay_rate_l", "FX Delay Rate", juce::NormalisableRange<float>(0.001f, 10.f, 0.001f, 0.3f), .5f));
    layout.add(std::make_unique<MetaParameterFloat>("fx_delay_rate_r", "FX Delay Rate", juce::NormalisableRange<float>(0.001f, 10.f, 0.001f, 0.3f), .5f));
    layout.add(std::make_unique<MetaParameterInt>("fx_delay_rate_sync_l", "FX Delay Rate Sync", 0, 8, 4)); // 2 bar, bar .... 1/128
    layout.add(std::make_unique<MetaParameterInt>("fx_delay_rate_sync_r", "FX Delay Rate Sync", 0, 8, 4));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_delay_feedback", "FX Delay Feedback", 0.f, 1.f, 0.35f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_delay_lowcut", "FX Delay Lowcut", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.f, 0.3f), 20.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_delay_highcut", "FX Delay Highcut", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.f, 0.3f), 20000.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_delay_pipo_width", "FX Delay Width", -1.f, 1.f, 1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_delay_mix", "FX Delay Mix", 0.f, 1.f, 0.5f));

    layout.add(std::make_unique<juce::AudioParameterBool>("fx_dist_on", "FX Dist On", false));
    layout.add(std::make_unique<juce::AudioParameterChoice>("fx_dist_mode", "FX Dist Mode", juce::StringArray{ "Tube", "SoftClip", "HardClip" }, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_dist_filter", "FX Dist Pre Filter", -1.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_dist_drive", "FX Dist Drive", 0.f, 40.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_dist_color", "FX Dist Color", -1.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_dist_gain", "FX Dist Gain", -40.f, 40.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_dist_mix", "FX Dist Mix", 0.f, 1.f, 1.f));

    layout.add(std::make_unique<juce::AudioParameterBool>("fx_comp_on", "FX Comp On", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("fx_comp_makeup", "FX Comp Makeup", true));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_comp_thresh", "FX Comp Thresh", -30.f, 0.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_comp_ratio", "FX Comp Ratio", juce::NormalisableRange<float>(1.f, 20.f, 0.01f, 0.4f), 4.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_comp_att", "FX Comp Attack", juce::NormalisableRange<float>(0.f, 500.f, 0.01f, 0.5f), 20.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_comp_rel", "FX Comp Release", juce::NormalisableRange<float>(0.f, 1000.f, 0.01f, 0.5f), 200.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_comp_gain", "FX Comp Gain", -24.f, 24.f, 0.f));

    layout.add(std::make_unique<juce::AudioParameterBool>("fx_reverb_on", "FX Reverb On", false));
    layout.add(std::make_unique<juce::AudioParameterInt>("fx_reverb_mode", "FX Reverb Mode", 0, 5, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_reverb_decay", "FX Reverb Decay", 0.f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_reverb_revsize", "FX Reverb Size", 0.f, 1.0f, 0.8f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_reverb_lowpass", "FX Reverb Lowpass", 0.f, 1.f, 0.6f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_reverb_damp", "FX Reverb Damp", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_reverb_density", "FX Reverb Density", 0.f, 1.f, 0.75f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_reverb_earlylate", "FX Reverb E/L Mix", 0.f, 1.f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_reverb_predel", "FX Reverb Pre-Delay", juce::NormalisableRange<float>(0.f, 1.f, 0.001f, 0.4f), 0.011f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_reverb_mix", "FX Reverb Mix", 0.f, 1.f, 0.5f));

    layout.add(std::make_unique<juce::AudioParameterBool>("fx_phaser_on", "FX Phaser On", false));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_phaser_center", "Phaser LFO Center", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.3f), 2000.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_phaser_depth", "Phaser LFO Depth", 0.f, 1.f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("fx_phaser_sync", "Phaser Sync", juce::StringArray{ "RateHz", "Straight", "Triplet", "Dotted" }, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_phaser_rate", "Phaser LFO Rate", juce::NormalisableRange<float>(0.01f, 10.f, 0.0001f, 0.5f), 1.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("fx_phaser_rate_sync", "Phaser Rate Sync", juce::StringArray{ "1/32", "1/16", "1/8", "1/4", "1/2", "1/1", "2/1", "4/1", "8/1", "16/1", "32/1" }, 3));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_phaser_res", "Phaser Resonance", 0.f, 1.f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_phaser_morph", "Phaser Morph", 0.f, 1.f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_phaser_stereo", "Phaser Stereo", 0.f, 1.f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("fx_phaser_mix", "Phaser Mix", 0.f, 1.f, 1.0f));

    auto getEQBandFreq = [](int band)
        {
            return 20.f * 2.f * std::pow(10000.f / 20.f / 2.f, band / (float)(globals::EQ_BANDS - 1));
        };

    layout.add(std::make_unique<juce::AudioParameterBool>("fx_eq_on", "FX EQ On", false));
    for (int i = 0; i < globals::EQ_BANDS; ++i) {
        auto paramPrefix = "fx_eq_band" + juce::String(i + 1);
        auto namePrefix = "FX EQ Band" + juce::String(i + 1);
        if (i == 0 || i == globals::EQ_BANDS - 1) {
            layout.add(std::make_unique<juce::AudioParameterChoice>(paramPrefix + "_mode", namePrefix + " Mode", juce::StringArray{ "Filter", "Shelf" }, 1));
        }
        layout.add(std::make_unique<juce::AudioParameterFloat>(paramPrefix + "_freq", namePrefix + " Freq", juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.3f), getEQBandFreq(i)));
        layout.add(std::make_unique<juce::AudioParameterFloat>(paramPrefix + "_q", namePrefix + " Q", 0.707f, 8.f, 0.707f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(paramPrefix + "_gain", namePrefix + " Gain", -globals::EQ_MAX_GAIN, globals::EQ_MAX_GAIN, 0.f));
    }

    return layout;
}


TetraOPAudioProcessor::TetraOPAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    )
    , settings{}
    , params(*this, &undoManager, "PARAMETERS", createParameterLayout())
    , mtsClientPtr{nullptr}
#endif
{
    juce::PropertiesFile::Options options{};
    options.applicationName = ProjectInfo::projectName;
    options.filenameSuffix = ".settings";
#if defined(JUCE_LINUX) || defined(JUCE_BSD)
    options.folderName = "~/.config/TetraOP";
#elif defined(JUCE_MAC) || defined(JUCE_IOS)
    options.folderName = "TetraOP";
#endif
    options.osxLibrarySubFolder = "Application Support";
    options.storageFormat = PropertiesFile::storeAsXML;
    settings.setStorageParameters(options);

    String presetsFolder = options
        .getDefaultFile()
        .getParentDirectory()
        .getChildFile("presets")
        .getFullPathName();

    String waveTablesFolder = options
        .getDefaultFile()
        .getParentDirectory()
        .getChildFile("wavetables")
        .getFullPathName();

    loadSettings();

    mtsClientPtr = MTS_RegisterClient();

    distoversampler = std::make_unique<juce::dsp::Oversampling<float>>(
        2, 1,
        juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR,
        true, true
    );

#ifdef JUCE_DEBUG
    int osstages = 0;
#else
    int osstages = 1;
#endif

    oversampler = std::make_unique<juce::dsp::Oversampling<float>>(
        2, osstages,
        juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR,
        true, true
    );

    for (int i = 0; i < FX::kFXs; i++) {
        fxOrder.push_back((FX::FXType)i);
        auto prefix = juce::String(FX::FXPrefix[i].data());
        params.addParameterListener(prefix + "on", this);
    }

    tablesMgr = std::make_unique<TablesManager>(*this, waveTablesFolder);
    presetmgr = std::make_unique<PresetManager>(*this, presetsFolder);

    modulation = std::make_unique<Modulation>(*this);
    synth = std::make_unique<Synth>(*this);

    params.addParameterListener("polyphony", this);
    params.addParameterListener("legato", this);
    params.addParameterListener("mono", this);
    params.addParameterListener("mpe", this);
    params.addParameterListener("glide", this);
    params.addParameterListener("pitch_bend", this);

    presetmgr->loadInit();
    undomgr = std::make_unique<UndoMgr>(*this);
    onFXChanged();
}

TetraOPAudioProcessor::~TetraOPAudioProcessor()
{
    MTS_DeregisterClient(mtsClientPtr);
}

void TetraOPAudioProcessor::parameterChanged(const juce::String& paramId, float value)
{
    (void)value;

    if (paramId.startsWith("fx_"))
        onFXChanged();
    else
        configsChanged = true;
}

// =================================================================

void TetraOPAudioProcessor::clearAll()
{
    synth->clear();

    for (auto& fx : fxchain)
        fx->clear();

    distoversampler->reset();
}

void TetraOPAudioProcessor::sortFX(std::vector <FX::FXType> sorted)
{
    fxOrder = normalizeFXOrder(std::move(sorted));
    rebuildFXChain();
    FXDirty.store(true);
}

void TetraOPAudioProcessor::onFXChanged()
{
    const juce::ScopedLock lock(dspLock);

    for (int i = 0; i < FX::kFXs; i++) {
        auto type = (FX::FXType)i;

        bool on = (bool)params.getRawParameterValue(String(FX::FXPrefix[i].data()) + "on")->load();

        if (on) {
            if (fxSlots[type] == nullptr)
                ensureFXExists(type);

            if (fxSlots[type] != nullptr) {
                fxSlots[type]->active.store(true);
            }
        }
        else if (fxSlots[type] != nullptr) {
            destroyFX(type);
        }
    }

    FXDirty.store(true);
}

std::vector<FX::FXType> TetraOPAudioProcessor::normalizeFXOrder(std::vector<FX::FXType> order) const
{
    std::vector<FX::FXType> normalized{};
    normalized.reserve(FX::kFXs);

    auto appendIfValid = [&](FX::FXType type)
        {
            if (type >= FX::kFXs)
                return;
            if (std::find(normalized.begin(), normalized.end(), type) != normalized.end())
                return;
            normalized.push_back(type);
        };

    for (auto type : order)
        appendIfValid(type);

    for (int i = 0; i < FX::kFXs; i++)
        appendIfValid((FX::FXType)i);

    return normalized;
}

std::unique_ptr<FX> TetraOPAudioProcessor::createFX(FX::FXType type)
{
    switch (type) {
        case FX::Compressor:  return std::make_unique<Compressor>(*this);
        case FX::Distortion:  return std::make_unique<Distortion>(*this);
        case FX::Chorus:      return std::make_unique<Chorus>(*this);
        case FX::Delay:       return std::make_unique<Delay>(*this);
        case FX::Reverb:      return std::make_unique<ReverbFX>(*this);
        case FX::Phaser:      return std::make_unique<PhaserFX>(*this);
        case FX::EQ:          return std::make_unique<EQ>(*this);
        default:
            jassertfalse;
            break;
    }

    return nullptr;
}

void TetraOPAudioProcessor::rebuildFXChain()
{
    fxchain.clear();

    for (auto type : fxOrder) {
        if (fxSlots[type] != nullptr)
            fxchain.push_back(fxSlots[type].get());
    }
}

void TetraOPAudioProcessor::ensureFXExists(FX::FXType type)
{
    if (fxSlots[type] != nullptr)
        return;

    fxSlots[type] = createFX(type);
    if (fxSlots[type] == nullptr)
        return;

    auto sampleRate = (float)getSampleRate();
    if (sampleRate > 0.f)
        fxSlots[type]->prepare(sampleRate);

    rebuildFXChain();
}

void TetraOPAudioProcessor::destroyFX(FX::FXType type)
{
    if (fxSlots[type] == nullptr)
        return;

    fxSlots[type].reset();
    rebuildFXChain();
}

// =================================================================

void TetraOPAudioProcessor::loadSettings ()
{
    if (auto* file = settings.getUserSettings()) {
        scale = (float)file->getDoubleValue("scale", 1.f);
        unboundedMouse = (bool)file->getBoolValue("unboundedMouse", true);
        importExportDir = file->getValue("importExportDir", "");
    }
}

void TetraOPAudioProcessor::saveSettings ()
{
    if (auto* file = settings.getUserSettings()) {
        file->setValue("version", PROJECT_VERSION);
        file->setValue("scale", scale);
        file->setValue("unboundedMouse", unboundedMouse);
        file->setValue("importExportDir", importExportDir);
    }
    settings.saveIfNeeded();
}

void TetraOPAudioProcessor::resetSettings()
{
    scale = 1.f;
    saveSettings();
}

void TetraOPAudioProcessor::loadTuningFile(String path)
{
    tuning.reset();

    if (path.isEmpty()) {
        tuningFileName = "";
        tuningFileFormat = "";
        tuningFileString = "";
        saveSettings();
        return;
    }

    try {
        String ext = File(path).getFileExtension();
        if (ext == ".scl") {
            auto scl = Tunings::readSCLFile(path.toStdString());
            tuning = std::make_unique<Tunings::Tuning>(Tunings::Tuning(scl));
            tuningFileString = String(scl.rawText);
        }
        else if (ext == ".kbm") {
            auto scl = Tunings::readKBMFile(path.toStdString());
            tuning = std::make_unique<Tunings::Tuning>(Tunings::Tuning(scl));
            tuningFileString = String(scl.rawText);
        }
        else {
            throw "Unkown format";
        }

        tuningFileName = File(path).getFileName();
        tuningFileFormat = File(path).getFileExtension();
        tuningFileDir = File(path).getParentDirectory().getFullPathName();
    }
    catch (...) {
        tuningFileName = "";
        tuningFileString = "";
        tuningFileFormat = "";
    }

    saveSettings();
}

void TetraOPAudioProcessor::loadTuningFileString(const std::string& str, const std::string& format)
{
    tuning.reset();
    try {
        if (format == ".scl") {
            std::istringstream stream(str);
            auto scl = Tunings::readSCLStream(stream);
            tuning = std::make_unique<Tunings::Tuning>(Tunings::Tuning(scl));
        }
        else if (format == ".kbm") {
            std::istringstream stream(str);
            auto scl = Tunings::readKBMStream(stream);
            tuning = std::make_unique<Tunings::Tuning>(Tunings::Tuning(scl));
        }
    }
    catch (...) {}
}

//==============================================================================
const juce::String TetraOPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TetraOPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TetraOPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TetraOPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TetraOPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TetraOPAudioProcessor::getNumPrograms()
{
    return 1;
}

int TetraOPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TetraOPAudioProcessor::setCurrentProgram (int index)
{
    (void)index;
}

const juce::String TetraOPAudioProcessor::getProgramName (int index)
{
    (void)index;
    return {};
}

void TetraOPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    (void)index;
    (void)newName;
}

//==============================================================================
void TetraOPAudioProcessor::prepareToPlay (double sampleRate, int _samplesPerBlock)
{
    clearAll();
    osfactor = (int)oversampler->getOversamplingFactor();
    samplesPerBlock = _samplesPerBlock;
    srate = (float)sampleRate;
    osrate = (float)sampleRate * osfactor;
    iosrate = 1.f / osrate;
    synth->setCurrentPlaybackSampleRate(sampleRate * osfactor);
    synth->prepare();
    modulation->prepare();

    for (auto& fx : fxchain) {
        fx->prepare((float)sampleRate);
    }

    oversampler->initProcessing(samplesPerBlock);
    oversampler->reset();

    distoversampler->initProcessing(samplesPerBlock);
    distoversampler->reset();

    tablesMgr->reloadWavetables();
}

void TetraOPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TetraOPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #endif

    return true;
  #endif
}
#endif

bool TetraOPAudioProcessor::supportsDoublePrecisionProcessing() const
{
    return false;
}

void TetraOPAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals disableDenormals;
    if (!dspLock.tryEnter())
    {
        blockMissed = true;
        return;
    }

    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    if (totalNumOutputChannels == 0 || numSamples == 0) {
        buffer.clear();
        return;
    }

    if (auto* phead = getPlayHead()) {
        if (auto pos = phead->getPosition()) {
            if (auto tempo = pos->getBpm()) {
                beatsPerSecond = *tempo / 60.0;
                secondsPerBeat = 60.0 / *tempo;
            }
            if (auto seconds = pos->getTimeInSeconds()) {
                timeInSeconds = *seconds;
            }
            else if (auto ppq = pos->getPpqPosition()) {
                if (auto tempo = pos->getBpm()) {
                    timeInSeconds = *ppq * (60.0 / *tempo); // fallback
                }
            }
            playing = pos->getIsPlaying();
        }
    }

    buffer.clear();

    if (configsChanged)
    {
        polyphony = (int)params.getRawParameterValue("polyphony")->load();
        mpe_enabled = (bool)params.getRawParameterValue("mpe")->load();
        bool mono = (bool)params.getRawParameterValue("mono")->load();
        float glide = params.getRawParameterValue("glide")->load();
        synth->setMono(mono);
        synth->setLegato((bool)params.getRawParameterValue("legato")->load());
        synth->setGlideRate(glide);
        synth->setPortamento(mono && glide > 0.f);
        synth->setPitchBendRange((int)params.getRawParameterValue("pitch_bend")->load());
        synth->setMPE(mpe_enabled);
        synth->setNumVoices(polyphony);
        configsChanged = false;
    }

    float masterGain = modulation->getValue("master_gain", false, numSamples, srate);
    int pos = 0;
    int todo = numSamples;

    juce::MidiBuffer oversampledMidi;
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        int safePos = juce::jlimit(0, buffer.getNumSamples() - 1, metadata.samplePosition);
        int newPos = safePos * osfactor;
        oversampledMidi.addEvent(message, newPos);
    }

    juce::dsp::AudioBlock<float> outputBlock(buffer);
    auto osBlock = oversampler->processSamplesUp(outputBlock);
    gin::ScratchBuffer osBuffer(2, numSamples * osfactor);

    synth->startBlock();
    while (todo > 0)
    {
        int thisBlock = std::min(todo, MAX_BLOCKSIZE / osfactor);

        currBlockSize = thisBlock;
        currBlockPos = pos;

        modulation->tick((double)osrate,
            thisBlock * osfactor,
            (float)secondsPerBeat);

        synth->renderNextBlock(
            osBuffer,
            oversampledMidi,
            pos * osfactor,
            thisBlock * osfactor);

        pos += thisBlock;
        todo -= thisBlock;

        modulation->endBlock(thisBlock * osfactor);
    }
    synth->endBlock(numSamples * osfactor);

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        std::memcpy(
            osBlock.getChannelPointer(ch),
            osBuffer.getWritePointer(ch),
            sizeof(float) * osBuffer.getNumSamples());
    }
    oversampler->processSamplesDown(outputBlock);

    processFx(buffer.getWritePointer(0), buffer.getWritePointer(1), numSamples);

    buffer.applyGain(masterGain);
    rmsL.store(buffer.getMagnitude(0, 0, numSamples));
    rmsR.store(buffer.getMagnitude(1, 0, numSamples));

    dspLock.exit();
}

void TetraOPAudioProcessor::processFx(float* bufL, float* bufR, int nsamples)
{
    for (auto& fx : fxchain) {
        if (fx->type == FX::Distortion) {
            float* channels[2] = { bufL, bufR };
            juce::dsp::AudioBlock<float> block(channels, 2, (size_t)nsamples);
            auto oversampledBlock = distoversampler->processSamplesUp(block);
            auto* osleft = oversampledBlock.getChannelPointer(0);
            auto* osright = oversampledBlock.getChannelPointer(1);

            fx->processBlock(osleft, osright, (int)oversampledBlock.getNumSamples(), 0, false);
            distoversampler->processSamplesDown(block);
        }
        else {
            fx->processBlock(bufL, bufR, nsamples, 0, false);
        }
    }
}

//==============================================================================
bool TetraOPAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* TetraOPAudioProcessor::createEditor()
{
    return new ScaledPluginEditor (new TetraOPAudioProcessorEditor (*this));
}

//==============================================================================
void TetraOPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = ValueTree("TETRAOP");
    auto paramsState = params.copyState();
    state.appendChild(paramsState, nullptr);
    state.setProperty("version", PROJECT_VERSION, nullptr);
    state.setProperty("name", presetmgr->selectedPreset.name, nullptr);

    auto lfos = ValueTree("LFOS");
    for (int i = 0; i < MAX_LFOS; ++i) {
        auto lfo = ValueTree(String("LFO" + String(i + 1)));
        lfo.setProperty("points", var(modulation->lfos[i].pattern.serialize()), nullptr);
        lfos.appendChild(lfo, nullptr);
    }
    state.appendChild(lfos, nullptr);
    state.appendChild(modulation->serialize(), nullptr);

    auto macros = ValueTree("MACROS");
    for (int i = 0; i < MAX_MACROS; ++i) {
        macros.setProperty(String("macro") + String(i + 1), modulation->macroNames[i], nullptr);
    }
    state.appendChild(macros, nullptr);

    auto tables = tablesMgr->serialize();
    state.appendChild(tables, nullptr);

    // serialize FX
    auto fxtree = juce::ValueTree("EFFECTS");
    juce::String s;
    for (int i = 0; i < int(fxOrder.size()); i++)
    {
        s << fxOrder[i];
        if (i < int(fxOrder.size()) - 1)
            s << ",";
    }
    fxtree.setProperty("master", s, nullptr);
    state.appendChild(fxtree, nullptr);

    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TetraOPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement>xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState == nullptr) { // Fallback to utf8 parsing
        auto xmlString = juce::String::fromUTF8(static_cast<const char*>(data), sizeInBytes);
        xmlState = juce::parseXML(xmlString);
    }

    if (!xmlState) return;
    auto state = ValueTree::fromXml(*xmlState);
    if (!state.isValid()) return;

    if (undomgr && !undomgr->isUndoing && presetmgr->selectedPreset.name.isNotEmpty()) {
        undomgr->createUndo();
    }

    isLoadingPreset = true;

    auto parameters = state.getChildWithName("PARAMETERS");
    auto lfos = state.getChildWithName("LFOS");
    auto mods = state.getChildWithName("MODULATIONS");
    auto macros = state.getChildWithName("MACROS");
    auto tables = state.getChildWithName("WAVETABLES");
    auto effects = state.getChildWithName("EFFECTS");

    if (parameters.isValid()) {
        params.replaceState(parameters);
    }

    if (lfos.isValid()) {
        for (int i = 0; i < MAX_LFOS; ++i) {
            auto lfo = lfos.getChildWithName("LFO" + String(i + 1));
            if (lfo.isValid()) {
                auto points = lfo.getProperty("points", "").toString().toStdString();
                if (!points.empty()) {
                    modulation->lfos[i].pattern.unserialize(points);
                }
            }
        }
    }

    if (mods.isValid()) {
        modulation->unserialize(mods);
    }

    for (int i = 0; i < MAX_MACROS; ++i) {
        modulation->macroNames[i] = String("Macro") + String(i + 1);
    }
    if (macros.isValid()) {
        for (int i = 0; i < MAX_MACROS; ++i) {
            String name = macros.getProperty("macro" + String(i + 1)).toString();
            if (name.isNotEmpty()) {
                modulation->macroNames[i] = name;
            }
        }
    }

    if (tables.isValid())
    {
        tablesMgr->unserialize(tables);
    }

    if (effects.isValid())
    {
        std::vector<FX::FXType> numbers;
        juce::String s = effects.getProperty("master").toString();
        juce::StringArray parts = juce::StringArray::fromTokens(s, ",", {});

        numbers.reserve(parts.size());
        for (auto& p : parts)
            numbers.push_back((FX::FXType)p.getIntValue());

        sortFX(numbers);
    }

    PresetManager::Preset preset;
    preset.id = state.getProperty("id", juce::Uuid().toString());
    preset.name = state.getProperty("name", "(Empty)");
    presetmgr->setSelected(preset);
    isLoadingPreset = false;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TetraOPAudioProcessor();
}
