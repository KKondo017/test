# weight_comb
ロードセルで質量計測し、自動で出荷規定重量の組み合わせを生成するプロジェクトです。

# 材料
* [Arduino UNO R3](https://www.amazon.co.jp/ELEGOO-ATmega328P-ATMEGA16U2-USB%E3%82%B1%E3%83%BC%E3%83%96%E3%83%AB-Arduino%E7%94%A8/dp/B06Y5TBNQX/ref=sxts_sxwds-bia-wc-p13n1_0?__mk_ja_JP=%E3%82%AB%E3%82%BF%E3%82%AB%E3%83%8A&crid=2UQDYB2JEMOOQ&cv_ct_cx=arduino&dchild=1&keywords=arduino&pd_rd_i=B06Y5TBNQX&pd_rd_r=1203962f-0579-4f1b-96ed-f9f2fad43b6b&pd_rd_w=JUkTG&pd_rd_wg=vh3yg&pf_rd_p=a83e8958-dd0c-43dc-9a4f-eb4f335dec93&pf_rd_r=1RQP8M3DBH9GXQADQ6V4&psc=1&qid=1589163117&sprefix=ardu%2Caps%2C968&sr=1-1-4a4d9315-f5eb-4c06-9950-60e8d500f1b5)
* [ロードセル](http://akizukidenshi.com/catalog/g/gP-13041/)
* [ロードセル用ADコンバータモジュール(HX711)](http://akizukidenshi.com/catalog/g/gK-12370/)
* [ブレッドボード](http://akizukidenshi.com/catalog/g/gP-05294/)
* [ジャンパーワイヤ(オス-メス)](http://akizukidenshi.com/catalog/g/gC-08933/)

# 配線
### Arduino⇔ADコンバータモジュール
| Arduino | ADコンバータ |
| ---- | ---- |
|  5V  |  VDD  |
|  GND  |  GND  |
|  10  |  DAT  |
|  11  |  CLK  |

### ADコンバータモジュール⇔ロードセル
| ADコンバータ | ロードセル |
| ---- | ---- |
|  AVDD  |  赤線  |
|  GND  |  黒線  |
|  INNA  |  白線  |
|  INPA  |  緑線  |

# fileの説明
| file | 概要 |
| ---- | ---- |
|  HX711.ino  |  Arduino側の質量計測コード(秋月サンプルソースほぼまま)  |
|  HX711multi.ino  |  単一Arduinoで2つの質量計測系から質量を取得する  |
|  csvwrite.py  |  HX711multi.ino書込みのArduinoから出力される質量2つをCSV吐出しする  |
|  DBwrite.py  |  質量をリアルタイムでデータベースに書き込む  |
|  moveave.py  |  Python側で移動平均し、確定値のみをCSV吐出しする  |
|  20200428.csv  |  moveave.pyの出力結果例  |
|  weight_comb.cpp  |  最適質量組合せを自動計算する  |
|  aiu_a33_s5.csv  |  weight_comb.cpp用の入力ファイル  |

### 注意点
2020/05/11現在、質量計測部と組合せ計算部は連携していません。
