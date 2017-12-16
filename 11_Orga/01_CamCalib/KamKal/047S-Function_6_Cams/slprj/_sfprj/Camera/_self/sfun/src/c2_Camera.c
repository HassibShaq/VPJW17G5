/* Include files */

#include "blascompat32.h"
#include "Camera_sfun.h"
#include "c2_Camera.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance.chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance.instanceNumber)
#include "Camera_sfun_debug_macros.h"

/* Type Definitions */

/* Named Constants */
#define c2_IN_NO_ACTIVE_CHILD          (0)

/* Variable Declarations */

/* Variable Definitions */
static SFc2_CameraInstanceStruct chartInstance;

/* Function Declarations */
static void initialize_c2_Camera(void);
static void initialize_params_c2_Camera(void);
static void enable_c2_Camera(void);
static void disable_c2_Camera(void);
static void c2_update_debugger_state_c2_Camera(void);
static const mxArray *get_sim_state_c2_Camera(void);
static void set_sim_state_c2_Camera(const mxArray *c2_st);
static void finalize_c2_Camera(void);
static void sf_c2_Camera(void);
static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber);
static const mxArray *c2_sf_marshall(void *c2_chartInstance, void *c2_u);
static const mxArray *c2_b_sf_marshall(void *c2_chartInstance, void *c2_u);
static const mxArray *c2_c_sf_marshall(void *c2_chartInstance, void *c2_u);
static const mxArray *c2_emlrt_marshallOut(uint8_T c2_u);
static void c2_emlrt_marshallIn(const mxArray *c2_out, char *c2_name, real_T
  c2_y[10]);
static uint8_T c2_b_emlrt_marshallIn(const mxArray *c2_b_is_active_c2_Camera,
  char *c2_name);
static void init_io_bus_offset(void);
static void init_dsm_address_info(void);

/* Function Definitions */
static void initialize_c2_Camera(void)
{
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
  chartInstance.c2_is_active_c2_Camera = 0U;
}

static void initialize_params_c2_Camera(void)
{
}

static void enable_c2_Camera(void)
{
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
}

static void disable_c2_Camera(void)
{
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
}

static void c2_update_debugger_state_c2_Camera(void)
{
}

static const mxArray *get_sim_state_c2_Camera(void)
{
  const mxArray *c2_st = NULL;
  const mxArray *c2_y = NULL;
  int32_T c2_i0;
  real_T c2_u[10];
  const mxArray *c2_b_y = NULL;
  real_T (*c2_out)[10];
  c2_out = (real_T (*)[10])ssGetOutputPortSignal(chartInstance.S, 1);
  c2_st = NULL;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_createcellarray(2));
  for (c2_i0 = 0; c2_i0 < 10; c2_i0 = c2_i0 + 1) {
    c2_u[c2_i0] = (*c2_out)[c2_i0];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_u, 0, 0U, 1U, 0U, 2, 1, 10));
  sf_mex_setcell(c2_y, 0, c2_b_y);
  sf_mex_setcell(c2_y, 1, c2_emlrt_marshallOut
                 (chartInstance.c2_is_active_c2_Camera));
  sf_mex_assign(&c2_st, c2_y);
  return c2_st;
}

static void set_sim_state_c2_Camera(const mxArray *c2_st)
{
  const mxArray *c2_u;
  real_T c2_dv0[10];
  int32_T c2_i1;
  real_T (*c2_out)[10];
  c2_out = (real_T (*)[10])ssGetOutputPortSignal(chartInstance.S, 1);
  chartInstance.c2_doneDoubleBufferReInit = true;
  c2_u = sf_mex_dup(c2_st);
  c2_emlrt_marshallIn(sf_mex_dup(sf_mex_getcell(c2_u, 0)), "out", c2_dv0);
  for (c2_i1 = 0; c2_i1 < 10; c2_i1 = c2_i1 + 1) {
    (*c2_out)[c2_i1] = c2_dv0[c2_i1];
  }

  chartInstance.c2_is_active_c2_Camera = c2_b_emlrt_marshallIn(sf_mex_dup
    (sf_mex_getcell(c2_u, 1)), "is_active_c2_Camera");
  sf_mex_destroy(&c2_u);
  c2_update_debugger_state_c2_Camera();
  sf_mex_destroy(&c2_st);
}

static void finalize_c2_Camera(void)
{
}

