#include "cnnapi.h"


void LeNet5() {

    // input 32x32x1
    image_mc_t *input = RandomInitImage(32, 32, 8, 1);

    // layer1 c1
    //kernel size = 5*5  strides = 1  num = 6
    kernel_mc_t *c1_ker = RandomInitKernel(5, 8, 6);
    image_mc_t *c1 = Convolution(input, c1_ker, 1);
    //outsize 28*28*6

    // layer2 s2
    //pool size = 2*2  strides = 2
    image_mc_t *s2 = MaxPooling(c1, 2, 2);
    //outsize 14*14*6

    // layer3 c3
    //kernel size = 5*5  strides = 1  num = 16
    kernel_mc_t *c3_ker = RandomInitKernel(5, 8, 16);
    image_mc_t *c3 = Convolution(s2, c3_ker, 1);
    //outsize 10*10*16

    // layer4 s4
    //pool size = 2*2  strides = 2
    image_mc_t *s4 = MaxPooling(c3, 2, 2);
    //outsize 5*5*16

    // layer5 fc1
    image_t *fc1_pre = Flatten(image_mc_t *s4);
    //outsize 1*400
    fc_filter_t *fc_filter1 = RandomInitFcFilterArray(1, 400, 8, 120);
    image_t *fc1 = Dense(fc1_pre, fc_filter1, 120);
    //outsize 1*120

    // layer6 fc2
    fc_filter_t *fc_filter2 = RandomInitFcFilterArray(1, 120, 8, 84);
    image_t *fc2 = Dense(fc1, fc_filter2, 84);
    //outsize 1*84

    // ouput 1*10
    fc_filter_t *fc_filter_out = RandomInitFcFilterArray(1, 84, 8, 10);
    image_t *output = Dense(fc2, fc_filter_out, 10);
    SetOutput_SC(output);

}

