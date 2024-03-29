> # NOTE:
> If you know how to compile native C++ program, please specify the C++ standard to C+11. 
> 
> Then go to CSSR orginal repository <https://github.com/stites/CSSR> download the source code and compile and use the Matlab script https://github.com/mmasque/CSSR_matlab_runner. 

# CSSR-Matlab

[![View CSSR-Matlab on File Exchange](https://www.mathworks.com/matlabcentral/images/matlab-file-exchange.svg)](https://au.mathworks.com/matlabcentral/fileexchange/74604-cssr-matlab)

CSSR is also known as **epsilon machine**. 
**Note that CSSR-Matlab is NOT an official distribution**. This program attempts to create a Matlab interface for CSSR.

Before using CSSR-Matlab, it is recommended to read documentations published in the CSSR official website http://www.bactra.org/CSSR. CSSR-Matlab is based on CSSR, the original algorithm CSSR is developed by [Cosma Shalizi](http://bactra.org/) and Kristina Klinkner. 

If you decide to use CSSR-Matlab, please appropriately cite the paper "Blind Construction of Optimal Nonlinear Recursive Predictors for Discrete Sequences",  [arXiv:cs/0406011 [cs.LG]](https://arxiv.org/abs/cs/0406011). 

CSSR has been successfully applied to brain signals. see [Brain signals can reveal how “awake” a fly’s brain is - MIT Technology Review](https://www.technologyreview.com/2019/06/14/134950/brain-signals-can-reveal-how-awake-a-flys-brain-is/) and [Roberto's paper](https://doi.org/10.1103/PhysRevResearch.2.023219) for details.

If you are looking at Roberto's paper ***General anesthesia reduces complexity and temporal asymmetry of the informational structures derived from neural recordings in Drosophila***, and want to look at the code of temporal asymmetry properties, including causal irreversibility, crypticity and symmetric KL divergence rate, then [Aidan's code repository](https://github.com/aidan-zec/brain_machine) may help. 


## Time Series to Symbolic Vector 

**Roberto N. Muñoz** 's paper "***General anesthesia reduces complexity and temporal asymmetry of the informational structures derived from neural recordings in Drosophila***", [ arXiv:1905.13173 [q-bio.NC]](https://arxiv.org/abs/1905.13173), converts the local field potential (LFP) signals to a binary sequence by using the median split.  

There are some discretisers which can transform time series to symbolic vectors in the `discretiser` directory, including **median** and **difference split**. Here is an example for converting a time series into a symbolic vector by using the median binarization (the value that is higher than the median is denoted as 1, otherwise as 0).   

![image-20210215193547025](https://haohua-li.github.io/shared/imgs/image-20210215193547025.png)

Another recommended discretiser is difference binarization (denote the increment as 1, otherwise as 0). 

![image-20210215193547025](https://haohua-li.github.io/shared/imgs/image-20210215195826278.png)

In addition, some other interesting discretisers like multiple level split (based on percentiles) and symbolic transformation (proposed by this [paper](https://doi.org/10.1016/j.cub.2013.07.075)) are included. If you are interested in the definitions of these discretisers, please read the source code. 



## CSSR-Matlab Usage 

To run CSSR in Matlab, navigator to the directory of **CSSR-Matlab** and execute the following command

```matlab
out = runCSSR(alphabet, data, maxlength, ofilename, siglevel, test_mode)
```

#### Alphabet Set

`alphabet` is the char array which contains all **alphanumeric symbols** used in the input data stream.

**NOTE:** only support `0-9`, `A-Z` and `a-z`. Each character represents a single symbol emitted at a particular period by the process.

Correct Example: 

  * ```matlab
    alphabet = 'ABC';
    alphabet = 'AB'; % or alphabet = '01';
    ```

**Wrong Example** (`10` is not a single symbol) : 

  * ```matlab
    alphabet = '012345678910';
    ```

#### Dataset

`data` is a collection of sequences of symbols (each sequence represents a distinct time series generated from the same stochastic process). If `data` has multiple rows, multiple-line mode will be turned on automatically. Note that data can be the file name of a text stream. 

* ```matlab
  data = 'test1.txt'; % a text stream named 'test1.txt'
  data = {'01001110000111110011'}; % single-line 
  data = {'01111110111'; '01000010110'}; % multi-line and same length
  data = {'011110'; '000011111011010111011100110110'}; % multi-line but different length
  data = {'ABCBABABACCCABC'}; % use more than 2 characters (instead of binary representation)
  ```
  
* Inside the text file `test1.txt`, the content looks like 

   * If `test1.txt` has single line 

     ```
     ABBBBBABABABCCCCABABABCCBBAAABABABCCCBABA
     ```

   * If `test1.txt` has multiple lines 

     ```
     ABABABBABCCCAAABBCACACAB
     BABABCBCBBCCBAAABA
     ABBABBABCACC
     ```

#### History Length 

`maxlength` is the maximum history length considered in causal states. For example, CSSR could recover the first-order Markov Chain from a discrete sequence when L is set to 1. It is important to investigate the correct order of the process. If history length is too large, the result of CSSR may diverge from the true solution. Read [CSS Parameters](https://github.com/randoruf/CSSR-Matlab#cssr-parameters) for more details. 

* ```matlab
  maxlength = 5; 
  ```

#### Output Name

`ofilename` CSSR-Matlab will rename files generated by CSSR. To understand what these output files represent,  please refer to the official CSSR depository <https://github.com/stites/CSSR#3-usage>. 

* ```matlab
  % If using a relative path, files will be placed in the directory of the running script. 
  ofilename = 'cssr'; 
  
  % If using an absolute path, files will be placed in that directory. 
  ofilename = 'C:\Users\????\Desktop\cssr'
  % Note that the current path is 'C:\Users\massw\Desktop' since 'cssr' is the output filename specified by the user.
  ```

#### Significance Level

`siglevel` is the significance level in range from 0 to 1, which is the probability of mistakenly splitting a state. This parameter is 0.001 by default. 

* ```matlab
  siglevel = 0.001;
  ```

* `test_mode` is optional, `'ks'` by default. If `'ch'` flag is set , use Chi-Squared significance test to split causal states instead of the default Kolmogorov-Smirnov test. 

  * ```matlab
    test_mode = 'ks'; 
    test_mode = 'ch'; % NOT RECOMMENDED
    ```

***Note***: **using 'ch' may lead Matlab to crash**. This is a known bug in the function taken from *Numerical Recipes in C*. Try to evaluate the command again if failing.  


## Example usage

```matlab
% add CSSR-Matlab to the path, "C:\Users\????\Documents\MATLAB\CSSR-Matlab" in my case.
if exist('runCSSR', 'file') ~= 2
    cssr_path = fullfile(userpath, 'CSSR-Matlab'); 
    addpath(cssr_path);
end 

% YOUR CODE HERE
% turn on multi-line mode automatically, significance level as 0.001, use 'ks' as test mode, and generate files named 'cssr' by default
out = runCSSR('ABC', {'ABABABCCCCBABABC'; 'ABABABCCCA'; 'BABBBACACACCB'}, 3);
```

The output `out` is a structure which includes fields `out.Cmu`, `out.state_prob` and `out.state_series`. 

* `out.Cmu` the statistical complexity. 
* `out.state_prob` the stationary probability distribution of each causal states. 
* `out.state_series` the series of causal states in the data. 

To access the state series, here is an example 
```
out.state_series{2, 1}{5}
```
It means taking the fifth symbol in the second sequence (the column index of the first curly bracket pair is always 1). 


For details,  please refer to the official CSSR depository <https://github.com/stites/CSSR#3-usage>.



## CSSR-Matlab Installation 

To install, download or clone from Github. Move all extracted files to a folder and add this folder to **Matlab path** (either by `addpath` function or `Set Path` button). 

Once all files are ready, [link C++ compiler with Matlab](https://au.mathworks.com/help/matlab/matlab_external/choose-c-or-c-compilers.html) and run `makeCSSR.m` to compiles *mex* binaries at the first time. 

The recommended C++ compilers on different environments,  

| Operating System | C++ Compiler                            |
| ---------------- | --------------------------------------- |
| Windows          | *Microsoft Visual C++* or *MinGW C/C++* |
| MacOS            | *Xcode*                                 |
| Linux            | *GCC C/C++*                             |

To set up the C++ compiler, follow the instruction prompted upon the Matlab command line window. Further details about compilers can be viewed [here](https://www.mathworks.com/support/requirements/supported-compilers.html). 



## CSSR Known Issues

Due to the limitations in the original C++ implementation, there are a few known issues in CSSR. For more details, see https://github.com/stites/CSSR#5-known-issues. 



## CSSR Parameters

One common question when using CSSR for the first time is how to choose the **history length** and **significance level**. It is important to "tune" these two parameters which may affect the stability/accuracy of CSSR. 

**Suggestions:** 

> exploring the data at low L and high s initially, and then increasing L and lowering s. If a stable architecture is found, it should be recorded at the lowest possible L.

To converge to a true epsilon machine, make sure the history length L is longer than true Markov order M (e.g. **L >= M**), otherwise, the number of states returned **may be incorrect** (less or higher than the actual one). 

However, for a fixed-length N data stream, if keep increasing history length L further than the true Markov order, **there is generally "blow up"** in the number of states (diverge from the true solution).  

For more details, please see https://github.com/stites/CSSR#4-some-suggestions-about-parameters. 



## License and Copyright 

CSSR-Matlab is licensed as GNU General Public License version 2. 

CSSR v0.1.1 Copyright (C) 2002, Kristina Klinkner and Cosma Shalizi. 

All main changes to CSSR can be viewed in GitHub [commits](https://github.com/randoruf/CSSR-Matlab/commit/6063581b4946a48bad61c78c16f529bd2e5efda8).



## Acknowledgements

Although this is a small project, I still would like to say thanks to people in [tLab](https://sites.google.com/monash.edu/tlab/home). I also want to say thanks to [Cosma Shalizi](http://bactra.org/) and Kristina Klinkner, they purposed and developed the theory of CSSR. 



## Related Resources

* tlab (Monash University) https://sites.google.com/monash.edu/tlab/home
* The original website of CSSR http://bactra.org/CSSR/
* Practical Computational Mechanics by James P. Crutchfield: http://csc.ucdavis.edu/~cmg/compmech/tutorials/pcm.pdf
* My summer and winter research reports : https://github.com/randoruf/tlab-intern-code-2019-2020
* [Lempel–Ziv](https://web.stanford.edu/class/ee376a/files/EE376C_lecture_LZ.pdf) may help you to understand the underlying mechanism of CSSR (in fact, it is a kind of decoding algorithms).
