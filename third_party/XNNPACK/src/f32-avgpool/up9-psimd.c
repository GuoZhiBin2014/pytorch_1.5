// Copyright 2019 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <psimd.h>

#include <xnnpack/avgpool.h>


void xnn_f32_avgpool_ukernel_up9__psimd(
    size_t n,
    size_t ks,
    size_t kc,
    const float** input,
    const float* zero,
    float* output,
    size_t input_increment,
    size_t output_increment,
    const union xnn_f32_avgpool_params params[restrict static 1])
{
  assert(n != 0);
  assert(ks != 0);
  assert(ks <= 9);
  assert(kc != 0);

  const psimd_f32 vmultiplier = psimd_load_splat_f32(&params->scalar.multiplier);
  const psimd_f32 voutput_min = psimd_load_splat_f32(&params->scalar.output_min);
  const psimd_f32 voutput_max = psimd_load_splat_f32(&params->scalar.output_max);

  do {
    const float* i0 = input[0];
    const float* i1 = input[1];
    const float* i2 = input[2];
    const float* i3 = input[3];
    const float* i4 = input[4];
    const float* i5 = input[5];
    const float* i6 = input[6];
    const float* i7 = input[7];
    const float* i8 = input[8];
    input = (const float**) ((uintptr_t) input + input_increment);
    if (ks < 2) {
      i1 = zero;
    }
    if (ks <= 2) {
      i2 = zero;
    }
    if (ks < 4) {
      i3 = zero;
    }
    if (ks <= 4) {
      i4 = zero;
    }
    if (ks < 6) {
      i5 = zero;
    }
    if (ks <= 6) {
      i6 = zero;
    }
    if (ks < 8) {
      i7 = zero;
    }
    if (ks <= 8) {
      i8 = zero;
    }

    size_t k = kc;
    while (k >= 4) {
      const psimd_f32 vi0 = psimd_load_f32(i0);
      i0 += 4;
      const psimd_f32 vi1 = psimd_load_f32(i1);
      i1 += 4;
      const psimd_f32 vi2 = psimd_load_f32(i2);
      i2 += 4;
      const psimd_f32 vi3 = psimd_load_f32(i3);
      i3 += 4;
      const psimd_f32 vi4 = psimd_load_f32(i4);
      i4 += 4;
      const psimd_f32 vi5 = psimd_load_f32(i5);
      i5 += 4;
      const psimd_f32 vi6 = psimd_load_f32(i6);
      i6 += 4;
      const psimd_f32 vi7 = psimd_load_f32(i7);
      i7 += 4;
      const psimd_f32 vi8 = psimd_load_f32(i8);
      i8 += 4;

      const psimd_f32 vsum018 = psimd_add_f32(psimd_add_f32(vi0, vi1), vi8);
      const psimd_f32 vsum23 = psimd_add_f32(vi2, vi3);
      const psimd_f32 vsum45 = psimd_add_f32(vi4, vi5);
      const psimd_f32 vsum67 = psimd_add_f32(vi6, vi7);

      const psimd_f32 vsum2345 = psimd_add_f32(vsum23, vsum45);
      const psimd_f32 vsum01678 = psimd_add_f32(vsum018, vsum67);
      const psimd_f32 vsum = psimd_add_f32(vsum2345, vsum01678);

      psimd_f32 vout = psimd_mul_f32(vsum, vmultiplier);
      vout = psimd_max_f32(vout, voutput_min);
      vout = psimd_min_f32(vout, voutput_max);

      psimd_store_f32(output, vout);
      output += 4;

      k -= 4;
    }
    if (k != 0) {
      const psimd_f32 vi0 = psimd_load_f32(i0);
      const psimd_f32 vi1 = psimd_load_f32(i1);
      const psimd_f32 vi2 = psimd_load_f32(i2);
      const psimd_f32 vi3 = psimd_load_f32(i3);
      const psimd_f32 vi4 = psimd_load_f32(i4);
      const psimd_f32 vi5 = psimd_load_f32(i5);
      const psimd_f32 vi6 = psimd_load_f32(i6);
      const psimd_f32 vi7 = psimd_load_f32(i7);
      const psimd_f32 vi8 = psimd_load_f32(i8);

      const psimd_f32 vsum01 = psimd_add_f32(vi0, vi1);
      const psimd_f32 vsum23 = psimd_add_f32(vi2, vi3);
      const psimd_f32 vsum45 = psimd_add_f32(vi4, vi5);
      const psimd_f32 vsum67 = psimd_add_f32(vi6, vi7);
      const psimd_f32 vsum018 = psimd_add_f32(vsum01, vi8);
      const psimd_f32 vsum2345 = psimd_add_f32(vsum23, vsum45);
      const psimd_f32 vsum01678 = psimd_add_f32(vsum018, vsum67);
      const psimd_f32 vsum = psimd_add_f32(vsum2345, vsum01678);

      psimd_f32 vout = psimd_mul_f32(vsum, vmultiplier);
      vout = psimd_max_f32(vout, voutput_min);
      vout = psimd_min_f32(vout, voutput_max);

      if (k & 2) {
        psimd_store2_f32(output, vout);
        output += 2;
        vout = psimd_concat_hi_f32(vout, vout);
      }
      if (k & 1) {
        psimd_store1_f32(output, vout);
        output += 1;
      }
    }
    output = (float*) ((uintptr_t) output + output_increment);
  } while (--n != 0);
}