static void sf_c2_Camera(void)
{
  int32_T c2_i2;
  int32_T c2_i3;
  uint8_T c2_previousEvent;
  int32_T c2_i4;
  real_T c2_in[10];
  real_T c2_nargout = 1.0;
  real_T c2_nargin = 1.0;
  real_T c2_size_in[2];
  real_T c2_out[10];
  int32_T c2_i5;
  int32_T c2_i6;
  static real_T c2_dv1[2] = { 1.0, 10.0 };

  int32_T c2_i7;
  int32_T c2_i8;
  real_T (*c2_b_out)[10];
  real_T (*c2_b_in)[10];
  c2_b_out = (real_T (*)[10])ssGetOutputPortSignal(chartInstance.S, 1);
  c2_b_in = (real_T (*)[10])ssGetInputPortSignal(chartInstance.S, 0);
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG,0);
  for (c2_i2 = 0; c2_i2 < 10; c2_i2 = c2_i2 + 1) {
    _SFD_DATA_RANGE_CHECK((*c2_b_in)[c2_i2], 0U);
  }

  for (c2_i3 = 0; c2_i3 < 10; c2_i3 = c2_i3 + 1) {
    _SFD_DATA_RANGE_CHECK((*c2_b_out)[c2_i3], 1U);
  }

  c2_previousEvent = _sfEvent_;
  _sfEvent_ = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG,0);
  for (c2_i4 = 0; c2_i4 < 10; c2_i4 = c2_i4 + 1) {
    c2_in[c2_i4] = (*c2_b_in)[c2_i4];
  }

  sf_debug_symbol_scope_push(5U, 0U);
  sf_debug_symbol_scope_add("nargout", &c2_nargout, c2_c_sf_marshall);
  sf_debug_symbol_scope_add("nargin", &c2_nargin, c2_c_sf_marshall);
  sf_debug_symbol_scope_add("size_in", &c2_size_in, c2_b_sf_marshall);
  sf_debug_symbol_scope_add("out", &c2_out, c2_sf_marshall);
  sf_debug_symbol_scope_add("in", &c2_in, c2_sf_marshall);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0,3);
  for (c2_i5 = 0; c2_i5 < 10; c2_i5 = c2_i5 + 1) {
    c2_out[c2_i5] = 0.0;
  }

  _SFD_EML_CALL(0,4);
  for (c2_i6 = 0; c2_i6 < 2; c2_i6 = c2_i6 + 1) {
    c2_size_in[c2_i6] = c2_dv1[c2_i6];
  }

  _SFD_EML_CALL(0,5);
  CV_EML_IF(0, 0, c2_size_in[1] < 10.0);
  _SFD_EML_CALL(0,15);
  CV_EML_IF(0, 2, c2_size_in[1] > 10.0);
  _SFD_EML_CALL(0,18);
  for (c2_i7 = 0; c2_i7 < 10; c2_i7 = c2_i7 + 1) {
    c2_out[c2_i7] = c2_in[c2_i7];
  }

  _SFD_EML_CALL(0,-18);
  sf_debug_symbol_scope_pop();
  for (c2_i8 = 0; c2_i8 < 10; c2_i8 = c2_i8 + 1) {
    (*c2_b_out)[c2_i8] = c2_out[c2_i8];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG,0);
  _sfEvent_ = c2_previousEvent;
  sf_debug_check_for_state_inconsistency(_CameraMachineNumber_,
    chartInstance.chartNumber, chartInstance.instanceNumber);
}

static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber)
{
}

static const mxArray *c2_sf_marshall(void *c2_chartInstance, void *c2_u)
{
  const mxArray *c2_y = NULL;
  int32_T c2_i9;
  real_T c2_b_u[10];
  const mxArray *c2_b_y = NULL;
  c2_y = NULL;
  for (c2_i9 = 0; c2_i9 < 10; c2_i9 = c2_i9 + 1) {
    c2_b_u[c2_i9] = (*((real_T (*)[10])c2_u))[c2_i9];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_b_u, 0, 0U, 1U, 0U, 2, 1, 10));
  sf_mex_assign(&c2_y, c2_b_y);
  return c2_y;
}

static const mxArray *c2_b_sf_marshall(void *c2_chartInstance, void *c2_u)
{
  const mxArray *c2_y = NULL;
  int32_T c2_i10;
  real_T c2_b_u[2];
  const mxArray *c2_b_y = NULL;
  c2_y = NULL;
  for (c2_i10 = 0; c2_i10 < 2; c2_i10 = c2_i10 + 1) {
    c2_b_u[c2_i10] = (*((real_T (*)[2])c2_u))[c2_i10];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_b_u, 0, 0U, 1U, 0U, 2, 1, 2));
  sf_mex_assign(&c2_y, c2_b_y);
  return c2_y;
}

