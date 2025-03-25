# なにこれ？
五月祭用hopfield network

#使い方
## win系
powershellなどでmain.batを実行
## linux系
シェルでmain.shを実行

# config.txtの見かた
## input_folder output_folder
いじらなくてよい
## noise_mode
flip, salt_and_pepper, gaussian, whiteの4つが使える
## noise_rate
flip, salt_and_pepperのノイズ率
## sigma 
gaussianの $\sigma$
## hopfield_mode
classic, modern, coutinuousの3つが使える。classic,modernを使う場合は2値画像かつnoise_modeはflipかsalt_and_pepperを使用すること。
