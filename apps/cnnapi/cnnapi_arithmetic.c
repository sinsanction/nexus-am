#include "cnninst.h"
#include "cnnapi.h"


//arithmetic
image_t *Convolution(image_t *input_image, kernel_t *input_kernel, int strides);

image_t *MaxPooling(image_t *input_image, int pool_size, int strides);

image_t *AvgPooling(image_t *input_image, int pool_size, int strides);

image_t *Activation(image_t *input_image, char *algorithm, uint16_t zero_point);

image_t *Dense(image_t *input_image, fc_filter_t *fc_filter_array, int units);