static const mxArray *c2_c_sf_marshall(void *c2_chartInstance, void *c2_u)
{
  const mxArray *c2_y = NULL;
  real_T c2_b_u;
  const mxArray *c2_b_y = NULL;
  c2_y = NULL;
  c2_b_u = *((real_T *)c2_u);
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_b_u, 0, 0U, 0U, 0U, 0));
  sf_mex_assign(&c2_y, c2_b_y);
  return c2_y;
}

const mxArray *sf_c2_Camera_get_eml_resolved_functions_info(void)
{
  const mxArray *c2_nameCaptureInfo = NULL;
  c2_ResolvedFunctionInfo c2_info[4];
  c2_ResolvedFunctionInfo (*c2_b_info)[4];
  const mxArray *c2_m0 = NULL;
  int32_T c2_i11;
  c2_ResolvedFunctionInfo *c2_r0;
  c2_nameCaptureInfo = NULL;
  c2_b_info = (c2_ResolvedFunctionInfo (*)[4])c2_info;
  (*c2_b_info)[0].context = "";
  (*c2_b_info)[0].name = "zeros";
  (*c2_b_info)[0].dominantType = "double";
  (*c2_b_info)[0].resolved = "[B]zeros";
  (*c2_b_info)[0].fileLength = 0U;
  (*c2_b_info)[0].fileTime1 = 0U;
  (*c2_b_info)[0].fileTime2 = 0U;
  (*c2_b_info)[1].context = "";
  (*c2_b_info)[1].name = "size";
  (*c2_b_info)[1].dominantType = "double";
  (*c2_b_info)[1].resolved = "[B]size";
  (*c2_b_info)[1].fileLength = 0U;
  (*c2_b_info)[1].fileTime1 = 0U;
  (*c2_b_info)[1].fileTime2 = 0U;
  (*c2_b_info)[2].context = "";
  (*c2_b_info)[2].name = "lt";
  (*c2_b_info)[2].dominantType = "double";
  (*c2_b_info)[2].resolved = "[B]lt";
  (*c2_b_info)[2].fileLength = 0U;
  (*c2_b_info)[2].fileTime1 = 0U;
  (*c2_b_info)[2].fileTime2 = 0U;
  (*c2_b_info)[3].context = "";
  (*c2_b_info)[3].name = "gt";
  (*c2_b_info)[3].dominantType = "double";
  (*c2_b_info)[3].resolved = "[B]gt";
  (*c2_b_info)[3].fileLength = 0U;
  (*c2_b_info)[3].fileTime1 = 0U;
  (*c2_b_info)[3].fileTime2 = 0U;
  sf_mex_assign(&c2_m0, sf_mex_createstruct("nameCaptureInfo", 1, 4));
  for (c2_i11 = 0; c2_i11 < 4; c2_i11 = c2_i11 + 1) {
    c2_r0 = &c2_info[c2_i11];
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->context, 15,
      0U, 0U, 0U, 2, 1, strlen(c2_r0->context)), "context",
                    "nameCaptureInfo", c2_i11);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->name, 15, 0U,
      0U, 0U, 2, 1, strlen(c2_r0->name)), "name",
                    "nameCaptureInfo", c2_i11);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->dominantType,
      15, 0U, 0U, 0U, 2, 1, strlen(c2_r0->dominantType)),
                    "dominantType", "nameCaptureInfo", c2_i11);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", c2_r0->resolved, 15,
      0U, 0U, 0U, 2, 1, strlen(c2_r0->resolved)), "resolved"
                    , "nameCaptureInfo", c2_i11);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->fileLength,
      7, 0U, 0U, 0U, 0), "fileLength", "nameCaptureInfo",
                    c2_i11);
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->fileTime1, 7,
      0U, 0U, 0U, 0), "fileTime1", "nameCaptureInfo", c2_i11
                    );
    sf_mex_addfield(c2_m0, sf_mex_create("nameCaptureInfo", &c2_r0->fileTime2, 7,
      0U, 0U, 0U, 0), "fileTime2", "nameCaptureInfo", c2_i11
                    );
  }

  sf_mex_assign(&c2_nameCaptureInfo, c2_m0);
  return c2_nameCaptureInfo;
}

