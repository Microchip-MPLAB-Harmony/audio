# coding: utf-8
##############################################################################
# Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
# THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
##############################################################################
import os
import sys

dbg = 0

src_ext = ('.c')
hdr_ext = ('.h')
lib_ext = ('.a')
imp_ext = ('.gpl', '.xiph')



wavTable     = [("LIB_", "wav/", "wav_dec.h", "audio/decoder/audio_decoders/wav"),
                ("LIB_", "wav/", "wav_dec.c", "audio/decoder/audio_decoders/wav")]

adpcmTable   = [("LIB_", "adpcm/", "adpcm_dec.h", "audio/decoder/audio_decoders/adpcm"),
                ("LIB_", "adpcm/", "adpcm_dec.c", "audio/decoder/audio_decoders/adpcm")]

flacTable    = [("LIB_", "flac/", "flac_dec.h", "audio/decoder/audio_decoders/flac"),
                ("LIB_", "flac/", "flac_dec.c", "audio/decoder/audio_decoders/flac"),
                ("LIB_", "flac/src/", "bitmath.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "bitreader.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "cpu.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "crc.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "fixed.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "float.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "format.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "lpc.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "main.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "md5.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "memory.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/src/", "stream_decoder.c", "audio/decoder/audio_decoders/flac/src"),
                ("LIB_", "flac/include/", "config.h", "audio/decoder/audio_decoders/flac/include"),
                ("LIB_", "flac/include/FLAC/", "all.h", "audio/decoder/audio_decoders/flac/include/FLAC"),
                ("LIB_", "flac/include/FLAC/", "assert.h", "audio/decoder/audio_decoders/flac/include/FLAC"),
                ("LIB_", "flac/include/FLAC/", "callback.h", "audio/decoder/audio_decoders/flac/include/FLAC"),
                ("LIB_", "flac/include/FLAC/", "export.h", "audio/decoder/audio_decoders/flac/include/FLAC"),
                ("LIB_", "flac/include/FLAC/", "format.h", "audio/decoder/audio_decoders/flac/include/FLAC"),
                ("LIB_", "flac/include/FLAC/", "metadata.h", "audio/decoder/audio_decoders/flac/include/FLAC"),
                ("LIB_", "flac/include/FLAC/", "ordinals.h", "audio/decoder/audio_decoders/flac/include/FLAC"),
                ("LIB_", "flac/include/FLAC/", "stream_decoder.h", "audio/decoder/audio_decoders/flac/include/FLAC"),
                ("LIB_", "flac/include/FLAC/", "stream_encoder.h", "audio/decoder/audio_decoders/flac/include/FLAC"),
                ("LIB_", "flac/include/private/", "all.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "bitmath.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "bitreader.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "bitwriter.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "cpu.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "crc.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "fixed.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "float.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "format.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "lpc.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "macros.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "md5.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "memory.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "metadata.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "ogg_decoder_aspect.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "ogg_encoder_aspect.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "ogg_helper.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "ogg_mapping.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "stream_encoder.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "stream_encoder_framing.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/private/", "window.h", "audio/decoder/audio_decoders/flac/include/private"),
                ("LIB_", "flac/include/protected/", "all.h", "audio/decoder/audio_decoders/flac/include/protected"),
                ("LIB_", "flac/include/protected/", "stream_decoder.h", "audio/decoder/audio_decoders/flac/include/protected"),
                ("LIB_", "flac/include/protected/", "stream_encoder.h", "audio/decoder/audio_decoders/flac/include/protected"),
                ("LIB_", "flac/include/share/", "alloc.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "compat.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "endswap.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "getopt.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "grabbag.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "macros.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "private.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "replaygain_analysis.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "replaygain_synthesis.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "safe_str.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "utf8.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/", "win_utf8_io.h", "audio/decoder/audio_decoders/flac/include/share"),
                ("LIB_", "flac/include/share/grabbag/", "cuesheet.h", "audio/decoder/audio_decoders/flac/include/share/grabbag"),
                ("LIB_", "flac/include/share/grabbag/", "file.h", "audio/decoder/audio_decoders/flac/include/share/grabbag"),
                ("LIB_", "flac/include/share/grabbag/", "picture.h", "audio/decoder/audio_decoders/flac/include/share/grabbag"),
                ("LIB_", "flac/include/share/grabbag/", "replaygain.h", "audio/decoder/audio_decoders/flac/include/share/grabbag"),
                ("LIB_", "flac/include/share/grabbag/", "seektable.h", "audio/decoder/audio_decoders/flac/include/share/grabbag")]

opusTable    = [("LIB_", "opus/", "opus_dec.h", "audio/decoder/audio_decoders/opus"),
                ("LIB_", "opus/src/src/", "analysis.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "mlp.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "mlp_data.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "opus.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "opus_compare.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "opus_decoder.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "opus_demo.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "opus_encoder.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "opus_multistream.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "opus_multistream_decoder.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "opus_multistream_encoder.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "repacketizer.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "repacketizer_demo.c", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "analysis.h", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "mlp.h", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "opus_private.h", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/src/", "tansig_table.h", "audio/decoder/audio_decoders/opus/src/src"),
                ("LIB_", "opus/src/silk/", "A2NLSF.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "ana_filt_bank_1.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "API.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "biquad_alt.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "bwexpander.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "bwexpander_32.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "check_control_input.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "CNG.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "code_signs.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "control.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "control_audio_bandwidth.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "control_codec.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "control_SNR.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "debug.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "debug.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "decoder_set_fs.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "decode_core.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "decode_frame.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "decode_indices.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "decode_parameters.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "decode_pitch.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "decode_pulses.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "dec_API.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "define.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "encode_indices.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "encode_pulses.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "enc_API.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "errors.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "gain_quant.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "HP_variable_cutoff.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "init_decoder.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "init_encoder.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "Inlines.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "inner_prod_aligned.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "interpolate.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "lin2log.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "log2lin.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "LPC_analysis_filter.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "LPC_inv_pred_gain.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "LP_variable_cutoff.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "MacroCount.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "MacroDebug.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "macros.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "main.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NLSF2A.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NLSF_decode.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NLSF_del_dec_quant.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NLSF_encode.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NLSF_stabilize.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NLSF_unpack.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NLSF_VQ.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NLSF_VQ_weights_laroia.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NSQ.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "NSQ_del_dec.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "pitch_est_defines.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "pitch_est_tables.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "PLC.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "PLC.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "process_NLSFs.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "quant_LTP_gains.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_down2.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_down2_3.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_private.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_private_AR2.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_private_down_FIR.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_private_IIR_FIR.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_private_up2_HQ.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_rom.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_rom.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "resampler_structs.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "shell_coder.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "sigm_Q15.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "SigProc_FIX.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "sort.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "stereo_decode_pred.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "stereo_encode_pred.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "stereo_find_predictor.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "stereo_LR_to_MS.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "stereo_MS_to_LR.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "stereo_quant_pred.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "structs.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "sum_sqr_shift.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "tables.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "tables_gain.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "tables_LTP.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "tables_NLSF_CB_NB_MB.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "tables_NLSF_CB_WB.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "tables_other.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "tables_pitch_lag.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "tables_pulses_per_block.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "table_LSF_cos.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "tuning_parameters.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "typedef.h", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "VAD.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/", "VQ_WMat_EC.c", "audio/decoder/audio_decoders/opus/src/silk"),
                ("LIB_", "opus/src/silk/x86/", "main_sse.h", "audio/decoder/audio_decoders/opus/src/silk/x86"),
                ("LIB_", "opus/src/silk/x86/", "NSQ_del_dec_sse.c", "audio/decoder/audio_decoders/opus/src/silk/x86"),
                ("LIB_", "opus/src/silk/x86/", "NSQ_sse.c", "audio/decoder/audio_decoders/opus/src/silk/x86"),
                ("LIB_", "opus/src/silk/x86/", "SigProc_FIX_sse.h", "audio/decoder/audio_decoders/opus/src/silk/x86"),
                ("LIB_", "opus/src/silk/x86/", "VAD_sse.c", "audio/decoder/audio_decoders/opus/src/silk/x86"),
                ("LIB_", "opus/src/silk/x86/", "VQ_WMat_EC_sse.c", "audio/decoder/audio_decoders/opus/src/silk/x86"),
                ("LIB_", "opus/src/silk/x86/", "x86_silk_map.c", "audio/decoder/audio_decoders/opus/src/silk/x86"),
                ("LIB_", "opus/src/silk/mips/", "macros_mipsr1.h", "audio/decoder/audio_decoders/opus/src/silk/mips"),
                ("LIB_", "opus/src/silk/mips/", "NSQ_del_dec_mipsr1.h", "audio/decoder/audio_decoders/opus/src/silk/mips"),
                ("LIB_", "opus/src/silk/mips/", "sigproc_fix_mipsr1.h", "audio/decoder/audio_decoders/opus/src/silk/mips"),
                ("LIB_", "opus/src/silk/float/", "apply_sine_window_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "autocorrelation_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "burg_modified_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "bwexpander_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "corrMatrix_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "encode_frame_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "energy_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "find_LPC_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "find_LTP_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "find_pitch_lags_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "find_pred_coefs_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "inner_product_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "k2a_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "levinsondurbin_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "LPC_analysis_filter_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "LPC_inv_pred_gain_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "LTP_analysis_filter_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "LTP_scale_ctrl_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "main_FLP.h", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "noise_shape_analysis_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "pitch_analysis_core_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "prefilter_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "process_gains_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "regularize_correlations_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "residual_energy_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "scale_copy_vector_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "scale_vector_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "schur_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "SigProc_FLP.h", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "solve_LS_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "sort_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "structs_FLP.h", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "warped_autocorrelation_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/float/", "wrappers_FLP.c", "audio/decoder/audio_decoders/opus/src/silk/float"),
                ("LIB_", "opus/src/silk/fixed/", "apply_sine_window_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "autocorr_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "burg_modified_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "corrMatrix_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "encode_frame_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "find_LPC_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "find_LTP_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "find_pitch_lags_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "find_pred_coefs_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "k2a_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "k2a_Q16_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "LTP_analysis_filter_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "LTP_scale_ctrl_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "main_FIX.h", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "noise_shape_analysis_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "pitch_analysis_core_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "prefilter_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "process_gains_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "regularize_correlations_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "residual_energy16_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "residual_energy_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "schur64_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "schur_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "solve_LS_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "structs_FIX.h", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "vector_ops_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/", "warped_autocorrelation_FIX.c", "audio/decoder/audio_decoders/opus/src/silk/fixed"),
                ("LIB_", "opus/src/silk/fixed/x86/", "burg_modified_FIX_sse.c", "audio/decoder/audio_decoders/opus/src/silk/fixed/x86"),
                ("LIB_", "opus/src/silk/fixed/x86/", "prefilter_FIX_sse.c", "audio/decoder/audio_decoders/opus/src/silk/fixed/x86"),
                ("LIB_", "opus/src/silk/fixed/x86/", "vector_ops_FIX_sse.c", "audio/decoder/audio_decoders/opus/src/silk/fixed/x86"),
                ("LIB_", "opus/src/silk/fixed/mips/", "noise_shape_analysis_FIX_mipsr1.h", "audio/decoder/audio_decoders/opus/src/silk/fixed/mips"),
                ("LIB_", "opus/src/silk/fixed/mips/", "prefilter_FIX_mipsr1.h", "audio/decoder/audio_decoders/opus/src/silk/fixed/mips"),
                ("LIB_", "opus/src/silk/fixed/mips/", "warped_autocorrelation_FIX_mipsr1.h", "audio/decoder/audio_decoders/opus/src/silk/fixed/mips"),
                ("LIB_", "opus/src/silk/arm/", "macros_armv4.h", "audio/decoder/audio_decoders/opus/src/silk/arm"),
                ("LIB_", "opus/src/silk/arm/", "macros_armv5e.h", "audio/decoder/audio_decoders/opus/src/silk/arm"),
                ("LIB_", "opus/src/silk/arm/", "SigProc_FIX_armv4.h", "audio/decoder/audio_decoders/opus/src/silk/arm"),
                ("LIB_", "opus/src/silk/arm/", "SigProc_FIX_armv5e.h", "audio/decoder/audio_decoders/opus/src/silk/arm"),
                ("LIB_", "opus/src/celt/", "arch.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "bands.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "bands.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "celt.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "celt.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "celt_decoder.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "celt_encoder.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "celt_lpc.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "celt_lpc.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "cpu_support.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "cwrs.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "cwrs.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "ecintrin.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "entcode.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "entcode.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "entdec.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "entdec.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "entenc.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "entenc.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "fixed_debug.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "fixed_generic.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "float_cast.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "kiss_fft.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "kiss_fft.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "laplace.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "laplace.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "mathops.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "mathops.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "mdct.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "mdct.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "mfrngcod.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "modes.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "modes.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "opus_custom_demo.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "os_support.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "pitch.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "pitch.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "quant_bands.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "quant_bands.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "rate.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "rate.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "stack_alloc.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "static_modes_fixed.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "static_modes_fixed_arm_ne10.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "static_modes_float.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "static_modes_float_arm_ne10.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "vq.c", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "vq.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/", "_kiss_fft_guts.h", "audio/decoder/audio_decoders/opus/src/celt"),
                ("LIB_", "opus/src/celt/x86/", "celt_lpc_sse.c", "audio/decoder/audio_decoders/opus/src/celt/x86"),
                ("LIB_", "opus/src/celt/x86/", "celt_lpc_sse.h", "audio/decoder/audio_decoders/opus/src/celt/x86"),
                ("LIB_", "opus/src/celt/x86/", "pitch_sse.c", "audio/decoder/audio_decoders/opus/src/celt/x86"),
                ("LIB_", "opus/src/celt/x86/", "pitch_sse.h", "audio/decoder/audio_decoders/opus/src/celt/x86"),
                ("LIB_", "opus/src/celt/x86/", "pitch_sse2.c", "audio/decoder/audio_decoders/opus/src/celt/x86"),
                ("LIB_", "opus/src/celt/x86/", "pitch_sse4_1.c", "audio/decoder/audio_decoders/opus/src/celt/x86"),
                ("LIB_", "opus/src/celt/x86/", "x86cpu.c", "audio/decoder/audio_decoders/opus/src/celt/x86"),
                ("LIB_", "opus/src/celt/x86/", "x86cpu.h", "audio/decoder/audio_decoders/opus/src/celt/x86"),
                ("LIB_", "opus/src/celt/x86/", "x86_celt_map.c", "audio/decoder/audio_decoders/opus/src/celt/x86"),
                ("LIB_", "opus/src/celt/tests/", "test_unit_cwrs32.c", "audio/decoder/audio_decoders/opus/src/celt/tests"),
                ("LIB_", "opus/src/celt/tests/", "test_unit_dft.c", "audio/decoder/audio_decoders/opus/src/celt/tests"),
                ("LIB_", "opus/src/celt/tests/", "test_unit_entropy.c", "audio/decoder/audio_decoders/opus/src/celt/tests"),
                ("LIB_", "opus/src/celt/tests/", "test_unit_laplace.c", "audio/decoder/audio_decoders/opus/src/celt/tests"),
                ("LIB_", "opus/src/celt/tests/", "test_unit_mathops.c", "audio/decoder/audio_decoders/opus/src/celt/tests"),
                ("LIB_", "opus/src/celt/tests/", "test_unit_mdct.c", "audio/decoder/audio_decoders/opus/src/celt/tests"),
                ("LIB_", "opus/src/celt/tests/", "test_unit_rotation.c", "audio/decoder/audio_decoders/opus/src/celt/tests"),
                ("LIB_", "opus/src/celt/tests/", "test_unit_types.c", "audio/decoder/audio_decoders/opus/src/celt/tests"),
                ("LIB_", "opus/src/celt/mips/", "celt_mipsr1.h", "audio/decoder/audio_decoders/opus/src/celt/mips"),
                ("LIB_", "opus/src/celt/mips/", "fixed_generic_mipsr1.h", "audio/decoder/audio_decoders/opus/src/celt/mips"),
                ("LIB_", "opus/src/celt/mips/", "kiss_fft_mipsr1.h", "audio/decoder/audio_decoders/opus/src/celt/mips"),
                ("LIB_", "opus/src/celt/mips/", "mdct_mipsr1.h", "audio/decoder/audio_decoders/opus/src/celt/mips"),
                ("LIB_", "opus/src/celt/mips/", "pitch_mipsr1.h", "audio/decoder/audio_decoders/opus/src/celt/mips"),
                ("LIB_", "opus/src/celt/mips/", "vq_mipsr1.h", "audio/decoder/audio_decoders/opus/src/celt/mips"),
                ("LIB_", "opus/src/celt/arm/", "armcpu.c", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "armcpu.h", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "arm_celt_map.c", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "celt_ne10_fft.c", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "celt_ne10_mdct.c", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "celt_neon_intr.c", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "fft_arm.h", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "fixed_armv4.h", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "fixed_armv5e.h", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "kiss_fft_armv4.h", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "kiss_fft_armv5e.h", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "mdct_arm.h", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/src/celt/arm/", "pitch_arm.h", "audio/decoder/audio_decoders/opus/src/celt/arm"),
                ("LIB_", "opus/include/", "opus.h", "audio/decoder/audio_decoders/opus/include"),
                ("LIB_", "opus/include/", "opus_custom.h", "audio/decoder/audio_decoders/opus/include"),
                ("LIB_", "opus/include/", "opus_defines.h", "audio/decoder/audio_decoders/opus/include"),
                ("LIB_", "opus/include/", "opus_multistream.h", "audio/decoder/audio_decoders/opus/include"),
                ("LIB_", "opus/include/", "opus_types.h", "audio/decoder/audio_decoders/opus/include")]

speexTable   = [("LIB_", "speex/", "speex_dec.c", "audio/decoder/audio_decoders/speex"),
                ("LIB_", "speex/", "speex_dec.h", "audio/decoder/audio_decoders/speex"),
                ("LIB_", "speex/libspeex/", "arch.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "bits.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "cb_search.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "cb_search.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "cb_search_arm4.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "cb_search_bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "cb_search_sse.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "exc_10_16_table.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "exc_10_32_table.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "exc_20_32_table.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "exc_5_256_table.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "exc_5_64_table.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "exc_8_128_table.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "fftwrap.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "filters.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "filters.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "filters_arm4.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "filters_bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "filters_sse.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "fixed_arm4.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "fixed_arm5e.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "fixed_bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "fixed_debug.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "fixed_generic.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "gain_table.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "gain_table_lbr.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "hexc_10_32_table.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "hexc_table.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "high_lsp_tables.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "kiss_fft.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "kiss_fft.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "kiss_fftr.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "kiss_fftr.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "lpc.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "lpc.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "lpc_bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "lsp.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "lsp.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "lsp_bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "lsp_tables_nb.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "ltp.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "ltp.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "ltp_arm4.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "ltp_bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "ltp_sse.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "math_approx.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "misc_bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "modes.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "modes.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "modes_wb.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "nb_celp.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "nb_celp.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "os_support.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "quant_lsp.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "quant_lsp.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "quant_lsp_bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "sb_celp.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "sb_celp.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "smallft.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "smallft.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "speex.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "speex_callbacks.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "speex_header.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "stack_alloc.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "stereo.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "testenc.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "testenc_uwb.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "testenc_wb.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "vbr.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "vbr.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "vorbis_psy.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "vorbis_psy.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "vq.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "vq.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "vq_arm4.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "vq_bfin.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "vq_sse.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "window.c", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/libspeex/", "_kiss_fft_guts.h", "audio/decoder/audio_decoders/speex/libspeex"),
                ("LIB_", "speex/include/speex/", "speex.h", "audio/decoder/audio_decoders/speex/include/speex"),
                ("LIB_", "speex/include/speex/", "speex_bits.h", "audio/decoder/audio_decoders/speex/include/speex"),
                ("LIB_", "speex/include/speex/", "speex_callbacks.h", "audio/decoder/audio_decoders/speex/include/speex"),
                ("LIB_", "speex/include/speex/", "speex_config.h", "audio/decoder/audio_decoders/speex/include/speex"),
                ("LIB_", "speex/include/speex/", "speex_config_types.h", "audio/decoder/audio_decoders/speex/include/speex"),
                ("LIB_", "speex/include/speex/", "speex_header.h", "audio/decoder/audio_decoders/speex/include/speex"),
                ("LIB_", "speex/include/speex/", "speex_stereo.h", "audio/decoder/audio_decoders/speex/include/speex"),
                ("LIB_", "speex/include/speex/", "speex_types.h", "audio/decoder/audio_decoders/speex/include/speex")]

utilsTable   = [("LIB_", "utils/", "utils.c", "audio/decoder/audio_decoders/utils"),
                ("LIB_", "utils/", "utils.h", "audio/decoder/audio_decoders/utils")]

oggTable     = [("LIB_", "ogg/", "ogg.h", "audio/decoder/audio_decoders/ogg")]

ftlTable     = [("LIB_", "../templates/", "audio_decoders.h.ftl", "audio/decoder"),
                ("LIB_", "../templates/", "audio_decoders_config.h.ftl", "audio/decoder"),
                ("LIB_", "../templates/", "gfx_decoders.h.ftl", "audio/decoder")]


# Wav
def enableWavDecoderFiles(component, enable):

    for fileSymbol, srcPath, file, destPath in wavTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

  
def enableWavDecoder(symbol, event):
    enableWavDecoderFiles(symbol.getComponent(), event["value"]==True)

# ADPCM
def enableAdpcmDecoderFiles(component, enable):

    for fileSymbol, srcPath, file, destPath in adpcmTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

  
def enableAdpcmDecoder(symbol, event):
    enableAdpcmDecoderFiles(symbol.getComponent(), event["value"]==True)


# FLAC
def enableFlacDecoderFiles(component, enable):

    for fileSymbol, srcPath, file, destPath in flacTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

  
def enableFlacDecoder(symbol, event):
    enableFlacDecoderFiles(symbol.getComponent(), event["value"]==True)


# SPEEX
def enableSpeexDecoderFiles(component, enable):

    for fileSymbol, srcPath, file, destPath in speexTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

  
def enableSpeexDecoder(symbol, event):
    enableSpeexDecoderFiles(symbol.getComponent(), event["value"]==True)


# OPUS
def enableOpusDecoderFiles(component, enable):

    for fileSymbol, srcPath, file, destPath in opusTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

    for fileSymbol, srcPath, file, destPath in oggTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Generate file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()
        exec("component.getSymbolByID(\"" + symbol + "\").setEnabled(enable)")

  
def enableOpusDecoder(symbol, event):
    enableOpusDecoderFiles(symbol.getComponent(), event["value"]==True)



    
def instantiateComponent(audioDecoderComponent):
    global CONFIG_USE_WAV_STREAMING
    global CONFIG_USE_ADPCM_STREAMING

    CONFIG_USE_WAV_STREAMING = audioDecoderComponent.createBooleanSymbol("CONFIG_USE_WAV_STREAMING", None)
    CONFIG_USE_WAV_STREAMING.setVisible(True)
    CONFIG_USE_WAV_STREAMING.setLabel("Enable WAV Decoder")
    CONFIG_USE_WAV_STREAMING.setDefaultValue(True)
    CONFIG_USE_WAV_STREAMING.setDependencies(enableWavDecoder, ["CONFIG_USE_WAV_STREAMING"])

    CONFIG_USE_ADPCM_STREAMING = audioDecoderComponent.createBooleanSymbol("CONFIG_USE_ADPCM_STREAMING", None)
    CONFIG_USE_ADPCM_STREAMING.setVisible(True)
    CONFIG_USE_ADPCM_STREAMING.setLabel("Enable ADPCM Decoder")
    CONFIG_USE_ADPCM_STREAMING.setDefaultValue(True)
    CONFIG_USE_ADPCM_STREAMING.setDependencies(enableAdpcmDecoder, ["CONFIG_USE_ADPCM_STREAMING"])

    CONFIG_USE_FLAC_DECODER = audioDecoderComponent.createBooleanSymbol("CONFIG_USE_FLAC_DECODER", None)
    CONFIG_USE_FLAC_DECODER.setVisible(True)
    CONFIG_USE_FLAC_DECODER.setLabel("Enable FLAC Decoder")
    CONFIG_USE_FLAC_DECODER.setDefaultValue(False)
    CONFIG_USE_FLAC_DECODER.setDependencies(enableFlacDecoder, ["CONFIG_USE_FLAC_DECODER"])

    CONFIG_USE_OGG_SPEEX_DECODER = audioDecoderComponent.createBooleanSymbol("CONFIG_USE_OGG_SPEEX_DECODER", None)
    CONFIG_USE_OGG_SPEEX_DECODER.setVisible(True)
    CONFIG_USE_OGG_SPEEX_DECODER.setLabel("Enable SPEEX Decoder")
    CONFIG_USE_OGG_SPEEX_DECODER.setDefaultValue(False)
    CONFIG_USE_OGG_SPEEX_DECODER.setDependencies(enableSpeexDecoder, ["CONFIG_USE_OGG_SPEEX_DECODER"])
   
    CONFIG_USE_OGG_OPUS_DECODER = audioDecoderComponent.createBooleanSymbol("CONFIG_USE_OGG_OPUS_DECODER", None)
    CONFIG_USE_OGG_OPUS_DECODER.setVisible(True)
    CONFIG_USE_OGG_OPUS_DECODER.setLabel("Enable OPUS Decoder")
    CONFIG_USE_OGG_OPUS_DECODER.setDefaultValue(False)
    CONFIG_USE_OGG_OPUS_DECODER.setDependencies(enableOpusDecoder, ["CONFIG_USE_OGG_OPUS_DECODER"])
    
    CONFIG_USE_WMA_DECODER = audioDecoderComponent.createBooleanSymbol("CONFIG_USE_WMA_DECODER", None)
    CONFIG_USE_WMA_DECODER.setVisible(False)
    CONFIG_USE_WMA_DECODER.setLabel("Enable WMA Decoder")
    CONFIG_USE_WMA_DECODER.setDefaultValue(False)
    CONFIG_USE_WMA_DECODER.setDependencies(enableOpusDecoder, ["CONFIG_USE_WMA_DECODER"])
    
    CONFIG_USE_MP3_DECODER = audioDecoderComponent.createBooleanSymbol("CONFIG_USE_MP3_DECODER", None)
    CONFIG_USE_MP3_DECODER.setVisible(False)
    CONFIG_USE_MP3_DECODER.setLabel("Enable MP3 Decoder")
    CONFIG_USE_MP3_DECODER.setDefaultValue(False)
    CONFIG_USE_MP3_DECODER.setDependencies(enableOpusDecoder, ["CONFIG_USE_MP3_DECODER"])
    
    CONFIG_USE_AAC_DECODER = audioDecoderComponent.createBooleanSymbol("CONFIG_USE_AAC_DECODER", None)
    CONFIG_USE_AAC_DECODER.setVisible(False)
    CONFIG_USE_AAC_DECODER.setLabel("Enable AAC Decoder")
    CONFIG_USE_AAC_DECODER.setDefaultValue(False)
    CONFIG_USE_AAC_DECODER.setDependencies(enableOpusDecoder, ["CONFIG_USE_AAC_DECODER"])
    
    CONFIG_USE_JPEG_DECODER = audioDecoderComponent.createBooleanSymbol("CONFIG_USE_JPEG_DECODER", None)
    CONFIG_USE_JPEG_DECODER.setVisible(False)
    CONFIG_USE_JPEG_DECODER.setLabel("Enable JPEG Decoder")
    CONFIG_USE_JPEG_DECODER.setDefaultValue(False)
    CONFIG_USE_JPEG_DECODER.setDependencies(enableOpusDecoder, ["CONFIG_USE_JPEG_DECODER"])
    


    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")  # e.g. "default"

    Log.writeInfoMessage("Audio Decoders instantiated")

    for fileSymbol, srcPath, file, destPath in wavTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Create unique file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()

        exec(symbol + " = audioDecoderComponent.createFileSymbol(\"" + symbol + "\", None)")
        exec(symbol + ".setSourcePath(\"" + srcPath + file + "\")")
        exec(symbol + ".setOutputName(\"" + file + "\")")
        exec(symbol + ".setDestPath(\"" + destPath + "\")")
        exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/decoder/audio_decoders\")")
        exec(symbol + ".setType(\"" + type + "\")")
        exec(symbol + ".setEnabled(CONFIG_USE_WAV_STREAMING.getValue() == True)")

    for fileSymbol, srcPath, file, destPath in adpcmTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Create unique file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()

        exec(symbol + " = audioDecoderComponent.createFileSymbol(\"" + symbol + "\", None)")
        exec(symbol + ".setSourcePath(\"" + srcPath + file + "\")")
        exec(symbol + ".setOutputName(\"" + file + "\")")
        exec(symbol + ".setDestPath(\"" + destPath + "\")")
        exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/decoder/audio_decoders\")")
        exec(symbol + ".setType(\"" + type + "\")")
        exec(symbol + ".setEnabled(CONFIG_USE_ADPCM_STREAMING.getValue() == True)")

    for fileSymbol, srcPath, file, destPath in flacTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Create unique file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()

        exec(symbol + " = audioDecoderComponent.createFileSymbol(\"" + symbol + "\", None)")
        exec(symbol + ".setSourcePath(\"" + srcPath + file + "\")")
        exec(symbol + ".setOutputName(\"" + file + "\")")
        exec(symbol + ".setDestPath(\"" + destPath + "\")")
        exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/decoder/audio_decoders\")")
        exec(symbol + ".setType(\"" + type + "\")")
        exec(symbol + ".setEnabled(CONFIG_USE_FLAC_DECODER.getValue() == True)")

    for fileSymbol, srcPath, file, destPath in speexTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Create unique file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()

        exec(symbol + " = audioDecoderComponent.createFileSymbol(\"" + symbol + "\", None)")
        exec(symbol + ".setSourcePath(\"" + srcPath + file + "\")")
        exec(symbol + ".setOutputName(\"" + file + "\")")
        exec(symbol + ".setDestPath(\"" + destPath + "\")")
        exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/decoder/audio_decoders\")")
        exec(symbol + ".setType(\"" + type + "\")")
        exec(symbol + ".setEnabled(CONFIG_USE_OGG_SPEEX_DECODER.getValue() == True)")

    for fileSymbol, srcPath, file, destPath in opusTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Create unique file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()

        exec(symbol + " = audioDecoderComponent.createFileSymbol(\"" + symbol + "\", None)")
        exec(symbol + ".setSourcePath(\"" + srcPath + file + "\")")
        exec(symbol + ".setOutputName(\"" + file + "\")")
        exec(symbol + ".setDestPath(\"" + destPath + "\")")
        exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/decoder/audio_decoders\")")
        exec(symbol + ".setType(\"" + type + "\")")
        exec(symbol + ".setEnabled(CONFIG_USE_OGG_OPUS_DECODER.getValue() == True)")

    for fileSymbol, srcPath, file, destPath in oggTable:
        # Set type
        baseFileName = os.path.splitext(file)[0]
        ext = os.path.splitext(file)[-1].lower()
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Create unique file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").upper() + baseFileName.upper() + "_" + type.upper()

        exec(symbol + " = audioDecoderComponent.createFileSymbol(\"" + symbol + "\", None)")
        exec(symbol + ".setSourcePath(\"" + srcPath + file + "\")")
        exec(symbol + ".setOutputName(\"" + file + "\")")
        exec(symbol + ".setDestPath(\"" + destPath + "\")")
        exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/decoder/audio_decoders\")")
        exec(symbol + ".setType(\"" + type + "\")")
        exec(symbol + ".setEnabled(CONFIG_USE_OGG_OPUS_DECODER.getValue() == True)")

    for fileSymbol, srcPath, file, destPath in ftlTable:
        # Set type
        baseFileName1 = os.path.splitext(file)[0]       # Strip the .ftl extension
        baseFileName = os.path.splitext(baseFileName1)[0]
        ext = os.path.splitext(baseFileName1)[-1].lower()
        print("baseFileName1: " + baseFileName1 + ", baseFileName: " + baseFileName + ", ext: " + ext)
        if ext in src_ext:
            type = "SOURCE"
        elif ext in hdr_ext:
            type = "HEADER"
        else:
            type = "IMPORTANT"

        # Create unique file symbol
        symbol = fileSymbol + srcPath.replace("/", "_").replace(".", "").upper() + baseFileName.upper() + "_" + type.upper()

        exec(symbol + " = audioDecoderComponent.createFileSymbol(\"" + symbol + "\", None)")
        exec(symbol + ".setSourcePath(\"" + srcPath + file + "\")")
        exec(symbol + ".setOutputName(\"" + baseFileName1 + "\")")
        exec(symbol + ".setDestPath(\"" + destPath + "\")")
        exec(symbol + ".setProjectPath(\"config/" + configName + "/audio/decoder\")")
        exec(symbol + ".setType(\"" + type + "\")")
        exec(symbol + ".setEnabled(True)")
        exec(symbol + ".setMarkup(True)")

    if("PIC32" in Variables.get("__PROCESSOR")):
       CONFIG_USE_WMA_DECODER.setReadOnly(True)
       CONFIG_USE_AAC_DECODER.setReadOnly(True)
       CONFIG_USE_MP3_DECODER.setReadOnly(True)
       CONFIG_USE_WMA_DECODER.setVisible(False)
       CONFIG_USE_AAC_DECODER.setVisible(False)
       CONFIG_USE_MP3_DECODER.setVisible(False)
    # else:
       # CONFIG_USE_FLAC_DECODER.setVisible(False)
       # CONFIG_USE_OGG_SPEEX_DECODER.setVisible(False)
       # CONFIG_USE_OGG_OPUS_DECODER.setVisible(False)

# TTD
#   Check if WMA, AAC, and MP3 .a files exist, if so setReadOnly(False)
#    if(os.path.is_file("../premium/decoder_wma/):
   

    
