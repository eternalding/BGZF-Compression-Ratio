# BGZF-Compression-Ratio
Parse BGZIP file and get the compressed ratio.\
Input an bgzip file(BGZF), and the program will calculate the ratio of:\
(#compreesed size) / (#uncompressed size)
Any suggestions are welcome.

[Warning] 
Error indicators are designed specificly for BGZF,\ 
please do not use it for gzip files.

## Usage
g++ compressed_rate.cpp -o cmp_rate -std=c++17
./cmp_rate -i <TARGET BGZF>
  
## Demo
![image](https://user-images.githubusercontent.com/39581323/130753253-9ac73195-d290-43ef-83cc-0d3939a70fdd.png)



  
  