static const mxArray *c2_emlrt_marshallOut(uint8_T c2_u)
{
  const mxArray *c2_y = NULL;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_create("y", &c2_u, 3, 0U, 0U, 0U, 0));
  return c2_y;
}

static void c2_emlrt_marshallIn(const mxArray *c2_out, char *c2_name, real_T
  c2_y[10])
{
  real_T c2_dv2[10];
  int32_T c2_i12;
  sf_mex_import(c2_name, sf_mex_dup(c2_out), &c2_dv2, 1, 0, 0U, 1, 0U, 2, 1, 10);
  for (c2_i12 = 0; c2_i12 < 10; c2_i12 = c2_i12 + 1) {
    c2_y[c2_i12] = c2_dv2[c2_i12];
  }

  sf_mex_destroy(&c2_out);
}

static uint8_T c2_b_emlrt_marshallIn(const mxArray *c2_b_is_active_c2_Camera,
  char *c2_name)
{
  uint8_T c2_y;
  uint8_T c2_u0;
  sf_mex_import(c2_name, sf_mex_dup(c2_b_is_active_c2_Camera), &c2_u0, 1, 3, 0U,
                0, 0U, 0);
  c2_y = c2_u0;
  sf_mex_destroy(&c2_b_is_active_c2_Camera);
  return c2_y;
}

static void init_io_bus_offset(void)
{
}

static void init_dsm_address_info(void)
{
}

/* SFunction Glue Code */
void sf_c2_Camera_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2447811900U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1496550047U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(4101333169U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2131595094U);
}

mxArray *sf_c2_Camera_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,4,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateDoubleMatrix(4,1,mxREAL);
    double *pr = mxGetPr(mxChecksum);
    pr[0] = (double)(3623041402U);
    pr[1] = (double)(2702783511U);
    pr[2] = (double)(584951785U);
    pr[3] = (double)(276736032U);
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,1,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(10);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,1,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(10);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  return(mxAutoinheritanceInfo);
}

static mxArray *sf_get_sim_state_info_c2_Camera(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"out\",},{M[8],M[0],T\"is_active_c2_Camera\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c2_Camera_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart(_CameraMachineNumber_,
          2,
          1,
          1,
          2,
          0,
          0,
          0,
          0,
          0,
          &(chartInstance.chartNumber),
          &(chartInstance.instanceNumber),
          ssGetPath(S),
          (void *)S);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          init_script_number_translation(_CameraMachineNumber_,
            chartInstance.chartNumber);
          sf_debug_set_chart_disable_implicit_casting(_CameraMachineNumber_,
            chartInstance.chartNumber,1);
          sf_debug_set_chart_event_thresholds(_CameraMachineNumber_,
            chartInstance.chartNumber,
            0,
            0,
            0);

          {
            unsigned int dimVector[2];
            dimVector[0]= 1;
            dimVector[1]= 10;
            _SFD_SET_DATA_PROPS(0,1,1,0,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
                                1.0,0,"in",0,c2_sf_marshall);
          }

          {
            unsigned int dimVector[2];
            dimVector[0]= 1;
            dimVector[1]= 10;
            _SFD_SET_DATA_PROPS(1,2,0,1,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
                                1.0,0,"out",0,NULL);
          }

          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of EML Model Coverage */
        _SFD_CV_INIT_EML(0,1,3,0,2,0,1,1);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,331);
        _SFD_CV_INIT_EML_IF(0,0,59,77,270,331);
        _SFD_CV_INIT_EML_IF(0,1,169,184,-1,-2);
        _SFD_CV_INIT_EML_IF(0,2,270,292,311,331);
        _SFD_CV_INIT_EML_FOR(0,0,110,136,269);
        _SFD_CV_INIT_EML_FOR(0,1,144,157,261);

        {
          static int condStart[] = { 173 };

          static int condEnd[] = { 184 };

          static int pfixExpr[] = { 0, -1 };

          _SFD_CV_INIT_EML_MCDC(0,0,172,184,1,0,&(condStart[0]),&(condEnd[0]),2,
                                &(pfixExpr[0]));
        }

        _SFD_TRANS_COV_WTS(0,0,0,1,0);
        if (chartAlreadyPresent==0) {
          _SFD_TRANS_COV_MAPS(0,
                              0,NULL,NULL,
                              0,NULL,NULL,
                              1,NULL,NULL,
                              0,NULL,NULL);
        }

        {
          real_T (*c2_in)[10];
          real_T (*c2_out)[10];
          c2_out = (real_T (*)[10])ssGetOutputPortSignal(chartInstance.S, 1);
          c2_in = (real_T (*)[10])ssGetInputPortSignal(chartInstance.S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, c2_in);
          _SFD_SET_DATA_VALUE_PTR(1U, c2_out);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(_CameraMachineNumber_,
        chartInstance.chartNumber,chartInstance.instanceNumber);
    }
  }
}

