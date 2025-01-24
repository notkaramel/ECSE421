# Sensors & Actuators

## Affine models/functions
- Linear function are affine
- Most affine function has bias

## Range
- Sensitivity is non-linear
- Quantization bits required for ADC

## Precision
- p = Range / # bits

## Dynamic Range
- D (dB) = 20 log_10((H-L)/p)
- Unchanged when the range & increment/precision is changed

## Quantization, Errors, Noises
- Quantization is a form of noise
- More bits added = less quantization error (reach -> 0 eventually)
- Root Mean Square Error (Noise RMS) = sqrt( 1/(H-L) * int_L^H{noise(t)^2}dt)

