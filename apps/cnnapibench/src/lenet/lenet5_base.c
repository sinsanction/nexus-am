#include "benchmark.h"
#include "cnnapi.h"
#include "cnnapi_common.h"
#include "cnnapi_stdins.h"

static image_mc_t *input;
static kernel_mc_t *c1_ker;
static kernel_mc_t *c3_ker;
static fc_filter_t *fc_filter1;
static fc_filter_t *fc_filter2;
static fc_filter_t *fc_filter_out;
static image_t *output;

static int test_pass;

void bench_lenet5_base_prepare() {
  bench_srand(1);
  test_pass = 1;

  // input 32x32x1
  input = RandomInitImage(32, 32, 8, 1);
  //kernel size = 5*5  strides = 1  num = 6
  c1_ker = RandomInitKernel(5, 8, 1, 6);
  //kernel size = 5*5  strides = 1  num = 16
  c3_ker = RandomInitKernel(5, 8, 6, 16);

  fc_filter1 = RandomInitFcFilterArray(1, 400, 8, 120);
  fc_filter2 = RandomInitFcFilterArray(1, 120, 8, 84);
  fc_filter_out = RandomInitFcFilterArray(1, 84, 8, 10);
}

void bench_lenet5_base_run() {

    // layer1 c1
    image_mc_t *c1 = StdIns_Convolution(input, c1_ker, 1);
    //outsize 28*28*6

    // layer2 s2
    //pool size = 2*2  strides = 2
    image_mc_t *s2 = StdIns_MaxPooling(c1, 2, 2);
    //outsize 14*14*6

    // layer3 c3
    image_mc_t *c3 = StdIns_Convolution(s2, c3_ker, 1);
    //outsize 10*10*16

    // layer4 s4
    //pool size = 2*2  strides = 2
    image_mc_t *s4 = StdIns_MaxPooling(c3, 2, 2);
    //outsize 5*5*16

    // layer5 fc1
    image_t *fc1_pre = Flatten(s4);
    //outsize 1*400
    image_t *fc1 = Dense(fc1_pre, fc_filter1, 120);
    //outsize 1*120

    // layer6 fc2
    image_t *fc2 = Dense(fc1, fc_filter2, 84);
    //outsize 1*84

    // ouput 1*10
    output = Dense(fc2, fc_filter_out, 10);
    SetOutput_SC(output);
}

int bench_lenet5_base_validate() {
  if (test_pass == 1) {
    printf("end: pass!!!\n");
  }
  else {
    printf("end: fail\n");
    SetOutput(input);
  }
  bench_free(input);
  bench_free(output);
  return (setting->checksum == 0x00030002) && test_pass;
}
