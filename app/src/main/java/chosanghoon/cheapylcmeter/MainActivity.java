/*
  MainActivity.java:
  CheapyLCMeter LC measuring code
  Copyright (c) 2016, Sanghoon Cho
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
  * Neither the name of the <organization> nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

package chosanghoon.cheapylcmeter;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import org.achartengine.ChartFactory;
import org.achartengine.GraphicalView;
import org.achartengine.chart.PointStyle;
import org.achartengine.model.XYMultipleSeriesDataset;
import org.achartengine.model.XYSeries;
import org.achartengine.renderer.XYMultipleSeriesRenderer;
import org.achartengine.renderer.XYSeriesRenderer;

import java.text.DecimalFormat;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("openslIO");
    }

    private boolean iOrC = true;//true : 인덕터 측정, false : 콘덴서 측정
    private int refMillibel = -2000;//기준 음량
    private int firstFreq = 1000;
    private int lastFreq = 10000;
    private int midFreq = 10000;
    private double first = -1;
    private double last = -1;
    private RadioGroup typeGroup;
    private RadioButton typeInductor;
    private RadioButton typeCapacitor;
    private EditText fixedInductor;
    private EditText fixedCapacitor;
    private TextView resultInductor;
    private TextView resultCapacitor;
    private Button measure;
    private Button calibrate;
    private LinearLayout layout;
    private XYSeries series;
    private XYMultipleSeriesDataset dataSet;
    private GraphicalView chart;
    private Handler handler;
    private Runnable result;

    public static native void openAudioDevice();

    public static native void closeAudioDevice();

    public static native float getAmplitude(int frequency);//약 500ms동안 녹음해서 sine wave 음량출력

    public static native void genTone(int frequency, int millibel);//약 100ms동안 sine wave 출력

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        typeGroup = (RadioGroup) findViewById(R.id.type_group);
        typeInductor = (RadioButton) findViewById(R.id.type_inductor);
        typeCapacitor = (RadioButton) findViewById(R.id.type_capacitor);
        fixedInductor = (EditText) findViewById(R.id.fixed_inductor);
        fixedCapacitor = (EditText) findViewById(R.id.fixed_capacitor);
        resultInductor = (TextView) findViewById(R.id.result_inductor);
        resultCapacitor = (TextView) findViewById(R.id.result_capacitor);
        measure = (Button) findViewById(R.id.measure);
        calibrate = (Button) findViewById(R.id.calibrate);
        layout = (LinearLayout) findViewById(R.id.chart);
        series = new XYSeries("");
        dataSet = new XYMultipleSeriesDataset();
        dataSet.addSeries(series);
        chart = ChartFactory.getLineChartView(getApplicationContext(), dataSet, getRenderer());
        layout.addView(chart);
        handler = new Handler();
        result = new Runnable() {
            @Override
            public void run() {
                double value;
                double fixedL = Double.valueOf(fixedInductor.getText().toString());//고정된 인덕터 값
                double fixedC = Double.valueOf(fixedCapacitor.getText().toString());//고정된 콘덴서 값
                String pattern = ".####";//소수점 4자리까지
                DecimalFormat dFormat = new DecimalFormat(pattern);
                value = Double.valueOf(dFormat.format(lcResonance(iOrC, fixedL, fixedC, midFreq)));

                if (iOrC) {
                    resultInductor.setText(String.valueOf(value));
                    resultCapacitor.setText(String.valueOf(0));
                } else {
                    resultInductor.setText(String.valueOf(0));
                    resultCapacitor.setText(String.valueOf(value));
                }
            }
        };

        openAudioDevice();

        AlertDialog.Builder alert = new AlertDialog.Builder(MainActivity.this);
        alert.setPositiveButton("Close", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();     //닫기
            }
        });
        alert.setMessage(R.string.alert);
        alert.show();


        measure.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                Thread measurement = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        double temp;
                        double min = 32768;
                        int minFreq = 10000;

                        for (int i = 0; i < 10; ++i) {
                            temp = audioLoopback(1000 + 1000 * i, calMillibel(1000 + 1000 * i));
                            series.add(1000 + 1000 * i, temp);
                            if (min > temp) {
                                min = temp;
                                minFreq = 1000 + 1000 * i;
                            }
                        }//1단계, 탐색간격 : 1000
                        chart.repaint();
                        midFreq = minFreq;
                        for (int i = 0; i < 20; ++i) {
                            temp = audioLoopback(midFreq - 1000 + 100 * i, calMillibel(midFreq - 1000 + 100 * i));
                            series.add(midFreq - 1000 + 100 * i, temp);
                            if (min > temp) {
                                min = temp;
                                minFreq = midFreq - 1000 + 100 * i;
                            }
                        }//2단계, 탐색간격 : 100
                        chart.repaint();
                        midFreq = minFreq;
                        for (int i = 0; i < 20; ++i) {
                            temp = audioLoopback(midFreq - 100 + 10 * i, calMillibel(midFreq - 100 + 10 * i));
                            series.add(midFreq - 100 + 10 * i, temp);
                            if (min > temp) {
                                min = temp;
                                minFreq = midFreq - 100 + 10 * i;
                            }
                        }//3단계, 탐색간격 : 10
                        chart.repaint();
                        midFreq = minFreq;
                        handler.post(result);
                    }
                });

                series.clear();
                measurement.start();
            }
        });//인덕터 또는 콘덴서 용량 계산 및 출력

        calibrate.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                Thread calibration = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        first = audioLoopback(firstFreq, refMillibel);
                        last = audioLoopback(lastFreq, refMillibel);
                        series.clear();
                        for (int i = firstFreq; i <= lastFreq; i += 1000)
                            series.add(i, audioLoopback(i, calMillibel(i)));
                        chart.repaint();
                    }
                });

                calibration.start();
            }
        });//주파수음답특성이 선형적이게 조정

        typeGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                switch (checkedId) {
                    case R.id.type_inductor:
                        iOrC = true;
                        break;
                    case R.id.type_capacitor:
                        iOrC = false;
                        break;
                }
            }
        });//인덕터 또는 콘덴서 중 무엇을 측정할건지 선택

    }

    protected void onDestroy() {
        super.onDestroy();
        closeAudioDevice();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (KeyEvent.KEYCODE_BACK == keyCode) {
            String alertTitle = getResources().getString(R.string.app_name); // alert 타이
            String buttonMessage = getResources().getString(R.string.msg); // alert 메시지
            String buttonYes = getResources().getString(R.string.btn_yes);   // yes버튼 글자
            String buttonNo = getResources().getString(R.string.btn_no);      // no버튼 글자
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle(alertTitle);
            builder.setMessage(buttonMessage);
            builder.setNegativeButton(buttonNo, null);
            builder.setPositiveButton(buttonYes, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    moveTaskToBack(true);
                    finish();
                }
            });
            builder.show();
        }
        return true;
    }

    private void setChartSettings(XYMultipleSeriesRenderer renderer) {
        renderer.setChartTitle("Frequency Response");
        renderer.setXTitle("Frequency");
        renderer.setYTitle("Response");
        renderer.setRange(new double[]{firstFreq, lastFreq, 0, 32768});
        renderer.setApplyBackgroundColor(true);
        renderer.setBackgroundColor(Color.WHITE);
        renderer.setMarginsColor(Color.WHITE);
        renderer.setAxisTitleTextSize(12);
        renderer.setChartTitleTextSize(12);
        renderer.setLabelsTextSize(15);
        renderer.setLegendTextSize(15);
        renderer.setPointSize(5f);
        renderer.setMargins(new int[]{20, 20, 50, 50});
        renderer.setAxesColor(Color.BLACK);
        renderer.setLabelsColor(Color.BLACK);
        renderer.setXLabels(5);
        renderer.setYLabels(5);
        renderer.setXAxisMin(firstFreq);
        renderer.setXAxisMax(lastFreq);
        renderer.setYAxisMin(0);
        renderer.setYAxisMax(32768);
        renderer.setClickEnabled(false);
        renderer.setZoomEnabled(false, false);
        renderer.setPanEnabled(false, false);
        renderer.setFitLegend(true);
        renderer.setShowGrid(true);
    }//차트 특성 설정

    private XYMultipleSeriesRenderer getRenderer() {
        XYMultipleSeriesRenderer renderer = new XYMultipleSeriesRenderer();
        XYSeriesRenderer r = new XYSeriesRenderer();
        r.setColor(Color.BLACK);
        r.setPointStyle(PointStyle.CIRCLE);
        r.setFillPoints(true);
        renderer.addSeriesRenderer(r);
        setChartSettings(renderer);
        return renderer;
    }

    private int calMillibel(int currentFreq) {
        int result;
        double current;
        double a;

        if ((first < 0) || (last < 0))
            return 0;
        current = ((last - first) / (double) (lastFreq - firstFreq)) * (currentFreq - firstFreq) + first;
        a = current / Math.pow(10, (1.0 / 2000.0) * (double) refMillibel);
        result = (int) (2000 * Math.log10(32767.0 / a));
        return result;
    }//주파수응답특성 조정 함수

    private double lcResonance(boolean iOrC, double fixedL, double fixedC, int lcFreq) {
        if ((fixedL == 0) && (fixedC == 0))
            return -1;
        if (iOrC)
            return (Math.pow(10, 15) / (4 * Math.pow(lcFreq, 2) * Math.pow(Math.PI, 2) * fixedC) - fixedL);
        else
            return (Math.pow(10, 15) / (4 * Math.pow(lcFreq, 2) * Math.pow(Math.PI, 2) * fixedL) - fixedC);
    }//부품의 용량 계산 함수

    private double audioLoopback(int freq, int millibel) {
        if ((freq < firstFreq) || (freq > lastFreq))
            return -1;

        genToneThread genTone = new genToneThread();
        getAmpThread getAmp = new getAmpThread();
        genTone.freq = freq;
        genTone.millibel = millibel;
        getAmp.freq = freq;

        genTone.start();
        getAmp.start();
        try {
            genTone.join();
            getAmp.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        return getAmp.amplitude;
    }//sine wave 출력하고 그것을 측정하는 함수

    private static class genToneThread extends Thread {
        public int freq = 1000;
        public int millibel = 0;

        @Override
        public void run() {
            genTone(freq, millibel);
        }
    }

    private static class getAmpThread extends Thread {
        public int freq = 1000;
        public double amplitude = 0;

        @Override
        public void run() {
            amplitude = getAmplitude(freq);
        }
    }
}


