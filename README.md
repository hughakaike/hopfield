# なにこれ？
五月祭用hopfield network

# 使い方
## windows・linux系
GUI操作を行う場合はpaint.pyを実行

学習用の画像はinput/imagesに入れる。デフォルトでは256×256の画像にのみ対応

hopfield network単体で動かす場合には

```windows
./run.bat
```

```linux系
./run.sh
```

# config.txtの見かた
## input_folder output_folder
いじらなくてよい
## image_file_format
画像フォーマット
## noise_mode
flip, salt_and_pepper, gaussian, whiteの4つが使える
## noise_rate
flip, salt_and_pepperのノイズ率
## sigma 
gaussianの $\sigma$
## hopfield_mode
classic, modern, coutinuousの3つが使える。classic,modernを使う場合は2値画像かつnoise_modeはflipかsalt_and_pepperを使用すること。

画像は神奈川工科大学 情報学部 情報工学科 信号処理応用研究室 (http://www.ess.ic.kanagawa-it.ac.jp/app_images_j.html)から
