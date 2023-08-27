// xrXRC.h: interface for the xrXRC class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "xrCDB.h"

//#ifdef ENGINE_BUILD
//#ifdef DEBUG
//extern XRCDB_API CStatTimer* cdb_clRAY; // total: ray-testing
//extern XRCDB_API CStatTimer* cdb_clBOX; // total: box query
//extern XRCDB_API CStatTimer* cdb_clFRUSTUM; // total: frustum query
//#endif
//#endif

//#ifdef ENGINE_BUILD
//class XRCDB_API xrXRC
//#else
class XRCDB_API xrXRC
//#endif
{
    CDB::COLLIDER CL;

public:
    IC void ray_options(u32 f) { CL.ray_options(f); }
    IC void ray_query(const CDB::MODEL* m_def, const Fvector& r_start, const Fvector& r_dir, float r_range = 10000.f)
    {
//#ifdef ENGINE_BUILD
//#ifdef DEBUG
//        Device.Statistic->clRAY.Begin();
//#endif
//        CL.ray_query(m_def, r_start, r_dir, r_range);
//#ifdef DEBUG
//        Device.Statistic->clRAY.End();
//#endif
//#else
#ifdef DEBUG
        cdb_clRAY->Begin();
#endif
        CL.ray_query(m_def, r_start, r_dir, r_range);
#ifdef DEBUG
        cdb_clRAY->End();
#endif	
//#endif
    }

    IC void box_options(u32 f) { CL.box_options(f); }
    IC void box_query(const CDB::MODEL* m_def, const Fvector& b_center, const Fvector& b_dim)
    {
#ifdef DEBUG
        cdb_clBOX->Begin();
#endif
        CL.box_query(m_def, b_center, b_dim);
#ifdef DEBUG
        cdb_clBOX->End();
#endif
    }

    IC void frustum_options(u32 f) { CL.frustum_options(f); }
    IC void frustum_query(const CDB::MODEL* m_def, const CFrustum& F)
    {
//#ifdef ENGINE_BUILD
//#ifdef DEBUG
//        Device.Statistic->clFRUSTUM.Begin();
//#endif
//        CL.frustum_query(m_def, F);
//#ifdef DEBUG
//        Device.Statistic->clFRUSTUM.End();
//#endif
//#else
#ifdef DEBUG
        cdb_clFRUSTUM->Begin();
#endif
        CL.frustum_query(m_def, F);
#ifdef DEBUG
        cdb_clFRUSTUM->End();
#endif
//#endif
    }

    IC CDB::RESULT* r_begin() { return CL.r_begin(); };
    IC CDB::RESULT* r_end() { return CL.r_end(); };
    IC void r_free() { CL.r_free(); }
    IC int r_count() { return CL.r_count(); };
    IC void r_clear() { CL.r_clear(); };
    IC void r_clear_compact() { CL.r_clear_compact(); };

    xrXRC();
    ~xrXRC();
};

XRCDB_API extern xrXRC XRC;