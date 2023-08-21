//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "DX10Fog\h_fluid_common.hlsl"

//////////////////////////////////////////////////////////////////////////////////////////
//	Geometry
[maxvertexcount (3)]
void main(triangle v2g_fluidsim In[3], inout TriangleStream<g2p_fluidsim> triStream)
{
    g2p_fluidsim Out;
    // cell0.z of the first vertex in the triangle determines the destination slice index
    Out.RTIndex = In[0].cell0.z;
    for(int v=0; v<3; v++)
    {
        Out.pos          = In[v].pos; 
        Out.cell0        = In[v].cell0;
        Out.texcoords    = In[v].texcoords;
        Out.LR           = In[v].LR;
        Out.BT           = In[v].BT;
        Out.DU           = In[v].DU;
        triStream.Append( Out );
    }
    triStream.RestartStrip( );
}