#pragma once

// Static geometry optimization
constexpr float O_S_L1_S_LOW = 10.f; // geometry 3d volume size
constexpr float O_S_L1_D_LOW = 150.f; // distance, after which it is not rendered
constexpr float O_S_L2_S_LOW = 100.f;
constexpr float O_S_L2_D_LOW = 200.f;
constexpr float O_S_L3_S_LOW = 500.f;
constexpr float O_S_L3_D_LOW = 250.f;
constexpr float O_S_L4_S_LOW = 2500.f;
constexpr float O_S_L4_D_LOW = 350.f;
constexpr float O_S_L5_S_LOW = 7000.f;
constexpr float O_S_L5_D_LOW = 400.f;

constexpr float O_S_L1_S_MED = 25.f;
constexpr float O_S_L1_D_MED = 50.f;
constexpr float O_S_L2_S_MED = 200.f;
constexpr float O_S_L2_D_MED = 150.f;
constexpr float O_S_L3_S_MED = 1000.f;
constexpr float O_S_L3_D_MED = 200.f;
constexpr float O_S_L4_S_MED = 2500.f;
constexpr float O_S_L4_D_MED = 300.f;
constexpr float O_S_L5_S_MED = 7000.f;
constexpr float O_S_L5_D_MED = 400.f;

constexpr float O_S_L1_S_HII = 50.f;
constexpr float O_S_L1_D_HII = 50.f;
constexpr float O_S_L2_S_HII = 400.f;
constexpr float O_S_L2_D_HII = 150.f;
constexpr float O_S_L3_S_HII = 1500.f;
constexpr float O_S_L3_D_HII = 200.f;
constexpr float O_S_L4_S_HII = 5000.f;
constexpr float O_S_L4_D_HII = 300.f;
constexpr float O_S_L5_S_HII = 20000.f;
constexpr float O_S_L5_D_HII = 350.f;

constexpr float O_S_L1_S_ULT = 50.f;
constexpr float O_S_L1_D_ULT = 35.f;
constexpr float O_S_L2_S_ULT = 500.f;
constexpr float O_S_L2_D_ULT = 125.f;
constexpr float O_S_L3_S_ULT = 1750.f;
constexpr float O_S_L3_D_ULT = 175.f;
constexpr float O_S_L4_S_ULT = 5250.f;
constexpr float O_S_L4_D_ULT = 250.f;
constexpr float O_S_L5_S_ULT = 25000.f;
constexpr float O_S_L5_D_ULT = 300.f;

// Dyn geometry optimization
constexpr float O_D_L1_S_LOW = 1.f; // geometry 3d volume size
constexpr float O_D_L1_D_LOW = 80.f; // distance, after which it is not rendered
constexpr float O_D_L2_S_LOW = 3.f;
constexpr float O_D_L2_D_LOW = 150.f;
constexpr float O_D_L3_S_LOW = 4000.f;
constexpr float O_D_L3_D_LOW = 250.f;

constexpr float O_D_L1_S_MED = 1.f;
constexpr float O_D_L1_D_MED = 40.f;
constexpr float O_D_L2_S_MED = 4.f;
constexpr float O_D_L2_D_MED = 100.f;
constexpr float O_D_L3_S_MED = 4000.f;
constexpr float O_D_L3_D_MED = 200.f;

constexpr float O_D_L1_S_HII = 1.4f;
constexpr float O_D_L1_D_HII = 30.f;
constexpr float O_D_L2_S_HII = 4.f;
constexpr float O_D_L2_D_HII = 80.f;
constexpr float O_D_L3_S_HII = 4000.f;
constexpr float O_D_L3_D_HII = 150.f;

constexpr float O_D_L1_S_ULT = 2.0f;
constexpr float O_D_L1_D_ULT = 30.f;
constexpr float O_D_L2_S_ULT = 8.f;
constexpr float O_D_L2_D_ULT = 50.f;
constexpr float O_D_L3_S_ULT = 4000.f;
constexpr float O_D_L3_D_ULT = 110.f;

constexpr Fvector4 o_optimize_static_l1_dist = {O_S_L1_D_LOW, O_S_L1_D_MED, O_S_L1_D_HII, O_S_L1_D_ULT};
constexpr Fvector4 o_optimize_static_l1_size = {O_S_L1_S_LOW, O_S_L1_S_MED, O_S_L1_S_HII, O_S_L1_S_ULT};
constexpr Fvector4 o_optimize_static_l2_dist = {O_S_L2_D_LOW, O_S_L2_D_MED, O_S_L2_D_HII, O_S_L2_D_ULT};
constexpr Fvector4 o_optimize_static_l2_size = {O_S_L2_S_LOW, O_S_L2_S_MED, O_S_L2_S_HII, O_S_L2_S_ULT};
constexpr Fvector4 o_optimize_static_l3_dist = {O_S_L3_D_LOW, O_S_L3_D_MED, O_S_L3_D_HII, O_S_L3_D_ULT};
constexpr Fvector4 o_optimize_static_l3_size = {O_S_L3_S_LOW, O_S_L3_S_MED, O_S_L3_S_HII, O_S_L3_S_ULT};
constexpr Fvector4 o_optimize_static_l4_dist = {O_S_L4_D_LOW, O_S_L4_D_MED, O_S_L4_D_HII, O_S_L4_D_ULT};
constexpr Fvector4 o_optimize_static_l4_size = {O_S_L4_S_LOW, O_S_L4_S_MED, O_S_L4_S_HII, O_S_L4_S_ULT};
constexpr Fvector4 o_optimize_static_l5_dist = {O_S_L5_D_LOW, O_S_L5_D_MED, O_S_L5_D_HII, O_S_L5_D_ULT};
constexpr Fvector4 o_optimize_static_l5_size = {O_S_L5_S_LOW, O_S_L5_S_MED, O_S_L5_S_HII, O_S_L5_S_ULT};

constexpr Fvector4 o_optimize_dynamic_l1_dist = {O_D_L1_D_LOW, O_D_L1_D_MED, O_D_L1_D_HII, O_D_L1_D_ULT};
constexpr Fvector4 o_optimize_dynamic_l1_size = {O_D_L1_S_LOW, O_D_L1_S_MED, O_D_L1_S_HII, O_D_L1_S_ULT};
constexpr Fvector4 o_optimize_dynamic_l2_dist = {O_D_L2_D_LOW, O_D_L2_D_MED, O_D_L2_D_HII, O_D_L2_D_ULT};
constexpr Fvector4 o_optimize_dynamic_l2_size = {O_D_L2_S_LOW, O_D_L2_S_MED, O_D_L2_S_HII, O_D_L2_S_ULT};
constexpr Fvector4 o_optimize_dynamic_l3_dist = {O_D_L3_D_LOW, O_D_L3_D_MED, O_D_L3_D_HII, O_D_L3_D_ULT};
constexpr Fvector4 o_optimize_dynamic_l3_size = {O_D_L3_S_LOW, O_D_L3_S_MED, O_D_L3_S_HII, O_D_L3_S_ULT};

extern IC float GetDistFromCamera(const Fvector& from_position);
extern IC bool IsValuableToRender(dxRender_Visual* pVisual, bool isStatic, bool sm, Fmatrix& transform_matrix);
