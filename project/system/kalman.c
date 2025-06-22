#include "kalman.h"







float KalmanFilter(float inData)

{
    static float prevData = 0;                                 //��һ������
    static float p = 10, q = 0.001, r = 0.001, kGain = 0;      // q ������� r ������Ӧ�ٶ�
    p = p + q;
    kGain = p / (p + r);                                        //���㿨��������
    inData = prevData + (kGain * (inData - prevData));          //���㱾���˲�����ֵ
    p = (1 - kGain) * p;                                        //���²�������
    prevData = inData;
    return inData;                                             //���ع���ֵ
}


unsigned long kalman_filter(unsigned long ADC_Value)

{
    float LastData;
    float NowData;
    float kalman_adc;
    static float kalman_adc_old = 0;
    static float P1;
    static float Q = 0.005;
    static float R = 3;
    static float Kg = 0;
    static float P = 1;
    NowData = ADC_Value;
    LastData = kalman_adc_old;
    P = P1 + Q;
    Kg = P / (P + R);
    kalman_adc = LastData + Kg * (NowData - kalman_adc_old);
    P1 = (1 - Kg) * P;
    P = P1;
    kalman_adc_old = kalman_adc;
    return (unsigned long)(kalman_adc);
}
