<#if CONFIG_USE_LIBQ_C_LIBRARY == true>
#include "audio/math/libq_c/libq_c.h"
</#if>

<#if CONFIG_USE_LIBQ_NFO3_LIBRARY == true || CONFIG_USE_LIBQ_NFOS_LIBRARY == true>
#include "audio/math/libq/libq.h"
</#if>

<#if CONFIG_USE_DSP_NFO3_LIBRARY == true || CONFIG_USE_DSP_NFOS_LIBRARY == true>
#include "audio/math/dsp/dsp.h"
</#if>
