#pragma once

namespace ppmd
{

enum { TOP=1 << 24, BOT=1 << 15 };

struct 
SUBRANGE 
{ 
    DWORD low, high, scale; 
};

static SUBRANGE SubRange = {0,0,0};
static DWORD    low=0, code=0, range=0;


inline void 
rcEncNormalize( _PPMD_FILE* stream )
{
    while(      (low ^ (low+range)) < TOP 
            ||  range < BOT 
            &&  ((range= -low & (BOT-1)),1)
         ) 
    {
        _PPMD_E_PUTC( low >> 24, stream );
        range   <<= 8;                        
        low     <<= 8;
    }
}

static inline void rcInitEncoder() { low=0; range=DWORD(-1); }

static inline void rcEncodeSymbol()
{
    low += SubRange.low*(range/=SubRange.scale);  range *= SubRange.high-SubRange.low;
}

static inline void rcFlushEncoder(_PPMD_FILE* stream)
{
    for (UINT i=0;i < 4;i++) {
        _PPMD_E_PUTC(low >> 24,stream);     low <<= 8;
    }
}
static inline void rcInitDecoder(_PPMD_FILE* stream)
{
    low=code=0;                             range=DWORD(-1);
    for (UINT i=0;i < 4;i++)
            code=(code << 8) | _PPMD_D_GETC(stream);
}

inline void
rcDecNormalize( _PPMD_FILE* stream )
{
    while(      (low ^ (low+range)) < TOP 
            ||  range < BOT 
            &&  ((range= -low & (BOT-1)),1) 
         ) 
    {
        code    = (code << 8) | _PPMD_D_GETC(stream);
        range <<= 8;                        
        low   <<= 8;
    }
}

static inline UINT rcGetCurrentCount() { return (code-low)/(range /= SubRange.scale); }
static inline void rcRemoveSubrange()
{
    low     += range*SubRange.low;                 
    range   *= SubRange.high-SubRange.low;
}

static inline UINT rcBinStart(UINT f0,UINT Shift)  { return f0*(range >>= Shift); }
static inline UINT rcBinDecode  (UINT tmp)         { return (code-low >= tmp); }
static inline void rcBinCorrect0(UINT tmp)         { range=tmp; }
static inline void rcBinCorrect1(UINT tmp,UINT f1) { low += tmp;   range *= f1; }



} // namespace ppmd