static void sf_opaque_initialize_c2_Camera(void *chartInstanceVar)
{
  chart_debug_initialization(chartInstance.S,0);
  initialize_params_c2_Camera();
  initialize_c2_Camera();
}

static void sf_opaque_enable_c2_Camera(void *chartInstanceVar)
{
  enable_c2_Camera();
}

static void sf_opaque_disable_c2_Camera(void *chartInstanceVar)
{
  disable_c2_Camera();
}

static void sf_opaque_gateway_c2_Camera(void *chartInstanceVar)
{
  sf_c2_Camera();
}

static mxArray* sf_opaque_get_sim_state_c2_Camera(void *chartInstanceVar)
{
  mxArray *st = (mxArray *) get_sim_state_c2_Camera();
  return st;
}

static void sf_opaque_set_sim_state_c2_Camera(void *chartInstanceVar, const
  mxArray *st)
{
  set_sim_state_c2_Camera(sf_mex_dup(st));
}

static void sf_opaque_terminate_c2_Camera(void *chartInstanceVar)
{
  if (sim_mode_is_rtw_gen(chartInstance.S) || sim_mode_is_external
      (chartInstance.S)) {
    sf_clear_rtw_identifier(chartInstance.S);
  }

  finalize_c2_Camera();
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c2_Camera(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c2_Camera();
  }
}

static void mdlSetWorkWidths_c2_Camera(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable("Camera","Camera",2);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop("Camera","Camera",2,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop("Camera","Camera",2,
      "gatewayCannotBeInlinedMultipleTimes"));
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,"Camera","Camera",2,1);
      sf_mark_chart_reusable_outputs(S,"Camera","Camera",2,1);
    }

    sf_set_rtw_dwork_info(S,"Camera","Camera",2);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
    ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  }

  ssSetChecksum0(S,(3853529867U));
  ssSetChecksum1(S,(3604242103U));
  ssSetChecksum2(S,(975317478U));
  ssSetChecksum3(S,(535559828U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
}

static void mdlRTW_c2_Camera(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    sf_write_symbol_mapping(S, "Camera", "Camera",2);
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c2_Camera(SimStruct *S)
{
  chartInstance.chartInfo.chartInstance = NULL;
  chartInstance.chartInfo.isEMLChart = 1;
  chartInstance.chartInfo.chartInitialized = 0;
  chartInstance.chartInfo.sFunctionGateway = sf_opaque_gateway_c2_Camera;
  chartInstance.chartInfo.initializeChart = sf_opaque_initialize_c2_Camera;
  chartInstance.chartInfo.terminateChart = sf_opaque_terminate_c2_Camera;
  chartInstance.chartInfo.enableChart = sf_opaque_enable_c2_Camera;
  chartInstance.chartInfo.disableChart = sf_opaque_disable_c2_Camera;
  chartInstance.chartInfo.getSimState = sf_opaque_get_sim_state_c2_Camera;
  chartInstance.chartInfo.setSimState = sf_opaque_set_sim_state_c2_Camera;
  chartInstance.chartInfo.getSimStateInfo = sf_get_sim_state_info_c2_Camera;
  chartInstance.chartInfo.zeroCrossings = NULL;
  chartInstance.chartInfo.outputs = NULL;
  chartInstance.chartInfo.derivatives = NULL;
  chartInstance.chartInfo.mdlRTW = mdlRTW_c2_Camera;
  chartInstance.chartInfo.mdlStart = mdlStart_c2_Camera;
  chartInstance.chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c2_Camera;
  chartInstance.chartInfo.extModeExec = NULL;
  chartInstance.chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance.chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance.chartInfo.storeCurrentConfiguration = NULL;
  chartInstance.S = S;
  ssSetUserData(S,(void *)(&(chartInstance.chartInfo)));/* register the chart instance with simstruct */
  if (!sim_mode_is_rtw_gen(S)) {
    init_dsm_address_info();
  }

  chart_debug_initialization(S,1);
}

void c2_Camera_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c2_Camera(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c2_Camera(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c2_Camera(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c2_Camera_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
